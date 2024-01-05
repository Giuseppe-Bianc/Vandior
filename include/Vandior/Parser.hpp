//
// Created by gbian on 01/01/2024.
//

#pragma once
#include "AST.hpp"
#include "Tokenizer.hpp"

namespace vnd {
    class Parser {
    public:
        explicit Parser(const std::string_view &input)
          : tokenizer{input}, tokens{tokenizer.tokenize()}, tokenSize(tokens.size()), position{} {}

        std::unique_ptr<ASTNode> parse();

    private:
        void consumeToken();
        //const Token &getNextToken();
        const Token &getCurrentToken();
        static bool isUnaryOperator(std::string_view view);
        static int getOperatorPrecedence(const Token &token);
        static int convertToInt(std::string_view str);
        static double convertToDouble(std::string_view str);
        std::unique_ptr<ASTNode> parsePrimary();
        std::unique_ptr<ASTNode> parseUnary();
        std::unique_ptr<ASTNode> parseBinary(int precedence);
        std::unique_ptr<ASTNode> parseExpression();

        Tokenizer tokenizer;
        std::vector<Token> tokens;
        std::size_t tokenSize;
        std::size_t position;
    };
}  // namespace vnd