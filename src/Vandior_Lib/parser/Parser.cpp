#include "Vandior/parser/Parser.hpp"
#include "Vandior/parser/ParserException.hpp"
#include <charconv>
#include <system_error>
#include <utility>

// NOLINTBEGIN(*-include-cleaner, *-no-recursion,*-avoid-magic-numbers, *-magic-numbers)
DISABLE_WARNINGS_PUSH(26445 26481)

namespace vnd {
    std::unique_ptr<ASTNode> Parser::parse() { return parseExpression(); }
    void Parser::consumeToken() noexcept {
        if(position < tokenSize) { position++; }
    }

    const std::vector<StrViewVec> Parser::operatorPrecedence = {{":"},
                                                                {"=", "+=", "-=", "*=", "/=", "^=", "%="},
                                                                {","},
                                                                {"||"},
                                                                {"&&"},
                                                                {"==", "!="},
                                                                {"<", "<=", ">", ">="},
                                                                {"+", "-"},
                                                                {"*", "/"},
                                                                {"^", "%"},
                                                                {"."}};
    const std::vector<TokenType> Parser::types = {
        TokenType::TYPE_I8,   TokenType::TYPE_I16, TokenType::TYPE_I32,  TokenType::TYPE_I64,   TokenType::TYPE_U8,  TokenType::TYPE_U16,
        TokenType::TYPE_U32,  TokenType::TYPE_U64, TokenType::TYPE_F32,  TokenType::TYPE_F64,   TokenType::TYPE_C32, TokenType::TYPE_C64,
        TokenType::TYPE_CHAR, TokenType::TYPE_STRING, TokenType::TYPE_BOOL, TokenType::IDENTIFIER,
    };

    const Token &Parser::getCurrentToken() const { return tokens.at(position); }
    std::size_t Parser::getUnaryOperatorPrecedence(const Token &token) noexcept {
        const auto &tokenValue = token.getValue();
        if(tokenValue == "+" || tokenValue == "-" || tokenValue == "!" || tokenValue == "++" || tokenValue == "--") {
            return operatorPrecedence.size() + 1;
        }
        return 0;
    }

    std::size_t Parser::getOperatorPrecedence(const Token &token) noexcept {
        const auto &tokenValue = token.getValue();
        std::size_t precedence = 0;
        for(const auto &itm : operatorPrecedence) {
            precedence++;
            if(std::find(std::ranges::begin(itm), std::ranges::end(itm), tokenValue) != std::ranges::end(itm)) { return precedence; }
        }
        return 0;
    }
    int Parser::convertToInt(std::string_view str) noexcept {
        int result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if(ec == std::errc()) [[likely]] {
            return result;
        } else if(ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) [[likely]] {
            // Handle error
            return 0;
        }

        return 0;
    }

    int Parser::convertToIntformExa(std::string_view str) noexcept {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 16);

        if(ec == std::errc()) [[likely]] {
            return result;
        } else if(ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) [[likely]] {
            // Handle error
            return 0;
        }

        return 0;
    }

    int Parser::convertToIntformOct(std::string_view str) noexcept {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 8);

        if(ec == std::errc()) [[likely]] {
            return result;
        } else if(ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) [[likely]] {
            // Handle error
            return 0;
        }

        return 0;
    }

    template <typename T>
    T Parser::convertToDouble(std::string_view str) noexcept {
        T result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if(ec == std::errc()) [[likely]] {
            return result;
        } else if(ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) [[unlikely]] {
            // Handle error
            return 0.0;
        }

        return 0.0;
    }

    template <typename T>
    std::complex<T> Parser::convertToImg(std::string_view str) noexcept {
        std::string_view doubleStr;

        if(str.ends_with("f")) {
            doubleStr = str.substr(0, str.size() - 2);
        } else {
            doubleStr = str.substr(0, str.size() - 1);
        }
        return std::complex<T>(0, convertToDouble<T>(doubleStr));
    }

    std::unique_ptr<ASTNode> Parser::parsePrimary() {
        using enum NumberNodeType;
        const Token &currentToken = getCurrentToken();
        const auto &currentType = currentToken.getType();
        const auto &currentValue = currentToken.getValue();
        auto cval = std::string{currentValue};
        
        if(isPreviusColon()) {
            if(std::find(std::ranges::begin(types), std::ranges::end(types), currentType) == std::ranges::end(types)) {
                throw ParserException(currentToken);
            }
            consumeToken();
            return MAKE_UNIQUE(TypeNode, currentToken);
        } else if(currentType == TokenType::INTEGER) {
            consumeToken();
            if(currentValue.starts_with("#o") || currentValue.starts_with("#O")) {
                cval.erase(0, 2);
                return MAKE_UNIQUE(NumberNode<int>, convertToIntformOct(cval), currentToken, Integer);
            }
            if(currentValue.starts_with("#")) {
                cval.erase(0, 1);
                return MAKE_UNIQUE(NumberNode<int>, convertToIntformExa(cval), currentToken, Integer);
            }
            return MAKE_UNIQUE(NumberNode<int>, convertToInt(currentValue), currentToken, Integer);
        } else if(currentType == TokenType::DOUBLE) {
            consumeToken();
            if(currentValue.ends_with("if")) {
                return MAKE_UNIQUE(NumberNode<std::complex<float>>, convertToImg<float>(currentValue), currentToken, ImaginaryFloat);
            }
            if(currentValue.ends_with("i")) {
                return MAKE_UNIQUE(NumberNode<std::complex<double>>, convertToImg<double>(currentValue), currentToken, Imaginary);
            }
            if(currentValue.ends_with("f")) {
                return MAKE_UNIQUE(NumberNode<float>, convertToDouble<float>(currentValue), currentToken, Float);
            }
            return MAKE_UNIQUE(NumberNode<double>, convertToDouble<double>(currentValue), currentToken, Double);
        } else if(currentType == TokenType::BOOLEAN) {
            consumeToken();
            auto value = true;
            if(currentValue == "false") { value = false; }
            return MAKE_UNIQUE(LiteralNode<bool>, value, currentToken, NodeType::Boolean);
        } else if(currentType == TokenType::CHAR) {
            consumeToken();
            return MAKE_UNIQUE(LiteralNode<char>, currentValue.at(0), currentToken, NodeType::Char);
        } else if(currentType == TokenType::STRING) {
            consumeToken();
            return MAKE_UNIQUE(LiteralNode<std::string_view>, currentValue, currentToken, NodeType::String);
        } else if(currentType == TokenType::IDENTIFIER) {
            consumeToken();
            return MAKE_UNIQUE(VariableNode, currentValue, currentToken);
        } else if(currentToken.getValue() == "(") {
            consumeToken();
            auto expression = parseExpression();
            if(getCurrentToken().getValue() == ")") {
                consumeToken();
                return expression;
            } else {
                // Handle error: mismatched parentheses
                throw ParserException(currentToken);
            }
        } else [[unlikely]] {
            // Handle error: unexpected token
            throw ParserException(currentToken);
        }
    }
    std::unique_ptr<ASTNode> Parser::parseUnary(std::size_t parentPrecendence) {
        const auto &currentToken = getCurrentToken();

        const auto unaryOperatorPrecedence = getUnaryOperatorPrecedence(currentToken);
        if(unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPrecendence) {
            consumeToken();
            auto operand = parseExpression(unaryOperatorPrecedence);
            return MAKE_UNIQUE(UnaryExpressionNode, currentToken, std::move(operand));
        } else {
            return parsePrimary();
        }
    }
    std::unique_ptr<ASTNode> Parser::parseBinary(std::size_t parentPrecendence) {
        auto left = parseUnary(parentPrecendence);
        while(true) {
            const auto precedence = getOperatorPrecedence(getCurrentToken());
            if(precedence == 0 || precedence <= parentPrecendence) { break; }
            const Token &opToken = getCurrentToken();
            consumeToken();
            auto right = parseExpression(precedence);
            left = MAKE_UNIQUE(BinaryExpressionNode, opToken, std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<ASTNode> Parser::parseExpression(std::size_t parentPrecendence) { return parseBinary(parentPrecendence); }

    bool Parser::isPreviusColon() const noexcept { return position > 0 && tokens.at(position - 1).getType() == TokenType::COLON; }
}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner,*-no-recursion, *-avoid-magic-numbers,*-magic-numbers)
