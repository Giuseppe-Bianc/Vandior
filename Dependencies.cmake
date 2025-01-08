include(cmake/CPM.cmake)

function(AddSpdlogPackage WcharSupport WcharFilenames)
    CPMAddPackage(
            NAME spdlog
            VERSION 1.15.0
            GITHUB_REPOSITORY "gabime/spdlog"
            OPTIONS
            "SPDLOG_FMT_EXTERNAL ON"
            "SPDLOG_ENABLE_PCH ON"
            "SPDLOG_BUILD_PIC ON"
            "SPDLOG_WCHAR_SUPPORT ${WcharSupport}"
            "SPDLOG_WCHAR_FILENAMES ${WcharFilenames}"
            "SPDLOG_SANITIZE_ADDRESS OFF"
    )
endfunction()

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(Vandior_setup_dependencies)

    if (NOT TARGET glm::glm)
        CPMAddPackage(
                NAME glm
                GIT_REPOSITORY https://github.com/g-truc/glm.git
                GIT_TAG master # Use "master" for the latest version
                OPTIONS # Add options if needed
                "GLM_TEST_ENABLE OFF" # Disable tests if needed
                "GLM_ENABLE_CXX_20 ON"
                "GLM_ENABLE_SIMD_AVX2 ON"
        )
    endif ()

    # For each dependency, see if it's
    # already been provided to us by a parent project

    if (NOT TARGET fmtlib::fmtlib)
        CPMAddPackage("gh:fmtlib/fmt#11.1.1")
    endif ()

    if (NOT TARGET spdlog::spdlog)
        if (WIN32)
            AddSpdlogPackage(ON ON)
        else ()
            AddSpdlogPackage(OFF OFF)
        endif ()
    endif ()

    if (NOT TARGET Catch2::Catch2WithMain)
        CPMAddPackage("gh:catchorg/Catch2@3.8.0")
    endif ()

    if (NOT TARGET CLI11::CLI11)
        CPMAddPackage("gh:CLIUtils/CLI11@2.4.2")
    endif ()
#    if (NOT TARGET nlohmann_json::nlohmann_json)
#        CPMAddPackage(
#                NAME nlohmann_json
#                GITHUB_REPOSITORY "nlohmann/json"
#                VERSION 3.11.3
#                OPTIONS
#                "JSON_BuildTests OFF"
#        )
#    endif ()
endfunction()
