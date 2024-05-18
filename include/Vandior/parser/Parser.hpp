//
// Created by gbian on 01/01/2024.
//

#pragma once
#include "AST.hpp"
#include "Vandior/lexer/Tokenizer.hpp"

namespace vnd {
    class Parser {
    public:
        [[nodiscard]] explicit Parser(const std::string_view &input, const std::string_view &fileName)
          : tokenizer{input, fileName}, tokens{tokenizer.tokenize()}, tokenSize(tokens.size()) {}

        std::unique_ptr<ASTNode> parse();

    private:
        static const std::vector<StrViewVec> operatorPrecedence;
        void consumeToken() noexcept;
        // const Token &getNextToken();
        const Token &getCurrentToken() const;
        static std::size_t getUnaryOperatorPrecedence(const Token &token) noexcept;
        static std::size_t getOperatorPrecedence(const Token &token) noexcept;
        static int convertToInt(std::string_view str) noexcept;
        static int convertToIntformExa(std::string_view str) noexcept;
        static int convertToIntformOct(std::string_view str) noexcept;
        static double convertToDouble(std::string_view str) noexcept;
        std::unique_ptr<ASTNode> parsePrimary();
        std::unique_ptr<ASTNode> parseUnary(std::size_t parentPrecendence);
        std::unique_ptr<ASTNode> parseBinary(std::size_t parentPrecendence);
        std::unique_ptr<ASTNode> parseExpression(std::size_t parentPrecendence = 0);

        Tokenizer tokenizer;
        std::vector<Token> tokens{};
        std::size_t tokenSize{};
        std::size_t position{};
    };
}  // namespace vnd
