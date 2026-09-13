cmake_minimum_required(VERSION 3.25)

project(
    binary_similarity_engine
    VERSION 0.1.0
    DESCRIPTION "High-performance malware similarity and binary tamper analysis engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(CompilerWarnings)
include(Sanitizers)
include(StaticAnalyzers)
include(Optimization)
include(Dependencies)

bsim_setup_dependencies()
bsim_enable_static_analyzers()

# ------------------------------------------------------------
# Shared project configuration
# ------------------------------------------------------------

add_library(bsim_project_options INTERFACE)
add_library(bsim_project_warnings INTERFACE)

target_compile_features(
    bsim_project_options
    INTERFACE
        cxx_std_23
)

bsim_set_project_warnings(bsim_project_warnings)
bsim_enable_sanitizers(bsim_project_options)

# ------------------------------------------------------------
# Core library
# ------------------------------------------------------------

add_library(
    bsim
    STATIC

    src/core/binary_image.cpp
    src/core/section.cpp

    src/io/mapped_file.cpp

    src/formats/binary_format.cpp
    src/formats/pe_parser.cpp
    src/formats/elf_parser.cpp

    src/features/entropy.cpp
    src/features/byte_histogram.cpp
    src/features/strings.cpp
    src/features/imports.cpp
    src/features/ngram.cpp

    src/similarity/similarity_engine.cpp
    src/similarity/minhash.cpp

    src/tamper/tamper_engine.cpp
)

add_library(bsim::bsim ALIAS bsim)

target_include_directories(
    bsim
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    bsim
    PUBLIC
        bsim_project_options

    PRIVATE
        bsim_project_warnings
)

bsim_enable_ipo(bsim)

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------

if(BSIM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ------------------------------------------------------------
# Benchmarks
# ------------------------------------------------------------

if(BSIM_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# ------------------------------------------------------------
# Fuzzing
# ------------------------------------------------------------

if(BSIM_BUILD_FUZZERS)
    add_subdirectory(fuzz)
endif()
