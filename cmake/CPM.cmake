set(CPM_DOWNLOAD_VERSION 0.40.5 CACHE STRING "Version of CPM.cmake to download")

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

# Convert the path to an absolute path (handles relative paths and tildes)
cmake_path(SET CPM_DOWNLOAD_LOCATION NORMALIZE "${CPM_DOWNLOAD_LOCATION}")

function(download_cpm)
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
            "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
            "${CPM_DOWNLOAD_LOCATION}"
            STATUS download_status
    )

    list(GET download_status 0 status_code)
    list(GET download_status 1 status_message)
    if (NOT status_code EQUAL 0)
        message(FATAL_ERROR "Failed to download CPM.cmake: ${status_message}")
    endif ()

    file(READ "${CPM_DOWNLOAD_LOCATION}" file_content)
    if ("${file_content}" STREQUAL "")
        message(FATAL_ERROR "Downloaded CPM.cmake is empty. Please check your network connection.")
    endif ()
endfunction()

if (NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
    download_cpm()
else ()
    # Check if the file is empty (e.g., from a failed download)
    file(READ "${CPM_DOWNLOAD_LOCATION}" file_content)
    if ("${file_content}" STREQUAL "")
        download_cpm()
    endif ()
    unset(file_content)
endif ()

include(${CPM_DOWNLOAD_LOCATION})

mark_as_advanced(CPM_DOWNLOAD_VERSION CPM_DOWNLOAD_LOCATION)
