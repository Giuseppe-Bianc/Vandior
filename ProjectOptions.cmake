include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(Vandior_supports_sanitizers)
    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(SUPPORTS_UBSAN ON)
    else ()
        set(SUPPORTS_UBSAN OFF)
    endif ()

    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(SUPPORTS_ASAN OFF)
    else ()
        set(SUPPORTS_ASAN ON)
    endif ()
endmacro()

macro(Vandior_setup_options)
    option(Vandior_ENABLE_HARDENING "Enable hardening" ON)
    option(Vandior_ENABLE_COVERAGE "Enable coverage reporting" OFF)
    cmake_dependent_option(
            Vandior_ENABLE_GLOBAL_HARDENING
            "Attempt to push hardening options to built dependencies"
            ON
            Vandior_ENABLE_HARDENING
            OFF)

    Vandior_supports_sanitizers()

    if (NOT PROJECT_IS_TOP_LEVEL OR Vandior_PACKAGING_MAINTAINER_MODE)
        option(Vandior_ENABLE_IPO "Enable IPO/LTO" OFF)
        option(Vandior_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
        option(Vandior_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(Vandior_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(Vandior_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(Vandior_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
        option(Vandior_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
        option(Vandior_ENABLE_PCH "Enable precompiled headers" OFF)
        option(Vandior_ENABLE_CACHE "Enable ccache" OFF)
        option(Vandior_ENABLE_INDEPT "Enable INDEPT compile definition" ON)
    else ()
        option(Vandior_ENABLE_IPO "Enable IPO/LTO" ON)
        option(Vandior_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
        option(Vandior_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(Vandior_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
        option(Vandior_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
        option(Vandior_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(Vandior_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(Vandior_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(Vandior_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
        option(Vandior_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
        option(Vandior_ENABLE_PCH "Enable precompiled headers" OFF)
        option(Vandior_ENABLE_CACHE "Enable ccache" ON)
        option(Vandior_ENABLE_INDEPT "Enable INDEPT compile definition" ON)
    endif ()

    if (NOT PROJECT_IS_TOP_LEVEL)
        mark_as_advanced(
                Vandior_ENABLE_IPO
                Vandior_WARNINGS_AS_ERRORS
                Vandior_ENABLE_USER_LINKER
                Vandior_ENABLE_SANITIZER_ADDRESS
                Vandior_ENABLE_SANITIZER_LEAK
                Vandior_ENABLE_SANITIZER_UNDEFINED
                Vandior_ENABLE_SANITIZER_THREAD
                Vandior_ENABLE_SANITIZER_MEMORY
                Vandior_ENABLE_UNITY_BUILD
                Vandior_ENABLE_CLANG_TIDY
                Vandior_ENABLE_CPPCHECK
                Vandior_ENABLE_COVERAGE
                Vandior_ENABLE_PCH
                Vandior_ENABLE_CACHE
                Vandior_ENABLE_INDEPT)  # Mark as advanced if not top-level
    endif ()

    Vandior_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
    if (LIBFUZZER_SUPPORTED AND (Vandior_ENABLE_SANITIZER_ADDRESS OR Vandior_ENABLE_SANITIZER_THREAD OR Vandior_ENABLE_SANITIZER_UNDEFINED))
        set(DEFAULT_FUZZER ON)
    else ()
        set(DEFAULT_FUZZER OFF)
    endif ()

    option(Vandior_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()

macro(Vandior_global_options)
    if (Vandior_ENABLE_IPO)
        include(cmake/InterproceduralOptimization.cmake)
        Vandior_enable_ipo()
    endif ()

    Vandior_supports_sanitizers()

    if (Vandior_ENABLE_HARDENING AND Vandior_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)
        if (NOT SUPPORTS_UBSAN
                OR Vandior_ENABLE_SANITIZER_UNDEFINED
                OR Vandior_ENABLE_SANITIZER_ADDRESS
                OR Vandior_ENABLE_SANITIZER_THREAD
                OR Vandior_ENABLE_SANITIZER_LEAK)
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else ()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif ()
        message("${Vandior_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${Vandior_ENABLE_SANITIZER_UNDEFINED}")
        Vandior_enable_hardening(Vandior_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif ()
    if (Vandior_ENABLE_INDEPT)
        add_compile_definitions(INDEPT)
        message(STATUS "INDEPT compile definition enabled")
    endif ()
endmacro()

macro(Vandior_local_options)
    if (PROJECT_IS_TOP_LEVEL)
        include(cmake/StandardProjectSettings.cmake)
    endif ()

    add_library(Vandior_warnings INTERFACE)
    add_library(Vandior_options INTERFACE)

    include(cmake/CompilerWarnings.cmake)
    Vandior_set_project_warnings(
            Vandior_warnings
            ${Vandior_WARNINGS_AS_ERRORS}
            ""
            ""
            ""
            "")

    if (Vandior_ENABLE_USER_LINKER)
        include(cmake/Linker.cmake)
        configure_linker(Vandior_options)
    endif ()

    include(cmake/Sanitizers.cmake)
    Vandior_enable_sanitizers(
            Vandior_options
            ${Vandior_ENABLE_SANITIZER_ADDRESS}
            ${Vandior_ENABLE_SANITIZER_LEAK}
            ${Vandior_ENABLE_SANITIZER_UNDEFINED}
            ${Vandior_ENABLE_SANITIZER_THREAD}
            ${Vandior_ENABLE_SANITIZER_MEMORY})

    set_target_properties(Vandior_options PROPERTIES UNITY_BUILD ${Vandior_ENABLE_UNITY_BUILD})

    if (Vandior_ENABLE_PCH)
        target_precompile_headers(
                Vandior_options
                INTERFACE
                <vector>
                <string>
                <utility>)
    endif ()

    if (Vandior_ENABLE_CACHE)
        include(cmake/Cache.cmake)
        Vandior_enable_cache()
    endif ()

    include(cmake/StaticAnalyzers.cmake)
    if (Vandior_ENABLE_CLANG_TIDY)
        Vandior_enable_clang_tidy(Vandior_options ${Vandior_WARNINGS_AS_ERRORS})
    endif ()

    if (Vandior_ENABLE_CPPCHECK)
        Vandior_enable_cppcheck(${Vandior_WARNINGS_AS_ERRORS} "" # override cppcheck options
        )
    endif ()

    if (Vandior_ENABLE_COVERAGE)
        include(cmake/Tests.cmake)
        Vandior_enable_coverage(Vandior_options)
    endif ()

    if (Vandior_WARNINGS_AS_ERRORS)
        check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
        if (LINKER_FATAL_WARNINGS)
            # This is not working consistently, so disabling for now
            # target_link_options(Vandior_options INTERFACE -Wl,--fatal-warnings)
        endif ()
    endif ()

    if (Vandior_ENABLE_HARDENING AND NOT Vandior_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)
        if (NOT SUPPORTS_UBSAN
                OR Vandior_ENABLE_SANITIZER_UNDEFINED
                OR Vandior_ENABLE_SANITIZER_ADDRESS
                OR Vandior_ENABLE_SANITIZER_THREAD
                OR Vandior_ENABLE_SANITIZER_LEAK)
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else ()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif ()
        Vandior_enable_hardening(Vandior_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif ()

endmacro()
