function(bsim_set_project_warnings target)

    if(MSVC)
        target_compile_options(
            ${target}
            INTERFACE
                /W4
                /permissive-
        )

        if(BSIM_WARNINGS_AS_ERRORS)
            target_compile_options(${target} INTERFACE /WX)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(
            ${target}
            INTERFACE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Wcast-align
                -Wunused
                -Woverloaded-virtual
                -Wnull-dereference
                -Wdouble-promotion
                -Wformat=2
        )

        if(BSIM_WARNINGS_AS_ERRORS)
            target_compile_options(${target} INTERFACE -Werror)
        endif()
    endif()

endfunction()
