include(GenerateExportHeader)

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
        parser/ArrayNode.cpp
        parser/NullptrNode.cpp
        parser/Statement.cpp
        transpiler/Transpiler.cpp
        transpiler/ProjectBuilder.cpp
        parser/AST.cpp
        lexer/ErrorHandler.cpp
)

add_library(Vandior::vandior_lib ALIAS vandior_lib)

# Retrieve the target name
get_target_property(target_name vandior_lib NAME)
include("${CMAKE_SOURCE_DIR}/cmake/Simd.cmake")
set_simd_instructions(${target_name})


target_link_libraries(vandior_lib
        PRIVATE
        Vandior_options
        Vandior_warnings
        PUBLIC
        Vandior::vandior_core_lib
        #nlohmann_json::nlohmann_json
)

target_include_directories(vandior_lib
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
if (Vandior_BUILD_FUZZ_TESTS)
    target_link_libraries(vandior_lib PRIVATE -fsanitize=fuzzer-no-link)
    target_compile_options(vandior_lib PRIVATE -fsanitize=fuzzer-no-link)
endif ()

target_compile_features(vandior_lib PUBLIC cxx_std_${CMAKE_CXX_STANDARD})


set_target_properties(
        vandior_lib
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(vandior_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/Vandior/vandior_export.hpp)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(vandior_lib PUBLIC VANDIOR_LIB_STATIC_DEFINE)
endif ()
