// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../parser/Parser.hpp"
#include "ProjectBuilder.hpp"

namespace vnd {
    class Transpiler {
    public:
        Transpiler(const std::string_view &input, const std::string_view &filename);
        void transpile();

    private:
        void createMockfile();
        static auto mapType(const std::string_view type) -> std::string_view;
        auto transpileNode(const ASTNode &node) -> std::string;
        auto transpileBinaryExpressionNode(const BinaryExpressionNode *binaryNode) -> std::string;
        auto transpileUnaryExpressionNode(const UnaryExpressionNode *unaryNode) -> std::string;
        auto transpileVariableNode(const VariableNode *variableNode) -> std::string;
        template <typename T> auto transpileNumericNode(const NumberNode<T> *numberNode) -> std::string;
        template <typename T> auto transpileLiteralNode(const LiteralNode<T> *literalNode) -> std::string;
        auto transpileTypeNode(const TypeNode *typeNode) -> std::string;
        auto transpileIndexNode(const IndexNode *indexNode) -> std::string;
        auto transpileArrayNode(const ArrayNode *arrayNode) -> std::string;
        std::string_view _filename;
        ProjectBuilder _projectBuilder;
        Parser _parser;
        fs::path _vnBuildSrcFolder;
        fs::path _mainOutputFilePath;
    };
}  // namespace vnd

// NOLINTEND(*-include-cleaner)
