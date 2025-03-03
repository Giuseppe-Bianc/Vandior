//
// Created by gbian on 01/01/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "AST.hpp"
#include "Vandior/lexer/Tokenizer.hpp"
#include "Vandior/parser/Statement.hpp"

namespace vnd {
    /**
     * @class Parser
     * @brief Parses tokens into an Abstract Syntax Tree (AST).
     */
    class Parser {
    public:
        /**
         * @brief Constructs a Parser object.
         * @param input The input string to be tokenized and parsed.
         * @param fileName The name of the file being parsed.
         */
        [[nodiscard]] explicit Parser(const std::string_view &input, const std::string_view &fileName)
          : tokenizer{input, fileName}, tokens{tokenizer.tokenize()} {}

        /**
         * @brief Parses the tokens into an AST.
         * @return A unique pointer to the root AST node.
         */
        std::vector<Statement> parse();

        /**
         * @brief Converts a string view to an integer.
         * @param str The string view to convert.
         * @return The integer representation of the string view.
         */
        static int convertToInt(std::string_view str);
        /**
         * @brief Converts a hexadecimal string view to an integer.
         * @param str The hexadecimal string view to convert.
         * @return The integer representation of the hexadecimal string view.
         */
        static int convertToIntformExa(std::string_view str);
        /**
         * @brief Converts an octal string view to an integer.
         * @param str The octal string view to convert.
         * @return The integer representation of the octal string view.
         */
        static int convertToIntformOct(std::string_view str);

    private:
        static const std::vector<StrViewVec> operatorPrecedence;  ///< Operator precedence levels.
        static const std::vector<TokenType> types;                ///< Token types.

#ifndef __llvm__
        template <typename T> static T handle_from_chars_error(const std::from_chars_result &result, std::string_view str);
#endif

        /**
         * @brief create a statement and emplace it in the parsing result.
         */
        void emplaceStatement(std::vector<Statement> &statements);

        /**
         * @brief Consumes the current token and advances to the next token.
         */
        void consumeToken() noexcept;
        /**
         * @brief Gets the current token.
         * @return A reference to the current token.
         */
        [[nodiscard]] const Token &getCurrentToken() const;
        [[nodiscard]] TokenType getCurrentTokenType() const;
        [[nodiscard]] bool isCurrentTokenType(const TokenType &type) const;
        /**
         * @brief Gets the precedence of an unary operator.
         * @param token The token representing the unary operator.
         * @return The precedence level of the unary operator.
         */
        static std::size_t getUnaryOperatorPrecedence(const Token &token) noexcept;

        static std::size_t adjustPrecedenceForVar(const std::string_view &tokenValue, std::size_t precedence) noexcept;

        /**
         * @brief Gets the precedence of an operator.
         * @param token The token representing the operator.
         * @param type The keyword token type of current statement.
         * @return The precedence level of the operator.
         */
        static std::size_t getOperatorPrecedence(const Token &token, const TokenType &type) noexcept;

        /**
         * @brief Converts a string view to a double.
         * @tparam T The type of the double.
         * @param str The string view to convert.
         * @return The double representation of the string view.
         */
        template <typename T> static T convertToDouble(std::string_view str);

        /**
         * @brief Converts a string view to a complex number.
         * @tparam T The type of the complex number.
         * @param str The string view to convert.
         * @return The complex number representation of the string view.
         */
        template <typename T> static std::complex<T> convertToImg(std::string_view str);

        /**
         * @brief Parses a primary integer expression.
         * @param currentToken The current token being parsed.
         * @param currentValue The string view of the current token's value.
         * @return A unique pointer to the parsed AST node representing the integer.
         */
        std::unique_ptr<ASTNode> parsePrimaryInteger(const Token &currentToken, std::string_view currentValue);
        /**
         * @brief Parses a primary double expression.
         * @param currentToken The current token being parsed.
         * @param currentValue The string view of the current token's value.
         * @return A unique pointer to the parsed AST node representing the double.
         */
        std::unique_ptr<ASTNode> parsePrimaryDouble(const Token &currentToken, const std::string_view &currentValue);
        /**
         * @brief Parses a primary expression.
         * @return A unique pointer to the parsed AST node.
         */
        std::unique_ptr<ASTNode> parsePrimary();
        /**
         * @brief Parses a unary expression.
         * @param parentPrecendence The precedence level of the parent expression.
         * @return A unique pointer to the parsed AST node.
         */
        std::unique_ptr<ASTNode> parseUnary(std::size_t parentPrecendence);

        /**
         * @brief Parses a binary expression.
         * @param parentPrecendence The precedence level of the parent expression.
         * @return A unique pointer to the parsed AST node.
         */
        std::unique_ptr<ASTNode> parseBinary(std::size_t parentPrecendence);

        /**
         * @brief Parses an expression.
         * @param parentPrecendence The precedence level of the parent expression (default is 0).
         * @return A unique pointer to the parsed AST node.
         */
        std::unique_ptr<ASTNode> parseExpression(std::size_t parentPrecendence = 0);

        // FIXME: Function parameters should not be of type "std::unique_ptr<T> const &"
        /**
         * @brief Parses an index expression.
         * @tparam T The type of the node.
         * @param node The node to parse the index for.
         */
        template <typename T> void parseIndex(const std::unique_ptr<T> &node);

        // FIXME: Function parameters should not be of type "std::unique_ptr<T> const &"
        /**
         * @brief Parses an array expression.
         * @param node The node to parse the array for.
         * @return True if the array was successfully parsed, false otherwise.
         */
        [[nodiscard]] bool parseArray(const std::unique_ptr<IndexNode> &node);

        // FIXME: Function parameters should not be of type "std::unique_ptr<T> const &"
        /**
         * @brief Parses a function call expression.
         * @param node The node to parse the function call for.
         * @return True if the function call was successfully parsed, false otherwise.
         */
        [[nodiscard]] bool parseCall(const std::unique_ptr<VariableNode> &node);

        /**
         * @brief Extract the function data from a fun statement tokens.
         * @return The return types of the function.
         */
        [[nodiscard]] StringVec extractFunData();

        Tokenizer tokenizer;                  ///< The tokenizer used to tokenize the input.
        std::vector<TokenVec> tokens{};       ///< The list of tokens.
        TokenVec *currentStatement{nullptr};  ///< The current statement being parsed.
        std::size_t tokenSize{};              ///< The size of the token list.
        std::size_t position{};               ///< The current position in the token list.
        Token keyword{};                      ///< The keyword token of the current statement.
    };
}  // namespace vnd

// NOLINTEND(*-include-cleaner)
