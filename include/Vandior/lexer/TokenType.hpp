// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"

namespace vnd {

    /**
     * @enum TokenType
     * @brief Represents the types of tokens in the programming language.
     */
    enum class TokenType : long {
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

    static inline constexpr TokenType eofTokenType = TokenType::EOFT;
    using TokenTypeVec = std::vector<TokenType>;

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

// NOLINTEND(*-include-cleaner)