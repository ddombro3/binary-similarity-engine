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

add_executable(
    bsim_test_similarity
    unit/test_similarity.cpp
)

add_executable(
    bsim_test_ngram
    unit/test_ngram.cpp
)

add_executable(
    bsim_test_thread_pool
    unit/test_thread_pool.cpp
)

add_executable(
    bsim_test_batch_analysis
    integration/test_batch_analysis.cpp
)

add_executable(
    bsim_test_batch_similarity
    integration/test_batch_similarity.cpp
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

target_link_libraries(
    bsim_test_similarity
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_ngram
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_thread_pool
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_batch_analysis
    PRIVATE
        bsim::bsim
        bsim_project_options
        bsim_project_warnings
)

target_link_libraries(
    bsim_test_batch_similarity
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

target_compile_features(
    bsim_test_similarity
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_ngram
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_thread_pool
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_batch_analysis
    PRIVATE
        cxx_std_23
)

target_compile_features(
    bsim_test_batch_similarity
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

add_test(
    NAME similarity
    COMMAND bsim_test_similarity
)

add_test(
    NAME ngram
    COMMAND bsim_test_ngram
)

add_test(
    NAME thread_pool
    COMMAND bsim_test_thread_pool
)

add_test(
    NAME batch_analysis
    COMMAND bsim_test_batch_analysis
)

add_test(
    NAME batch_similarity
    COMMAND bsim_test_batch_similarity
)