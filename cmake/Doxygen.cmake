# Enable doxygen doc builds of source
function(Vandior_enable_doxygen DOXYGEN_THEME)
    message(STATUS "Configuring Doxygen documentation...")

    # Ensure Doxygen is available
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    # Initialize variables
    set(doxygen_conf "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    # Default to top README file as the main page if not specified
    if (NOT DEFINED DOXYGEN_USE_MDFILE_AS_MAINPAGE AND EXISTS "${PROJECT_SOURCE_DIR}/README.md")
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    endif()

    # Set default values
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    # svg files are much smaller than jpeg and png, and yet they have higher quality
    set(DOXYGEN_DOT_IMAGE_FORMAT "svg")
    set(DOXYGEN_DOT_TRANSPARENT YES)

    # Exclude vcpkg and _deps directories if not specified
    if (NOT DEFINED DOXYGEN_EXCLUDE_PATTERNS)
        set(DOXYGEN_EXCLUDE_PATTERNS
                "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/*"
                "${CMAKE_CURRENT_BINARY_DIR}/_deps/*"
        )
    endif()

    # Set Doxygen default theme if none provided
    if (NOT DOXYGEN_THEME)
        set(DOXYGEN_THEME "awesome-sidebar")
    endif()

    if (DOXYGEN_THEME STREQUAL "awesome" OR DOXYGEN_THEME STREQUAL "awesome-sidebar")
        # Use a modern doxygen theme
        # https://github.com/jothepro/doxygen-awesome-css v2.3.4
        message(STATUS "Using Doxygen Awesome theme: ${DOXYGEN_THEME}")
        FetchContent_Declare(doxygen_theme
                URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v2.3.4.zip)
        FetchContent_MakeAvailable(doxygen_theme)
        if (DOXYGEN_THEME STREQUAL "awesome" OR DOXYGEN_THEME STREQUAL "awesome-sidebar")
            set(DOXYGEN_HTML_EXTRA_STYLESHEET "${doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")
        endif ()
        if (DOXYGEN_THEME STREQUAL "awesome-sidebar")
            list(APPEND DOXYGEN_HTML_EXTRA_STYLESHEET
                    "${doxygen_theme_SOURCE_DIR}/doxygen-awesome-sidebar-only.css")
        endif()
    else()
        message(WARNING "Unknown Doxygen theme '${DOXYGEN_THEME}'. Using default theme.")
    endif()

    # Find Doxygen and Dot if available
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    # Add Doxygen-docs target
    message(STATUS "Adding `doxygen-docs` target to build the documentation.")
    doxygen_add_docs(doxygen-docs ALL ${PROJECT_SOURCE_DIR}
            COMMENT "Generating Doxygen documentation at: ${CMAKE_CURRENT_BINARY_DIR}/html/index.html Open the above file in a web browser to view the documentation.")
    message(STATUS "Doxygen documentation configuration completed.")
endfunction()
