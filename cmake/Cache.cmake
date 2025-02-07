# Enable cache if available
function(Vandior_enable_cache)
    set(CACHE_OPTION "ccache" CACHE STRING "Compiler cache to be used (choices: 'ccache', 'sccache')")
    set(CACHE_OPTION_VALUES "ccache" "sccache")
    set_property(CACHE CACHE_OPTION PROPERTY STRINGS ${CACHE_OPTION_VALUES})
    list(FIND CACHE_OPTION_VALUES "${CACHE_OPTION}" CACHE_OPTION_INDEX)
    if (CACHE_OPTION_INDEX EQUAL -1)
        message(STATUS
                "Using custom compiler cache system: '${CACHE_OPTION}'. Supported options are: ${CACHE_OPTION_VALUES}"
        )
    endif ()

    # Find the binary for the selected cache system (either ccache or sccache)
    find_program(CACHE_BINARY "${CACHE_OPTION}" HINTS ENV PATH NO_CACHE)

    if (CACHE_BINARY)
        message(STATUS "Compiler cache system '${CACHE_BINARY}' found. Enabling cache.")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CACHE_BINARY}" CACHE FILEPATH "C++ compiler cache" FORCE)
        set(CMAKE_C_COMPILER_LAUNCHER "${CACHE_BINARY}" CACHE FILEPATH "C compiler cache" FORCE)
    else ()
        message(WARNING "Selected cache system '${CACHE_OPTION}' was not found. Compiler cache will not be used.")
    endif ()
    mark_as_advanced(CACHE_OPTION)
endfunction()
