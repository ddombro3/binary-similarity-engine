add_executable(
    bsim_test_mapped_file
    unit/test_mapped_file.cpp
)

add_executable(
    bsim_test_entropy
    unit/test_entropy.cpp
)

add_executable(
    bsim_test_byte_histogram
    unit/test_byte_histogram.cpp
)

add_executable(
    bsim_test_strings
    unit/test_strings.cpp
)

add_executable(
    bsim_test_binary_format
    unit/test_binary_format.cpp
)

add_executable(
    bsim_test_elf_parser
    unit/test_elf_parser.cpp
)

add_executable(
    bsim_test_pe_parser
    unit/test_pe_parser.cpp
)

add_executable(
    bsim_test_analysis_pipeline
    integration/test_analysis_pipeline.cpp
)

target_link_libraries(
    bsim_test_mapped_file
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_entropy
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_byte_histogram
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_strings
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_binary_format
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_elf_parser
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_pe_parser
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_analysis_pipeline
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_compile_features(
    bsim_test_mapped_file
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_entropy
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_byte_histogram
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_strings
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_binary_format
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_elf_parser
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_pe_parser
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_analysis_pipeline
    PRIVATE
        cxx_std_23
)

add_test(
    NAME mapped_file
    COMMAND bsim_test_mapped_file
)

add_test(
    NAME entropy
    COMMAND bsim_test_entropy
)

add_test(
    NAME byte_histogram
    COMMAND bsim_test_byte_histogram
)

add_test(
    NAME strings
    COMMAND bsim_test_strings
)

add_test(
    NAME binary_format
    COMMAND bsim_test_binary_format
)

add_test(
    NAME elf_parser
    COMMAND bsim_test_elf_parser
)

add_test(
    NAME pe_parser
    COMMAND bsim_test_pe_parser
)

add_test(
    NAME analysis_pipeline
    COMMAND bsim_test_analysis_pipeline
)