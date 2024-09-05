#include "Vandior/parser/Parser.hpp"
#include "Vandior/parser/ParserException.hpp"
#include <charconv>
#include <system_error>
#include <utility>

// NOLINTBEGIN(*-include-cleaner, *-no-recursion,*-avoid-magic-numbers, *-magic-numbers, *-err58-cpp)
DISABLE_WARNINGS_PUSH(26410 26411 26415 26445 26481)

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
    const std::vector<TokenType> Parser::types = {TokenType::TYPE_I8,   TokenType::TYPE_I16,    TokenType::TYPE_I32, TokenType::TYPE_I64,
                                                  TokenType::TYPE_U8,   TokenType::TYPE_U16,    TokenType::TYPE_U32, TokenType::TYPE_U64,
                                                  TokenType::TYPE_F32,  TokenType::TYPE_F64,    TokenType::TYPE_C32, TokenType::TYPE_C64,
                                                  TokenType::TYPE_CHAR, TokenType::TYPE_STRING, TokenType::TYPE_BOOL};

    const Token &Parser::getCurrentToken() const { return tokens.at(position); }
    std::size_t Parser::getUnaryOperatorPrecedence(const Token &token) noexcept {
        if(const auto &tokenValue = token.getValue();
           tokenValue == "+" || tokenValue == "-" || tokenValue == "!" || tokenValue == "++" || tokenValue == "--") {
            return operatorPrecedence.size() + 1;
        }
        return 0;
    }

    std::size_t Parser::getOperatorPrecedence(const Token &token) noexcept {
        const auto &tokenValue = token.getValue();
        std::size_t precedence = 0;
        for(const auto &itm : operatorPrecedence) {
            precedence++;
            if(std::ranges::find(itm, tokenValue) != itm.end()) { return precedence; }
        }
        return 0;
    }
    int Parser::convertToInt(std::string_view str) {
        int result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if(ec == std::errc::invalid_argument) {
            throw std::invalid_argument("parse_integer: invalid argument");
        } else if(ec == std::errc::result_out_of_range) {
            throw std::out_of_range("parse_integer: result out of range");
        } else if(ec != std::errc()) {
            throw std::runtime_error("parse_integer: unknown error");
        }

        if(ptr != str.data() + str.size()) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }

    int Parser::convertToIntformExa(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 16);

        if(ec == std::errc::invalid_argument) {
            throw std::invalid_argument("parse_integer: invalid argument");
        } else if(ec == std::errc::result_out_of_range) {
            throw std::out_of_range("parse_integer: result out of range");
        } else if(ec != std::errc()) {
            throw std::runtime_error("parse_integer: unknown error");
        }

        if(ptr != str.data() + str.size()) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }

    int Parser::convertToIntformOct(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 8);

        if(ec == std::errc::invalid_argument) {
            throw std::invalid_argument("parse_integer: invalid argument");
        } else if(ec == std::errc::result_out_of_range) {
            throw std::out_of_range("parse_integer: result out of range");
        } else if(ec != std::errc()) {
            throw std::runtime_error("parse_integer: unknown error");
        }

        if(ptr != str.data() + str.size()) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }

    template <typename T> T Parser::convertToDouble(std::string_view str) noexcept {
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

    template <typename T> std::complex<T> Parser::convertToImg(std::string_view str) noexcept {
        std::string_view doubleStr;

        if(str.ends_with("f")) {
            doubleStr = str.substr(0, str.size() - 2);
        } else {
            doubleStr = str.substr(0, str.size() - 1);
        }
        return std::complex<T>(0, convertToDouble<T>(doubleStr));
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::unique_ptr<ASTNode> Parser::parsePrimary() {
        using enum NumberNodeType;
        const Token &currentToken = getCurrentToken();
        const auto &currentType = currentToken.getType();
        const auto &currentValue = currentToken.getValue();
        auto cval = std::string{currentValue};

        if(std::ranges::find(types, currentType) != types.end()) {
            consumeToken();
            auto node = MAKE_UNIQUE(TypeNode, currentToken);
            parseIndex<TypeNode>(node);
            return node;
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
            auto node = MAKE_UNIQUE(VariableNode, currentValue, currentToken);
            if(!parseCall(node)) { parseIndex<VariableNode>(node); };
            return node;
        } else if(currentToken.getValue() == "(") {
            consumeToken();
            auto expression = parseExpression();
            if(getCurrentToken().getValue() == ")") {
                consumeToken();
                return expression;
            }
            // Handle error: mismatched parentheses
            throw ParserException(currentToken);
        } else if(currentToken.getValue() == "{") {
            auto token = getCurrentToken();
            consumeToken();
            if(getCurrentToken().getType() == vnd::TokenType::CLOSE_CUR_PARENTESIS) {
                consumeToken();
                return MAKE_UNIQUE(ArrayNode, nullptr, token);
            }
            auto elements = parseExpression();
            if(getCurrentToken().getType() != vnd::TokenType::CLOSE_CUR_PARENTESIS) { throw ParserException(getCurrentToken()); }
            consumeToken();
            return MAKE_UNIQUE(ArrayNode, std::move(elements), token);
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

    template <typename T> void Parser::parseIndex(const std::unique_ptr<T> &node) {
        using enum vnd::TokenType;
        auto token = getCurrentToken();
        if(token.getType() != OPEN_SQ_PARENTESIS) { return; }
        consumeToken();
        if(getCurrentToken().getType() == CLOSE_SQ_PARENTESIS) {
            consumeToken();
            auto index = MAKE_UNIQUE(IndexNode, nullptr, token);
            if(!parseArray(index)) { parseIndex<IndexNode>(index); }
            node->set_index(vnd_move_always_even_const(index));
            return;
        }
        auto elements = parseExpression();
        if(getCurrentToken().getType() != CLOSE_SQ_PARENTESIS) { throw ParserException(getCurrentToken()); }
        consumeToken();
        auto index = MAKE_UNIQUE(IndexNode, std::move(elements), token);
        if(!parseArray(index)) { parseIndex<IndexNode>(index); }
        node->set_index(vnd_move_always_even_const(index));
    }

    bool Parser::parseArray(const std::unique_ptr<IndexNode> &node) {
        using enum vnd::TokenType;
        auto token = getCurrentToken();
        if(token.getType() != OPEN_CUR_PARENTESIS) { return false; }
        consumeToken();
        if(getCurrentToken().getType() == CLOSE_CUR_PARENTESIS) {
            consumeToken();
            node->set_array(MAKE_UNIQUE(ArrayNode, nullptr, token));
            return true;
        }
        auto elements = parseExpression();
        if(getCurrentToken().getType() != CLOSE_CUR_PARENTESIS) { throw ParserException(getCurrentToken()); }
        consumeToken();
        node->set_array(MAKE_UNIQUE(ArrayNode, std::move(elements), token));
        return true;
    }

    bool Parser::parseCall(const std::unique_ptr<VariableNode> &node) {
        using enum vnd::TokenType;
        if(getCurrentToken().getType() != OPEN_PARENTESIS) { return false; }
        consumeToken();
        if(getCurrentToken().getType() == CLOSE_PARENTESIS) {
            consumeToken();
            node->set_call();
            return true;
        }
        auto elements = parseExpression();
        if(getCurrentToken().getType() != CLOSE_PARENTESIS) { throw ParserException(getCurrentToken()); }
        consumeToken();
        node->set_call(std::move(elements));
        return true;
    }

}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-no-recursion,*-avoid-magic-numbers, *-magic-numbers, *-err58-cpp)
