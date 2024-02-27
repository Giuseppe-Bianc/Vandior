#pragma once
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
        K_IF,                  ///< Keyword 'if'
        K_WHILE,               ///< Keyword 'while'
        K_ELSE,                ///< Keywork 'else'
        K_FOR,                 ///< Keyword 'for'
        K_BREAK,               ///< Keyword 'break'
        K_FUN,                 ///< Keyword 'fun'
        K_RETURN,              ///< Keyword 'return'
        K_NULLPTR,             ///< Keyword 'nullptr'
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

/**
 * This function is a formatter for TokenType using fmt.
 * \cond
 */
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
        case K_IF:
            name = "K_IF";
            break;
        case K_WHILE:
            name = "K_WHILE";
            break;
        case K_ELSE:
            name = "K_ELSE";
            break;
        case K_FOR:
            name = "K_FOR";
            break;
        case K_BREAK:
            name = "BREAK";
            break;
        case K_FUN:
            name = "K_FUN";
            break;
        case K_RETURN:
            name = "K_RETURN";
            break;
        case K_NULLPTR:
            name = "K_NULLPTR";
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

/** \endcond */
