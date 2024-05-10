#include "Vandior/parser/Parser.hpp"
#include <charconv>
#include <system_error>
#include <utility>

// NOLINTBEGIN(*-include-cleaner, *-no-recursion)
DISABLE_WARNINGS_PUSH(26445 26481)

namespace vnd {
    std::unique_ptr<vnd::ASTNode> Parser::parse() { return parseExpression(); }
    void Parser::consumeToken() noexcept {
        if(position < tokenSize) { position++; }
    }

    const Token &Parser::getCurrentToken() const { return tokens.at(position); }
    bool Parser::isUnaryOperator(std::string_view view) noexcept { return view == "+" || view == "-"; }
    int Parser::getOperatorPrecedence(const Token &token) noexcept {
        const auto &tokenValue = token.getValue();
        if(tokenValue == "+" || tokenValue == "-") {
            return 1;
        } else if(tokenValue == "*" || tokenValue == "/") {
            return 2;
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

    double Parser::convertToDouble(std::string_view str) noexcept {
        double result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if(ec == std::errc()) [[likely]] {
            return result;
        } else if(ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) [[unlikely]] {
            // Handle error
            return 0.0;
        }

        return 0.0;
    }
    std::unique_ptr<ASTNode> Parser::parsePrimary() {
        using enum NumberNodeType;
        const Token &currentToken = getCurrentToken();
        const auto &currentType = currentToken.getType();
        const auto &currentValue = currentToken.getValue();
        auto cval = std::string{currentValue};

        if(currentType == TokenType::INTEGER) {
            consumeToken();
            if(currentValue.starts_with("#o") || currentValue.starts_with("#O")) {
                cval.erase(0, 2);
                return std::make_unique<NumberNode<int>>(convertToIntformOct(cval), currentToken, Integer);
            }
            if(currentValue.starts_with("#")) {
                cval.erase(0, 1);
                return std::make_unique<NumberNode<int>>(convertToIntformExa(cval), currentToken, Integer);
            }
            return std::make_unique<NumberNode<int>>(convertToInt(currentValue), currentToken, Integer);
        } else if(currentType == TokenType::DOUBLE) {
            consumeToken();
            return std::make_unique<NumberNode<double>>(convertToDouble(currentValue), currentToken, Double);
        } else if(currentType == TokenType::BOOLEAN) {
            consumeToken();
            auto value = true;
            if(currentValue == "false") { value = false; }
            return std::make_unique<LiteralNode<bool>>(value, currentToken, NodeType::Boolean);
        } else if(currentType == TokenType::CHAR) {
            consumeToken();
            return std::make_unique<LiteralNode<char>>(currentValue.at(0), currentToken, NodeType::Char);
        } else if(currentType == TokenType::STRING) {
            consumeToken();
            return std::make_unique<LiteralNode<std::string_view>>(currentValue, currentToken, NodeType::String);
        } else if(currentType == TokenType::IDENTIFIER) {
            consumeToken();
            return std::make_unique<VariableNode>(currentValue, currentToken);
        } else if(currentToken.getValue() == "(") {
            consumeToken();
            auto expression = parseExpression();
            if(getCurrentToken().getValue() == ")") {
                consumeToken();
                return expression;
            } else {
                // Handle error: mismatched parentheses
                return nullptr;
            }
        } else [[unlikely]] {
            // Handle error: unexpected token
            return nullptr;
        }
    }
    std::unique_ptr<ASTNode> Parser::parseUnary() {
        const Token &currentToken = getCurrentToken();

        if(isUnaryOperator(currentToken.getValue())) {
            consumeToken();
            auto operand = parseUnary();
            return std::make_unique<UnaryExpressionNode>(currentToken, std::move(operand));
        } else {
            return parsePrimary();
        }
    }
    std::unique_ptr<ASTNode> Parser::parseBinary(int precedence) {
        auto left = parseUnary();
        while(getOperatorPrecedence(getCurrentToken()) > precedence) {
            const Token &opToken = getCurrentToken();
            consumeToken();
            auto right = parseUnary();
            left = std::make_unique<BinaryExpressionNode>(opToken, std::move(left), std::move(right));
        }

        return left;
    }
    std::unique_ptr<ASTNode> Parser::parseExpression() { return parseBinary(0); }
}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner,*-no-recursion)
