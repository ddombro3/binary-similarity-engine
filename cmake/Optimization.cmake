include(CheckIPOSupported)

function(bsim_enable_ipo target)

    if(NOT BSIM_ENABLE_IPO)
        return()
    endif()

    check_ipo_supported(
        RESULT BSIM_IPO_SUPPORTED
        OUTPUT BSIM_IPO_ERROR
        LANGUAGES CXX
    )

    if(BSIM_IPO_SUPPORTED)
        set_property(
            TARGET ${target}
            PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
        )

        message(STATUS "IPO/LTO enabled for target: ${target}")
    else()
        message(
            WARNING
            "IPO/LTO is not supported: ${BSIM_IPO_ERROR}"
        )
    endif()

endfunction()
