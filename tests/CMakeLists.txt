add_executable(
    bsim_test_mapped_file
    unit/test_mapped_file.cpp
)

target_link_libraries(
    bsim_test_mapped_file
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

add_test(
    NAME mapped_file
    COMMAND bsim_test_mapped_file
)