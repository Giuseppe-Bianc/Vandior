# Enable doxygen doc builds of source
function(Vandior_enable_doxygen DOXYGEN_THEME)
    message(STATUS "Configuring Doxygen documentation...")

    # Ensure Doxygen is available
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    # Initialize variables
    set(doxygen_conf "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    # Default to top README file as the main page if not specified
    if ((NOT DOXYGEN_USE_MDFILE_AS_MAINPAGE) AND EXISTS "${PROJECT_SOURCE_DIR}/README.md")
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    endif ()

    # Set default values
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    # svg files are much smaller than jpeg and png, and yet they have higher quality
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_DOT_TRANSPARENT YES)

    # Exclude vcpkg and _deps directories if not specified
    if (NOT DOXYGEN_EXCLUDE_PATTERNS)
        set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/*" "${CMAKE_CURRENT_BINARY_DIR}/_deps/*")
    endif ()

    # Set Doxygen theme
    if ("${DOXYGEN_THEME}" STREQUAL "")
        set(DOXYGEN_THEME "awesome-sidebar")
    endif ()

    if ("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
        # use a modern doxygen theme
        # https://github.com/jothepro/doxygen-awesome-css v2.3.3
        FetchContent_Declare(_doxygen_theme
                URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v2.3.3.zip)
        FetchContent_MakeAvailable(_doxygen_theme)
        if ("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
            set(DOXYGEN_HTML_EXTRA_STYLESHEET "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")
        endif ()
        if ("${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
            set(DOXYGEN_HTML_EXTRA_STYLESHEET ${DOXYGEN_HTML_EXTRA_STYLESHEET}
                    "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome-sidebar-only.css")
        endif ()
    else ()
        # Use the original Doxygen theme
    endif ()

    # Find Doxygen and Dot if available
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    # Add Doxygen-docs target
    message(STATUS "Adding `doxygen-docs` target to build the documentation.")
    doxygen_add_docs(doxygen-docs ALL ${PROJECT_SOURCE_DIR}
            COMMENT "Generating Doxygen documentation...\nDocumentation will be available at: ${CMAKE_CURRENT_BINARY_DIR}/html/index.html\nOpen the above file in a web browser to view the documentation.")
    message(STATUS "Doxygen documentation configuration completed.")
endfunction()
