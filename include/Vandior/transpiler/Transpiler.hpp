// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../lexer/TokenType.hpp"
#include "../parser/Parser.hpp"
#include "ProjectBuilder.hpp"

namespace vnd {
    class Transpiler {
    public:
        Transpiler(const std::string_view &input, const std::string_view &filename);
        void transpile();

        /**
         * @brief Maps a given type to a corresponding string view.
         *
         * @param type The type to be mapped.
         * @return The corresponding string view.
         */
        static auto mapType(const std::string_view type) -> std::string_view;

    private:
        /**
         * Creates a mock file.
         */
        void createMockfile();

        /**
         * Transpiles the given Token keyword into a string representatio.
         *
         * @param keyword the Token to tranpile.
         * @return The transpiled string representation of the keyword.
         */
        auto transpileKeyword(const Token &keyword) -> std::string;

        /**
         * Transpiles the given ASTNode into a string representation.
         *
         * @param node The ASTNode to transpile.
         * @return The transpiled string representation of the node.
         */
        auto transpileNode(const ASTNode &node) -> std::string;

        /**
         * @brief Transpiles a binary expression node with a specific format.
         *
         * This function takes a binary expression node and an output stream, and transpiles the node
         * into a string representation using the specified format.
         *
         * @param binaryNode The binary expression node to transpile.
         * @param code The output stream to write the transpiled code to.
         * @param format The format to use for transpiling the binary expression node.
         */
        void transpileBinaryExpressionNodeWhitFormat(const BinaryExpressionNode *binaryNode, std::ostringstream &code,
                                                     const std::string &format);
        /**
         * Transpiles a binary expression node.
         *
         * @param binaryNode The binary expression node to transpile.
         * @return The transpiled binary expression as a string.
         */
        auto transpileBinaryExpressionNode(const BinaryExpressionNode *binaryNode) -> std::string;
        /**
         * Transpiles a unary expression node.
         *
         * @param unaryNode The unary expression node to transpile.
         * @return The transpiled unary expression as a string.
         */
        auto transpileUnaryExpressionNode(const UnaryExpressionNode *unaryNode) -> std::string;
        /**
         * Transpiles a variable node into a string representation.
         *
         * @param variableNode The variable node to transpile.
         * @return The string representation of the transpiled variable node.
         */
        auto transpileVariableNode(const VariableNode *variableNode) -> std::string;
        /**
         * @brief Transpiles a numeric node.
         *
         * This function takes a pointer to a NumberNode and transpiles it into a string representation.
         * The transpiled string is returned.
         *
         * @tparam T The type of the numeric value.
         * @param numberNode Pointer to the NumberNode to be transpiled.
         * @return The transpiled string representation of the numeric node.
         */
        template <typename T> auto transpileNumericNode(const NumberNode<T> *numberNode) -> std::string;
        /**
         * @brief Transpiles a literal node to a string representation.
         *
         * This function takes a literal node as input and returns a string representation of the node.
         * The template parameter `T` specifies the type of the literal value.
         *
         * @param literalNode A pointer to the literal node to be transpiled.
         * @return The string representation of the transpiled literal node.
         */
        template <typename T> auto transpileLiteralNode(const LiteralNode<T> *literalNode) -> std::string;
        template <typename T> auto getValueOrLog(const std::optional<T> &opt, std::string_view errorMsg) -> T;
        /**
         * Transpiles the given TypeNode into a string representation.
         *
         * @param typeNode The TypeNode to be transpiled.
         * @return The string representation of the transpiled TypeNode.
         */
        auto transpileTypeNode(const TypeNode *typeNode) -> std::string;
        /**
         * Transpiles the given IndexNode into a string representation.
         *
         * @param indexNode The IndexNode to transpile.
         * @return A pair containing the transpiled string representation of the IndexNode and of the ArrayNode.
         */
        auto transpileIndexNode(const IndexNode *indexNode) -> std::pair<std::string, std::string>;
        /**
         * Transpiles an ArrayNode into a string representation.
         *
         * @param arrayNode The ArrayNode to transpile.
         * @return The transpiled string representation of the ArrayNode.
         */
        auto transpileArrayNode(const ArrayNode *arrayNode) -> std::string;
        std::string_view _filename;
        ProjectBuilder _projectBuilder;
        Parser _parser;
        fs::path _vnBuildFolder;
        fs::path _vnBuildSrcFolder;
        fs::path _mainOutputFilePath;
    };

    struct TranspileContext {
        std::ostringstream code;  // Holds the transpiled code.
        std::string index;        // Holds the current index used in transpilation.
        std::string arr;          // Holds the array or collection name.

        // Method to set both index and array using std::pair, using std::move for efficiency
        void setIndexAndArray(const std::pair<std::string, std::string> &idxAndArray) {
            index = idxAndArray.first;
            arr = idxAndArray.second;
        }
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)
