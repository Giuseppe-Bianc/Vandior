add_executable(vandior main.cpp
        Costanti.hpp)


# Retrieve the target name
get_target_property(target_name vandior NAME)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
    target_compile_options(${target_name} PRIVATE /arch:AVX2)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
else ()
    message(STATUS "Impossible to set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
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
