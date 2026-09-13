function(bsim_enable_sanitizers target)

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        return()
    endif()

    if(BSIM_ENABLE_ASAN AND BSIM_ENABLE_TSAN)
        message(
            FATAL_ERROR
            "AddressSanitizer and ThreadSanitizer cannot be enabled together."
        )
    endif()

    set(BSIM_SANITIZER_FLAGS)

    if(BSIM_ENABLE_ASAN)
        list(APPEND BSIM_SANITIZER_FLAGS -fsanitize=address)
    endif()

    if(BSIM_ENABLE_UBSAN)
        list(APPEND BSIM_SANITIZER_FLAGS -fsanitize=undefined)
    endif()

    if(BSIM_ENABLE_TSAN)
        list(APPEND BSIM_SANITIZER_FLAGS -fsanitize=thread)
    endif()

    if(BSIM_SANITIZER_FLAGS)
        target_compile_options(
            ${target}
            INTERFACE
                ${BSIM_SANITIZER_FLAGS}
                -fno-omit-frame-pointer
        )

        target_link_options(
            ${target}
            INTERFACE
                ${BSIM_SANITIZER_FLAGS}
                -fno-omit-frame-pointer
        )
    endif()

endfunction()
