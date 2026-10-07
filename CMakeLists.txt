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
find_package(Threads REQUIRED)

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

target_compile_options(
    bsim_project_options
    INTERFACE
        -stdlib=libc++
)

target_link_options(
    bsim_project_options
    INTERFACE
        -stdlib=libc++
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
    src/core/thread_pool.cpp
    src/core/batch_analyzer.cpp

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
    src/similarity/batch_similarity.cpp
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
        Threads::Threads

    PRIVATE
        bsim_project_warnings
)

bsim_enable_ipo(bsim)

# ------------------------------------------------------------
# CLI
# ------------------------------------------------------------

add_executable(
    bsim_cli
    apps/bsim/main.cpp
    src/cli/cli.cpp
    src/cli/output_panel.cpp
)

target_link_libraries(
    bsim_cli
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_compile_features(
    bsim_cli
    PRIVATE
        cxx_std_23
)

target_compile_definitions(
    bsim_cli
    PRIVATE
        BSIM_VERSION="${PROJECT_VERSION}"
)

set_target_properties(
    bsim_cli
    PROPERTIES
        OUTPUT_NAME bsim
)

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
