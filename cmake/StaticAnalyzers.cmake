function(bsim_enable_static_analyzers)

    if(BSIM_ENABLE_CLANG_TIDY)
        find_program(
            BSIM_CLANG_TIDY
            NAMES
                clang-tidy-23
                clang-tidy
            REQUIRED
        )

        message(STATUS "clang-tidy: ${BSIM_CLANG_TIDY}")

        set(
            CMAKE_CXX_CLANG_TIDY
            "${BSIM_CLANG_TIDY}"
            PARENT_SCOPE
        )
    endif()

endfunction()
