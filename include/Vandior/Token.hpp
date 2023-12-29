#pragma once
#include "headers.hpp"  // NOLINT(*-include-cleaner)

enum class TokenType : long {
    INTEGER,
    OPERATOR,
    IDENTIFIER,
    // Add more types as needed
};

template <> struct fmt::formatter<TokenType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(TokenType tokenType, FormatContext &ctx) {
        std::string_view name;
        switch(tokenType) {
        case TokenType::INTEGER:
            name = "INTEGER";
            break;
        case TokenType::OPERATOR:
            name = "OPERATOR";
            break;
        case TokenType::IDENTIFIER:
            name = "IDENTIFIER";
            break;
        // Add more cases as needed
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
    Token(TokenType type, std::string_view value) : _type(type), _value(value) {}

    [[nodiscard]] inline TokenType getType() const { return _type; }

    [[nodiscard]] inline std::string_view getValue() const { return _value; }
    [[nodiscard]] std::string to_string() const;  // NOLINT(*-include-cleaner)
    void setType(TokenType type) { _type = type; }
    void setValue(const std::string_view &value) { _value = value; }

private:
    TokenType _type;
    std::string_view _value;
};

template <> struct fmt::formatter<Token> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const Token &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.to_string(), ctx);
    }
};
