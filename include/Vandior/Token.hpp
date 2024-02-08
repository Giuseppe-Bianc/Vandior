/**
 * @brief Defines TokenType and Token classes for lexical analysis in a programming language.
 */
#pragma once

#include "CodeSourceLocation.hpp"
#include "headers.hpp"  // NOLINT(*-include-cleaner)
namespace vnd {

    /**
     * @enum TokenType
     * @brief Represents the types of tokens in the programming language.
     */
    enum class TokenType : long {
        INTEGER,               ///< Integer literal
        DOUBLE,                ///< Double literal
        BOOLEAN,               ///< Boolean literal
        OPERATOR,              ///< Generic operator
        MINUS_OPERATOR,        ///< Minus operator
        EQUAL_OPERATOR,        ///< Equal operator
        DOT_OPERATOR,          ///< Dot operator
        IDENTIFIER,            ///< Identifier
        CHAR,                  ///< Character literal
        STRING,                ///< String literal
        K_MAIN,                ///< Keyword 'main'
        K_VAR,                 ///< Keyword 'var'
        K_STRUCTURE,           ///< Keyword 'structure'
        K_FOR,                 ///< Keyword 'for'
        K_FUN,                 ///< Keyword 'fun'
        K_RETURN,              ///< Keyword 'return'
        OPEN_PARENTESIS,       ///< Opening parenthesis '('
        OPEN_SQ_PARENTESIS,    ///< Opening square parenthesis '['
        OPEN_CUR_PARENTESIS,   ///< Opening curly parenthesis '{'
        CLOSE_PARENTESIS,      ///< Closing parenthesis ')'
        CLOSE_SQ_PARENTESIS,   ///< Closing square parenthesis ']'
        CLOSE_CUR_PARENTESIS,  ///< Closing curly parenthesis '}'
        OPERATION_EQUAL,       ///< Operation equal '='
        BOOLEAN_OPERATOR,      ///< Boolean operator
        NOT_OPERATOR,          ///< Not operator '!'
        LOGICAL_OPERATOR,      ///< Logical operator
        UNARY_OPERATOR,        ///< Unary operator
        COMMA,                 ///< Comma ','
        COLON,                 ///< Colon ':'
        COMMENT,               ///< Comment
        UNKNOWN,               ///< Unknown token type
        EOFT,                  ///< End of file token
        // Add more types as needed
    };

    static inline constexpr TokenType eofTokenType = TokenType::EOFT;
}  // namespace vnd
template <> struct fmt::formatter<vnd::TokenType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(vnd::TokenType tokenType, FormatContext &ctx) {
        std::string_view name;
        switch(tokenType) {
            using enum vnd::TokenType;
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
        case DOT_OPERATOR:
            name = "DOT_OPERATOR";
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
        case NOT_OPERATOR:
            name = "NOT_OPERATOR";
            break;
        case LOGICAL_OPERATOR:
            name = "LOGICAL_OPERATOR";
            break;
        case UNARY_OPERATOR:
            name = "UNARY_OPERATOR";
            break;
        case COMMA:
            name = "COMMA";
            break;
        case COLON:
            name = "COLON";
            break;
        case COMMENT:
            name = "COMMENT";
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

namespace vnd {

    /**
     * @brief Represents a token used in some lexical analysis.
     */
    class Token {
    public:
        /**
         * @brief Default constructor for Token.
         * Initializes the token with UNKNOWN type and default values for other members.
         */
        Token() noexcept : Token(TokenType::UNKNOWN, "", CodeSourceLocation::unknown()) {}  // NOLINT(*-redundant-member-init)

        /**
         * @brief Parameterized constructor for Token.
         * Initializes the token with the specified values.
         * @param type The type of the token.
         * @param value The value associated with the token.
         * @param sourceLocation The source location where the token appears.
         */
        Token(TokenType type, const std::string_view &value, const CodeSourceLocation &sourceLocation) noexcept
          : _type(type), _value(value), _sourceLocation{sourceLocation} {}

        /**
         * @brief Copy constructor for Token.
         * Initializes the token by copying values from another token.
         * @param other The token to copy.
         */
        Token(const Token &other) = default;

        /**
         * @brief Move constructor for Token.
         * Initializes the token by moving values from another token.
         * @param other The token to move.
         */
        Token(Token &&other) noexcept = default;

        /**
         * @brief Copy assignment operator for Token.
         * Copies values from another token.
         * @param other The token to copy.
         * @return Reference to the current token.
         */
        Token &operator=(const Token &other) = default;

        /**
         * @brief Move assignment operator for Token.
         * Moves values from another token.
         * @param other The token to move.
         * @return Reference to the current token.
         */
        Token &operator=(Token &&other) noexcept = default;

        /**
         * @brief Destructor for Token.
         */
        ~Token() = default;

        /**
         * @brief Get the type of the token.
         * @return The type of the token.
         */
        [[nodiscard]] inline TokenType getType() const noexcept { return _type; }

        /**
         * @brief Get the value associated with the token.
         * @return The value associated with the token.
         */
        [[nodiscard]] inline std::string_view getValue() const noexcept { return _value; }

        /**
         * @brief Get the file where the token appears.
         * @return The filename.
         */
        [[nodiscard]] inline std::string_view getFileName() const noexcept { return _sourceLocation.getFileName(); }

        /**
         * @brief Get the line number where the token appears.
         * @return The line number.
         */
        [[nodiscard]] inline std::size_t getLine() const noexcept { return _sourceLocation.getLine(); }

        /**
         * @brief Get the column number where the token appears.
         * @return The column number.
         */
        [[nodiscard]] inline std::size_t getColumn() const noexcept { return _sourceLocation.getColumn(); }

        /**
         * @brief Convert the token to a string representation.
         * @return A string representation of the token.
         */
        [[nodiscard]] std::string to_string() const;  // NOLINT(*-include-cleaner)

        /**
         * @brief Set the type of the token.
         * @param type The type to set.
         */
        inline void setType(TokenType type) noexcept { _type = type; }

        /**
         * @brief Set the value associated with the token.
         * @param value The value to set.
         */
        inline void setValue(const std::string_view &value) noexcept { _value = value; }

        /**
         * @brief Set the filename where the token appears.
         * @param fileName The filename to set.
         */
        void setFileName(const std::string_view &fileName) noexcept { _sourceLocation.setFileName(fileName); }

        /**
         * @brief Set the line number where the token appears.
         * @param line The line number to set.
         */
        void setLine(std::size_t line) noexcept { _sourceLocation.setLine(line); }

        /**
         * @brief Set the column number where the token appears.
         * @param column The column number to set.
         */
        void setColumn(std::size_t column) noexcept { _sourceLocation.setColumn(column); }

        /**
         * @brief Default comparison operator for Tokens.
         * @return True if the tokens are equal, false otherwise.
         */
        auto operator<=>(const Token &) const = default;

        /**
         * @brief Equality operator.
         * @param other The Token to compare with.
         * @return True if the Token are equal, false otherwise.
         */
        bool operator==(const Token &other) const noexcept = default;

        /**
         * @brief Inequality operator.
         * @param other The Token to compare with.
         * @return True if the Token are not equal, false otherwise.
         */
        bool operator!=(const Token &other) const noexcept = default;

    private:
        TokenType _type;                     ///< The type of the token.
        std::string_view _value;             ///< The value associated with the token.
        CodeSourceLocation _sourceLocation;  ///< the token source location;
    };

}  // namespace vnd

template <> struct fmt::formatter<vnd::Token> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const vnd::Token &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.to_string(), ctx);
    }
};
