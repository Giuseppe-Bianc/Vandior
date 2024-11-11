// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"

namespace vnd {

    /**
     * @enum TokenType
     * @brief Represents the types of tokens in the programming language.
     */
    enum class TokenType : std::uint8_t {
        INTEGER,               ///< Integer literal
        DOUBLE,                ///< Double literal
        BOOLEAN,               ///< Boolean literal
        PLUS,                  ///< Plus oprator
        MINUS,                 ///< Minus operator
        NOT,                   ///< Not operator '!'
        STAR,                  ///< Star operator
        DIVIDE,                ///< Divide operator
        XOR,                   ///< Xor operator
        PERCENT,               ///< Percent operator
        OR,                    ///< Or operator
        AND,                   ///< And operator
        EQUAL,                 ///< Equal operator
        LESS,                  ///< Less operator
        GREATER,               ///< Greater operator
        PLUSPLUS,              ///< Plus Plus oprator
        MINUSMINUS,            ///< Minus Minus operator
        PLUSEQUAL,             ///< Plus Equal oprator
        MINUSEQUAL,            ///< Minus Equal oprator
        NOTEQUAL,              ///< Not Equal operator '!='
        STAREQUAL,             ///< Star  Equal operator
        DIVIDEEQUAL,           ///< Divide  Equal operator
        XOREQUAL,              ///< Xor Equal operator
        PERCENTEQUAL,          ///< Percent  Equal operator
        OROR,                  ///< Or operator
        ANDAND,                ///< And operator
        EQUALEQUAL,            ///< Equal Equal operator
        LESSEQUAL,             ///< Less Equal operator
        GREATEREQUAL,          ///< Greater Equal operator
        DOT,                   ///< Dot operator
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
        COMMA,                 ///< Comma ','
        COLON,                 ///< Colon ':'
        TYPE_I8,               ///< i8
        TYPE_I16,              ///< i16 token
        TYPE_I32,              ///< i32 token
        TYPE_I64,              ///< i64 token
        TYPE_U8,               ///< u8 token
        TYPE_U16,              ///< u16 token
        TYPE_U32,              ///< u32 token
        TYPE_U64,              ///< u64 token
        TYPE_F32,              ///< f32 token
        TYPE_F64,              ///< f64 token
        TYPE_C32,              ///< c32 token
        TYPE_C64,              ///< c64 token
        TYPE_CHAR,             ///< char token
        TYPE_STRING,           ///< string token
        TYPE_BOOL,             ///< bool token
        COMMENT,               ///< Comment
        UNKNOWN,               ///< Unknown token type
        EOFT,                  ///< End of file token
        // Add more types as needed
    };

    static inline constexpr auto eofTokenType = TokenType::EOFT;
    static inline constexpr std::array<TokenType, 9> keywordsTokenTypes = {
        TokenType::K_MAIN, TokenType::K_VAR,   TokenType::K_IF,  TokenType::K_WHILE,  TokenType::K_ELSE,
        TokenType::K_FOR,  TokenType::K_BREAK, TokenType::K_FUN, TokenType::K_RETURN,
    };
    static inline constexpr std::array<TokenType, 6> structuresTokenTypes = {TokenType::K_MAIN, TokenType::K_IF,  TokenType::K_WHILE,
                                                                             TokenType::K_ELSE, TokenType::K_FOR, TokenType::K_FUN};
    using TokenTypeVec = std::vector<TokenType>;

    /**
     * @brief Checks if a tokenType is a keyword.
     * @param type the TokenType to check.
     * @return A pair containing a bool flag indicating if the token is a keyword and another flag indicating if it's a structure.
     */
    [[nodiscard]] static constexpr std::pair<bool, bool> checkKeyword(const TokenType &type) noexcept {
        return std::make_pair(std::ranges::find(keywordsTokenTypes, type) != std::end(keywordsTokenTypes),
                              std::ranges::find(structuresTokenTypes, type) != std::end(structuresTokenTypes));
    }

    /**
     * @brief Gets the TokenType of a value.
     * @param value The value.
     * @return TokenType of the value.
     */
    [[nodiscard]] static constexpr TokenType getBracketsType(const std::string_view &value) noexcept {
        using enum TokenType;
        switch(value.front()) {
        case '(':
            return OPEN_PARENTESIS;
        case ')':
            return CLOSE_PARENTESIS;
        case '[':
            return OPEN_SQ_PARENTESIS;
        case ']':
            return CLOSE_SQ_PARENTESIS;
        case '{':
            return OPEN_CUR_PARENTESIS;
        case '}':
            return CLOSE_CUR_PARENTESIS;
        default:
            return UNKNOWN;
        }
    }

    /**
     * @brief Determines the token type of single character operators.
     * @param view The character.
     * @return TokenType of the character.
     */
    [[nodiscard]] static constexpr TokenType singoleCharOp(const char view) noexcept {
        switch(view) {
            using enum TokenType;
        case minuscs:
            return MINUS;
        case '=':
            return EQUAL;
        case '<':
            return LESS;
        case '>':
            return GREATER;
        case '!':
            return NOT;
        case plusscr:
            return PLUS;
        case '*':
            return STAR;
        case '/':
            return DIVIDE;
        case '^':
            return XOR;
        case '%':
            return PERCENT;
        case '|':
            return OR;
        case '&':
            return AND;
        default:
            return UNKNOWN;
        }
    }
}  // namespace vnd

/**
 * This function is a formatter for TokenType using fmt.
 * \cond
 */
template <> struct fmt::formatter<vnd::TokenType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    auto format(vnd::TokenType tokenType, format_context &ctx) const -> format_context::iterator {
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
        case PLUS:
            name = "PLUS_OPERATOR";
            break;
        case MINUS:
            name = "MINUS_OPERATOR";
            break;
        case EQUAL:
            name = "EQUAL_OPERATOR";
            break;
        case DOT:
            name = "DOT_OPERATOR";
            break;
        case STAR:
            name = "STAR_OPERATOR";
            break;
        case DIVIDE:
            name = "DIVIDE_OPERATOR";
            break;
        case XOR:
            name = "XOR_OPERATOR";
            break;
        case PERCENT:
            name = "PERCENT_OPERATOR";
            break;
        case OR:
            name = "OR_OPERATOR";
            break;
        case AND:
            name = "AND_OPERATOR";
            break;
        case LESS:
            name = "LESS_OPERATOR";
            break;
        case GREATER:
            name = "GREATER_OPERATOR";
            break;
        case PLUSPLUS:
            name = "PLUSPLUS_OPERATOR";
            break;
        case MINUSMINUS:
            name = "MINUSMINUS_OPERATOR";
            break;
        case PLUSEQUAL:
            name = "PLUSEQUAL_OPERATOR";
            break;
        case MINUSEQUAL:
            name = "MINUSEQUAL_OPERATOR";
            break;
        case NOTEQUAL:
            name = "NOTEQUAL_OPERATOR";
            break;
        case STAREQUAL:
            name = "STAREQUAL_OPERATOR";
            break;
        case DIVIDEEQUAL:
            name = "DIVIDEEQUAL_OPERATOR";
            break;
        case XOREQUAL:
            name = "XOREQUAL_OPERATOR";
            break;
        case PERCENTEQUAL:
            name = "PERCENTEQUAL_OPERATOR";
            break;
        case OROR:
            name = "OROR_OPERATOR";
            break;
        case ANDAND:
            name = "ANDAND_OPERATOR";
            break;
        case EQUALEQUAL:
            name = "EQUALEQUAL_OPERATOR";
            break;
        case LESSEQUAL:
            name = "LESSEQUAL_OPERATOR";
            break;
        case GREATEREQUAL:
            name = "GREATEREQUAL_OPERATOR";
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
        case NOT:
            name = "NOT_OPERATOR";
            break;
        case COMMA:
            name = "COMMA";
            break;
        case COLON:
            name = "COLON";
            break;
        case TYPE_I8:
            name = "TYPE_I8";
            break;
        case TYPE_I16:
            name = "TYPE_I16";
            break;
        case TYPE_I32:
            name = "TYPE_I32";
            break;
        case TYPE_I64:
            name = "TYPE_I64";
            break;
        case TYPE_U8:
            name = "TYPE_U8";
            break;
        case TYPE_U16:
            name = "TYPE_U16";
            break;
        case TYPE_U32:
            name = "TYPE_U32";
            break;
        case TYPE_U64:
            name = "TYPE_U64";
            break;
        case TYPE_F32:
            name = "TYPE_F32";
            break;
        case TYPE_F64:
            name = "TYPE_F64";
            break;
        case TYPE_C32:
            name = "TYPE_C32";
            break;
        case TYPE_C64:
            name = "TYPE_C64";
            break;
        case TYPE_CHAR:
            name = "TYPE_CHAR";
            break;
        case TYPE_STRING:
            name = "TYPE_STRING";
            break;
        case TYPE_BOOL:
            name = "TYPE_BOOL";
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

// NOLINTEND(*-include-cleaner)