include(GenerateExportHeader)

find_package(glm REQUIRED)
add_library(vandior_lib vandior.cpp
        lexer/Token.cpp
        lexer/Tokenizer.cpp
        lexer/TokenizerUtility.cpp
        parser/Parser.cpp
        parser/ASTNode.cpp
        parser/BinaryExpressionNode.cpp
        parser/UnaryExpressionNode.cpp
        parser/NumberNode.cpp
        parser/VariableNode.cpp
        parser/TypeNode.cpp
        parser/IndexNode.cpp
)

add_library(Vandior::vandior_lib ALIAS vandior_lib)

# Retrieve the target name
get_target_property(target_name vandior_lib NAME)

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


target_link_libraries(vandior_lib PRIVATE
        Vandior_options
        Vandior_warnings)
target_link_libraries(vandior_lib PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
        nlohmann_json::nlohmann_json
)

target_include_directories(vandior_lib ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
target_compile_features(vandior_lib PUBLIC cxx_std_23)


set_target_properties(
        vandior_lib
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(vandior_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/Vandior/vandior_export.hpp)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(vandior_lib PUBLIC VANDIOR_LIB_STATIC_DEFINE)
endif ()
