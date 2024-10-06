include(GenerateExportHeader)

find_package(glm REQUIRED)
add_library(vandior_core_lib vandiorCore.cpp
        glm_prety_string_cast.cpp)

add_library(Vandior::vandior_core_lib ALIAS vandior_core_lib)

# Retrieve the target name
get_target_property(target_name vandior_core_lib NAME)
include("${CMAKE_SOURCE_DIR}/cmake/Simd.cmake")
set_simd_instructions(${target_name})


target_link_libraries(vandior_core_lib
        PRIVATE
        Vandior_options
        Vandior_warnings
        PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
        #nlohmann_json::nlohmann_json
)

target_include_directories(vandior_core_lib
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
if (Vandior_BUILD_FUZZ_TESTS)
    target_link_libraries(vandior_core_lib PRIVATE -fsanitize=fuzzer-no-link)
    target_compile_options(vandior_core_lib PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(vandior_core_lib PUBLIC cxx_std_${CMAKE_CXX_STANDARD})


set_target_properties(
        vandior_core_lib
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(vandior_core_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/VandiorCore/vandior_core_export.hpp)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(vandior_core_lib PUBLIC VANDIOR_LIB_STATIC_DEFINE)
endif ()
