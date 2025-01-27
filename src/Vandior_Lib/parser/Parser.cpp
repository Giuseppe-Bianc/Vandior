#include "Vandior/parser/Parser.hpp"
#include "Vandior/parser/ParserException.hpp"
#include <charconv>
#include <system_error>
#include <utility>

// NOLINTBEGIN(*-include-cleaner, *-no-recursion,*-avoid-magic-numbers, *-magic-numbers, *-err58-cpp, *-suspicious-stringview-data-usage)
DISABLE_WARNINGS_PUSH(26410 26411 26415 26445 26481)

namespace vnd {
    std::vector<Statement> Parser::parse() {
        if(tokens.empty()) { return {}; }
        std::vector<Statement> statements;
        statements.reserve(10);
        emplaceStatement(statements);
        if(tokens.size() == 1 && tokens.at(0).getType() == eofTokenType) {
            statements.back().addNode(nullptr);
        } else {
            statements.back().addNode(parseExpression());
        }
        return statements;
    }

    void Parser::emplaceStatement(std::vector<Statement> &statements) {
        Token token{};
        const auto &tokensFront = tokens.front();
        StringVec data;
        const auto [fst, snd] = checkKeyword(tokensFront.getType());
        if(fst) {
            token = tokensFront;
            tokens.erase(tokens.begin());
            if(token.getType() == TokenType::K_FUN) { data = extractFunData(); }
        }
        if(snd) {
            const auto tokensSize = tokens.size();
            if(tokensSize < 2 || tokens.at(tokensSize - 2).getValue() != "{") { throw ParserException(tokensFront); }
            tokens.erase(tokens.end() - 2);
        }
        statements.emplace_back(token, data);
        keyword = token;
    }

    void Parser::consumeToken() noexcept {
        if(position < tokenSize) { position++; }
    }

    const std::vector<StrViewVec> Parser::operatorPrecedence = {{","},
                                                                {":"},
                                                                {"=", "+=", "-=", "*=", "/=", "^=", "%="},
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
    TokenType Parser::getCurrentTokenType() const { return tokens.at(position).getType(); }
    bool Parser::isCurrentTokenType(const TokenType &type) const { return getCurrentTokenType() == type; }
    std::size_t Parser::getUnaryOperatorPrecedence(const Token &token) noexcept {
        if(const auto &tokenValue = token.getValue();
           tokenValue == "+" || tokenValue == "-" || tokenValue == "!" || tokenValue == "++" || tokenValue == "--") {
            return operatorPrecedence.size() + 1;
        }
        return 0;
    }

    std::size_t Parser::adjustPrecedenceForVar(const std::string_view &tokenValue, std::size_t precedence) noexcept {
        if(tokenValue == ",") { precedence++; }
        if(tokenValue == ":") { precedence--; }
        if(tokenValue == "=") { precedence -= 2; }
        return precedence;
    }

    std::size_t Parser::getOperatorPrecedence(const Token &token, const TokenType &type) noexcept {
        const auto &tokenValue = token.getValue();
        std::size_t precedence = 0;

        for(const auto &itm : operatorPrecedence) {
            precedence++;
            if(std::ranges::find(itm, tokenValue) != itm.end()) {
                if(type == TokenType::K_VAR) { precedence = adjustPrecedenceForVar(tokenValue, precedence); }
                return precedence;
            }
        }
        return 0;
    }
#ifndef __llvm__
    template <typename T> T Parser::handle_from_chars_error(const std::from_chars_result &result, std::string_view str) {
        if(result.ec == std::errc::invalid_argument) { throw std::invalid_argument("parse_integer: invalid argument"); }
        if(result.ec == std::errc::result_out_of_range) { throw std::out_of_range("parse_integer: result out of range"); }
        const std::string_view parsed_view(str.data(), C_ST(result.ptr - str.data()));
        if(parsed_view != str) { throw std::invalid_argument("parse_integer: trailing characters"); }
        return {};
    }
    int Parser::convertToInt(std::string_view str) {
        int result{};
        const auto fcharRes = std::from_chars(str.data(), str.data() + str.size(), result);
        handle_from_chars_error<int>(fcharRes, str);
        return result;
    }

    int Parser::convertToIntformExa(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        const auto fcharRes = std::from_chars(str.data(), str.data() + str.size(), result, 16);
        handle_from_chars_error<int>(fcharRes, str);
        return result;
    }

    int Parser::convertToIntformOct(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        const auto fcharRes = std::from_chars(str.data(), str.data() + str.size(), result, 8);
        handle_from_chars_error<int>(fcharRes, str);
        return result;
    }
#else
    int Parser::convertToInt(std::string_view str) {
        int result{};
        const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
        if(ec == std::errc::invalid_argument) { throw std::invalid_argument("parse_integer: invalid argument"); }
        if(ec == std::errc::result_out_of_range) { throw std::out_of_range("parse_integer: result out of range"); }
        const std::string_view parsed_view(str.data(), C_ST(ptr - str.data()));
        if(parsed_view != str) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }

    int Parser::convertToIntformExa(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 16);
        if(ec == std::errc::invalid_argument) { throw std::invalid_argument("parse_integer: invalid argument"); }
        if(ec == std::errc::result_out_of_range) { throw std::out_of_range("parse_integer: result out of range"); }
        const std::string_view parsed_view(str.data(), C_ST(ptr - str.data()));
        if(parsed_view != str) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }

    int Parser::convertToIntformOct(std::string_view str) {
        int result{};
        // NOLINTNEXTLINE(*-avoid-magic-numbers, *-magic-numbers)
        const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, 8);
        if(ec == std::errc::invalid_argument) { throw std::invalid_argument("parse_integer: invalid argument"); }
        if(ec == std::errc::result_out_of_range) { throw std::out_of_range("parse_integer: result out of range"); }
        const std::string_view parsed_view(str.data(), C_ST(ptr - str.data()));
        if(parsed_view != str) { throw std::invalid_argument("parse_integer: trailing characters"); }

        return result;
    }
#endif

    template <typename T> T Parser::convertToDouble(std::string_view str) {
        T result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if(ec == std::errc::invalid_argument) { throw std::invalid_argument("parse_double: invalid argument"); }
        if(ec == std::errc::result_out_of_range) { throw std::out_of_range("parse_double: result out of range"); }

        return result;
    }

    template <typename T> std::complex<T> Parser::convertToImg(std::string_view str) {
        if(str.ends_with('f')) {
            str.remove_suffix(2);
        } else {
            str.remove_suffix(1);
        }

        return std::complex<T>(0, convertToDouble<T>(str));
    }

    std::unique_ptr<ASTNode> Parser::parsePrimaryInteger(const Token &currentToken, std::string_view currentValue) {
        consumeToken();
        if(currentValue.starts_with("#o") || currentValue.starts_with("#O")) {
            currentValue.remove_prefix(2);
            return MAKE_UNIQUE(VND_NUM_INT, convertToIntformOct(currentValue), currentToken, NumberNodeType::Integer);
        }
        if(currentValue.starts_with("#")) {
            currentValue.remove_prefix(1);
            return MAKE_UNIQUE(VND_NUM_INT, convertToIntformExa(currentValue), currentToken, NumberNodeType::Integer);
        }
        return MAKE_UNIQUE(VND_NUM_INT, convertToInt(currentValue), currentToken, NumberNodeType::Integer);
    }
    std::unique_ptr<ASTNode> Parser::parsePrimaryDouble(const Token &currentToken, const std::string_view &currentValue) {
        consumeToken();
        if(currentValue.ends_with("if")) {
            return MAKE_UNIQUE(VND_NUM_CFLOAT, convertToImg<float>(currentValue), currentToken, NumberNodeType::ImaginaryFloat);
        }
        if(currentValue.ends_with("i")) {
            return MAKE_UNIQUE(VND_NUM_CDOUBLE, convertToImg<double>(currentValue), currentToken, NumberNodeType::Imaginary);
        }
        if(currentValue.ends_with("f")) {
            return MAKE_UNIQUE(VND_NUM_FLOAT, convertToDouble<float>(currentValue), currentToken, NumberNodeType::Float);
        }
        return MAKE_UNIQUE(VND_NUM_DOUBLE, convertToDouble<double>(currentValue), currentToken, NumberNodeType::Double);
    }
    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::unique_ptr<ASTNode> Parser::parsePrimary() {
        using enum NumberNodeType;
        const Token &currentToken = getCurrentToken();
        const auto &currentType = getCurrentTokenType();
        const auto &currentValue = currentToken.getValue();

        if(std::ranges::find(types, currentType) != types.end()) {
            consumeToken();
            auto node = MAKE_UNIQUE(TypeNode, currentToken);
            parseIndex<TypeNode>(node);
            return node;
        } else if(currentType == TokenType::INTEGER) {
            return parsePrimaryInteger(currentToken, currentValue);
        } else if(currentType == TokenType::DOUBLE) {
            return parsePrimaryDouble(currentToken, currentValue);
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
            if(!parseCall(node)) { parseIndex<VariableNode>(node); }
            return node;
        } else if(currentType == TokenType::K_NULLPTR) {
            consumeToken();
            return MAKE_UNIQUE(NullptrNode, currentToken);
        } else if(currentValue == "(") {
            consumeToken();
            auto expression = parseExpression();
            if(getCurrentToken().getValue() == ")") {
                consumeToken();
                return expression;
            }
            // Handle error: mismatched parentheses
            throw ParserException(currentToken);
        } else if(currentValue == "{") {
            auto token = getCurrentToken();
            consumeToken();
            if(isCurrentTokenType(TokenType::CLOSE_CUR_PARENTESIS)) {
                consumeToken();
                return MAKE_UNIQUE(ArrayNode, nullptr, token);
            }
            auto elements = parseExpression();
            if(!isCurrentTokenType(TokenType::CLOSE_CUR_PARENTESIS)) { throw ParserException(getCurrentToken()); }
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
            const auto precedence = getOperatorPrecedence(getCurrentToken(), keyword.getType());
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
        if(isCurrentTokenType(CLOSE_SQ_PARENTESIS)) {
            consumeToken();
            auto index = MAKE_UNIQUE(IndexNode, nullptr, token);
            if(!parseArray(index)) { parseIndex<IndexNode>(index); }
            node->set_index(vnd_move_always_even_const(index));
            return;
        }
        auto elements = parseExpression();
        if(!isCurrentTokenType(CLOSE_SQ_PARENTESIS)) { throw ParserException(getCurrentToken()); }
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
        if(isCurrentTokenType(CLOSE_CUR_PARENTESIS)) {
            consumeToken();
            node->set_array(MAKE_UNIQUE(ArrayNode, nullptr, token));
            return true;
        }
        auto elements = parseExpression();
        if(!isCurrentTokenType(CLOSE_CUR_PARENTESIS)) { throw ParserException(getCurrentToken()); }
        consumeToken();
        node->set_array(MAKE_UNIQUE(ArrayNode, std::move(elements), token));
        return true;
    }

    bool Parser::parseCall(const std::unique_ptr<VariableNode> &node) {
        using enum vnd::TokenType;
        if(!isCurrentTokenType(OPEN_PARENTESIS)) { return false; }
        consumeToken();
        if(isCurrentTokenType(CLOSE_PARENTESIS)) {
            consumeToken();
            node->set_call();
            return true;
        }
        auto elements = parseExpression();
        if(!isCurrentTokenType(CLOSE_PARENTESIS)) { throw ParserException(getCurrentToken()); }
        consumeToken();
        node->set_call(std::move(elements));
        return true;
    }

    StringVec Parser::extractFunData() {
        StringVec result;
        if(tokens.size() <= 3) { return {}; }
        auto iter = std::prev(tokens.end(), 3);
        while(iter->getType() != TokenType::CLOSE_PARENTESIS && std::distance(tokens.begin(), iter) >= 3) {
            result.emplace(result.begin(), iter->getValue());
            iter = tokens.erase(iter);
            --iter;
        }
        return result;
    }
}  // namespace vnd
DISABLE_WARNINGS_POP()
//  NOLINTEND(*-include-cleaner, *-no-recursion,*-avoid-magic-numbers, *-magic-numbers, *-err58-cpp, *-suspicious-stringview-data-usage)
