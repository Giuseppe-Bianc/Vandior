#
# This function will prevent in-source builds
#
function(Vandior_assure_out_of_source_builds)
    # make sure the user doesn't play dirty with symlinks
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

    # disallow in-source builds
    if ("${srcdir}" STREQUAL "${bindir}")
        message(STATUS "######################################################")
        message(STATUS "Warning: In-source builds are disabled.")
        message(STATUS "Please create a separate build directory and run CMake from there.")
        message(STATUS "Source directory: ${srcdir}")
        message(STATUS "Binary directory: ${bindir}")
        message(STATUS "######################################################")
        message(FATAL_ERROR "Quitting configuration.")
    endif ()
endfunction()

Vandior_assure_out_of_source_builds()
