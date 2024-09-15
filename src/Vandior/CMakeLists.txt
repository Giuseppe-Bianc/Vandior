add_executable(vandior main.cpp
        Costanti.hpp)


# Retrieve the target name
get_target_property(target_name vandior NAME)
include("${CMAKE_SOURCE_DIR}/cmake/Simd.cmake")
set_simd_instructions(${target_name})

target_compile_definitions(vandior PRIVATE INDEPT)

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
