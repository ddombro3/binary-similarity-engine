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