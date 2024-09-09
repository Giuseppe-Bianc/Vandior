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
        std::string transpileNode(const ASTNode &node);
        std::string transpileBinaryExpressionNode(const BinaryExpressionNode *binaryNode);
        std::string transpileUnaryExpressionNode(const UnaryExpressionNode *unaryNode);
        std::string transpileVariableNode(const VariableNode *variableNode);
        template <typename T> std::string transpileNumericNode(const NumberNode<T> *numberNode);
        template <typename T> std::string transpileLiteralNode(const LiteralNode<T> *literalNode);
        std::string transpileTypeNode(const TypeNode *typeNode);
        std::string transpileIndexNode(const IndexNode *indexNode);
        std::string transpileArrayNode(const ArrayNode *arrayNode);
        std::string_view _filename;
        ProjectBuilder _projectBuilder;
        Parser _parser;
        fs::path _vnBuildSrcFolder;
        fs::path _mainOutputFilePath;
    };
}  // namespace vnd
