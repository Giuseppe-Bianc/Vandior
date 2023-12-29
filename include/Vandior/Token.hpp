#pragma once
#include "headers.hpp"  // NOLINT(*-include-cleaner)

enum class TokenType : long {
    INTEGER,
    OPERATOR,
    IDENTIFIER,
    UNKNOWN,
    EOFT,
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
        case TokenType::EOFT:
            name = "EOF";
            break;
        // Add more cases as needed
        case TokenType::UNKNOWN:
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
