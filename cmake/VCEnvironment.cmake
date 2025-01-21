include("${CMAKE_CURRENT_LIST_DIR}/Utilities.cmake")

macro(detect_architecture)
    # Detect the architecture in a cleaner way
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CMAKE_SYSTEM_PROCESSOR_LOWER)

    if (CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^(x86|i[3456]86)$")
        set(VCVARSALL_ARCH "x86")
    elseif (CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^(x64|x86_64|amd64)$")
        set(VCVARSALL_ARCH "x64")
    elseif (CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL "arm")
        set(VCVARSALL_ARCH "arm")
    elseif (CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^(arm64|aarch64)$")
        set(VCVARSALL_ARCH "arm64")
    else ()
        set(VCVARSALL_ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}")
        message(WARNING "Unknown architecture detected: ${CMAKE_SYSTEM_PROCESSOR_LOWER}. Using host processor architecture: ${VCVARSALL_ARCH}.")
    endif ()
endmacro()

# Run vcvarsall.bat and set CMake environment variables
function(run_vcvarsall)
    # Check if using MSVC and vcvarsall has not been run yet
    if (MSVC AND NOT "$ENV{VSCMD_VER}")
        # Find vcvarsall.bat
        get_filename_component(MSVC_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
        find_file(VCVARSALL_FILE
                NAMES vcvarsall.bat
                PATHS "${MSVC_DIR}"
                "${MSVC_DIR}/.."
                "${MSVC_DIR}/../.."
                "${MSVC_DIR}/../../../../../../../.."
                "${MSVC_DIR}/../../../../../../.."
                PATH_SUFFIXES "VC/Auxiliary/Build" "Common7/Tools" "Tools")

        if (EXISTS "${VCVARSALL_FILE}")
            # Detect the architecture
            detect_architecture()

            # Run vcvarsall.bat and print informative messages
            message(STATUS "Searching for vcvarsall.bat in the Visual Studio directory...")
            message(STATUS "Found vcvarsall.bat at: ${VCVARSALL_FILE}")
            message(STATUS "Detected architecture: ${VCVARSALL_ARCH}")

            # Run vcvarsall.bat to set up the MSVC environment
            message(STATUS "Running `${VCVARSALL_FILE} ${VCVARSALL_ARCH}` to set up the MSVC environment")
            execute_process(
                    COMMAND cmd /c "${VCVARSALL_FILE}" "${VCVARSALL_ARCH}" "&&" "call" "echo" "VCVARSALL_ENV_START" "&" "set"
                    OUTPUT_VARIABLE VCVARSALL_OUTPUT
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

            # Parse the output and set the environment variables
            find_substring_by_prefix(VCVARSALL_ENV "VCVARSALL_ENV_START" "${VCVARSALL_OUTPUT}")
            if (NOT VCVARSALL_ENV)
                message(WARNING "No environment variables found in the vcvarsall.bat output. Check for errors.")
            else ()
                set_env_from_string("${VCVARSALL_ENV}")
                message(STATUS "MSVC environment is set up successfully.")
            endif ()
        else ()
            message(WARNING "Could not find `vcvarsall.bat` for automatic MSVC environment preparation. Please manually open the MSVC command prompt and rebuild the project.")
        endif ()
    endif ()
endfunction()
