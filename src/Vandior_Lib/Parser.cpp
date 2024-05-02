#include "Vandior/Parser.hpp"
#include <charconv>
#include <system_error>
#include <utility>

namespace vnd {
    // NOLINTBEGIN(*-include-cleaner, *-no-recursion)
    std::unique_ptr<vnd::ASTNode> Parser::parse() { return parseExpression(); }
    void Parser::consumeToken() noexcept {
        if(position < tokenSize) { position++; }
    }
    /*const Token &Parser::getNextToken() {  // NOLINT(*-include-cleaner)
        if(position < tokenSize) {
            consumeToken();
            return tokens.at(position);
        }
        return tokens.back();
    }*/
    const Token &Parser::getCurrentToken() { return tokens.at(position); }
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
        const Token &currentToken = getCurrentToken();
        const auto &currentType = currentToken.getType();
        const auto &currentValue = currentToken.getValue();
        auto cval = std::string{currentValue};

        if(currentType == TokenType::INTEGER) {
            consumeToken();
            if(currentValue.starts_with("#o") || currentValue.starts_with("#O")) {
                cval.erase(0, 2);
                return std::make_unique<IntegerNumberNode>(convertToIntformOct(cval));
            }
            if(currentValue.starts_with("#")) {
                cval.erase(0, 1);
                return std::make_unique<IntegerNumberNode>(convertToIntformExa(cval));
            }
            return std::make_unique<IntegerNumberNode>(convertToInt(currentValue));
        } else if(currentType == TokenType::DOUBLE) {
            consumeToken();
            return std::make_unique<DoubleNumberNode>(convertToDouble(currentValue));
        } else if(currentType == TokenType::IDENTIFIER) {
            consumeToken();
            return std::make_unique<VariableNode>(currentValue);
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
            return std::make_unique<UnaryExpressionNode>(currentToken.getValue(), std::move(operand));
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
            left = std::make_unique<BinaryExpressionNode>(opToken.getValue(), std::move(left), std::move(right));
        }

        return left;
    }
    std::unique_ptr<ASTNode> Parser::parseExpression() { return parseBinary(0); }
    // NOLINTEND(*-include-cleaner,*-no-recursion)
}  // namespace vnd
