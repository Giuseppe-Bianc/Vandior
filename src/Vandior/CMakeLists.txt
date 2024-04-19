add_executable(vandior main.cpp
        FileReaderError.hpp
        FolderCreationResult.hpp)


if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(vandior PRIVATE -march=native)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(vandior PRIVATE -march=native)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
    target_compile_options(vandior PRIVATE /arch:AVX2)
else ()
    message(STATUS "Impossible to set simd instructionto to avx2 for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif ()

target_link_libraries(
        vandior
        PRIVATE Vandior::Vandior_options
        Vandior::Vandior_warnings)

target_link_system_libraries(
        vandior
        PRIVATE
        CLI11::CLI11
        Vandior::vandior_lib
)

target_include_directories(vandior PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
