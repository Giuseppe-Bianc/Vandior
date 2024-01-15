#pragma once
#include "headers.hpp"  // NOLINT(*-include-cleaner)

enum class TokenType : long {
    INTEGER,
    DOUBLE,
    BOOLEAN,
    OPERATOR,
    MINUS_OPERATOR,
    EQUAL_OPERATOR,
    IDENTIFIER,
    CHAR,
    STRING,
    K_MAIN,
    K_VAR,
    K_STRUCTURE,
    K_FOR,
    K_FUN,
    K_RETURN,
    OPEN_PARENTESIS,
    OPEN_SQ_PARENTESIS,
    OPEN_CUR_PARENTESIS,
    CLOSE_PARENTESIS,
    CLOSE_SQ_PARENTESIS,
    CLOSE_CUR_PARENTESIS,
    OPERATION_EQUAL,
    BOOLEAN_OPERATOR,
    LOGICAL_OPERATOR,
    UNARY_OPERATOR,
    UNKNOWN,
    EOFT,
    // Add more types as needed
};

template <> struct fmt::formatter<TokenType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(TokenType tokenType, FormatContext &ctx) {
        std::string_view name;
        switch(tokenType) {
            using enum TokenType;
        case INTEGER:
            name = "INTEGER";
            break;
        case DOUBLE:
            name = "DOUBLE";
            break;
        case BOOLEAN:
            name = "BOOLEAN";
            break;
        case OPERATOR:
            name = "OPERATOR";
            break;
        case MINUS_OPERATOR:
            name = "MINUS_OPERATOR";
            break;
        case EQUAL_OPERATOR:
            name = "EQUAL_OPERATOR";
            break;
        case IDENTIFIER:
            name = "IDENTIFIER";
            break;
        case CHAR:
            name = "CHAR";
            break;
        case STRING:
            name = "STRING";
            break;
        case EOFT:
            name = "EOF";
            break;
        case K_MAIN:
            name = "K_MAIN";
            break;
        case K_VAR:
            name = "K_VAR";
            break;
        case K_STRUCTURE:
            name = "K_STRUCTURE";
            break;
        case K_FOR:
            name = "K_FOR";
            break;
        case K_FUN:
            name = "K_FUN";
            break;
        case K_RETURN:
            name = "K_RETURN";
            break;
        case OPEN_PARENTESIS:
            name = "OPEN_PARENTESIS";
            break;
        case OPEN_SQ_PARENTESIS:
            name = "OPEN_SQ_PARENTESIS";
            break;
        case OPEN_CUR_PARENTESIS:
            name = "OPEN_CUR_PARENTESIS";
            break;
        case CLOSE_PARENTESIS:
            name = "CLOSE_PARENTESIS";
            break;
        case CLOSE_SQ_PARENTESIS:
            name = "CLOSE_SQ_PARENTESIS";
            break;
        case CLOSE_CUR_PARENTESIS:
            name = "CLOSE_CUR_PARENTESIS";
            break;
        case OPERATION_EQUAL:
            name = "OPERATION_EQUAL";
            break;
        case BOOLEAN_OPERATOR:
            name = "BOOLEAN_OPERATOR";
            break;
        case LOGICAL_OPERATOR:
            name = "LOGICAL_OPERATOR";
            break;
        case UNARY_OPERATOR:
            name = "UNARY_OPERATOR";
            break;
        // Add more cases as needed
        case UNKNOWN:
            [[fallthrough]];
        default:
            name = "UNKNOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};

// Class representing a Token

class Token {
public:
    Token() noexcept : _type(TokenType::UNKNOWN), _value{}, _line{}, _column{} {}  // NOLINT(*-redundant-member-init)
    Token(TokenType type, const std::string_view &value, std::size_t line, std::size_t column) noexcept
      : _type(type), _value(value), _line(line), _column(column) {}

    [[nodiscard]] inline TokenType getType() const noexcept { return _type; }
    [[nodiscard]] inline std::string_view getValue() const noexcept { return _value; }
    [[nodiscard]] inline std::size_t getLine() const noexcept { return _line; }
    [[nodiscard]] inline std::size_t getColumn() const noexcept { return _column; }
    [[nodiscard]] std::string to_string() const;  // NOLINT(*-include-cleaner)
    inline void setType(TokenType type) noexcept { _type = type; }
    inline void setValue(const std::string_view &value) noexcept { _value = value; }
    void setLine(std::size_t line) noexcept { _line = line; }
    void setColumn(std::size_t column) noexcept { _column = column; }
    auto operator<=>(const Token &) const = default;

private:
    TokenType _type;
    std::string_view _value;
    std::size_t _line;
    std::size_t _column;
};

template <> struct fmt::formatter<Token> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const Token &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.to_string(), ctx);
    }
};
