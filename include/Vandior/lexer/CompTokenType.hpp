//
// Created by gbian on 07/05/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "TokenType.hpp"

namespace vnd {

    /**
     * @brief Converts a TokenType to its corresponding string representation.
     * This function provides a string representation for a given TokenType.
     * It can be useful for debugging, logging, or any situation where a human-readable
     * form of the token type is required.
     * @param type The TokenType to be converted to a string.
     * @return A std::string representing the token type.
     * @note The function is marked as [[nodiscard]], which means the compiler will issue
     * a warning if the return value is ignored.
     * Example usage:
     * @code
     * TokenType type = TokenType::PLUS;
     * std::string typeStr = vnd::comp_tokType(type);
     * std::cout << "Token type: " << typeStr << std::endl;  // Output: Token type: PLUS_OP
     * @endcode
     */
    [[nodiscard]] inline std::string comp_tokType(TokenType type) noexcept {
        using enum TokenType;
        switch(type) {
        case INTEGER:
            return "INT";
        case DOUBLE:
            return "DBL";
        case BOOLEAN:
            return "BOOL";
        case PLUS:
            return "PLUS_OP";
        case MINUS:
            return "MINUS_OP";
        case EQUAL:
            return "EQUAL_OP";
        case DOT:
            return "DOT_OP";
        case STAR:
            return "STAR_OP";
        case DIVIDE:
            return "DIVIDE_OP";
        case XOR:
            return "XOR_OP";
        case PERCENT:
            return "PERCENT_OP";
        case OR:
            return "OR_OP";
        case AND:
            return "AND_OP";
        case LESS:
            return "LESS_OP";
        case GREATER:
            return "GREATER_OP";
        case PLUSPLUS:
            return "PLUSPLUS_OP";
        case MINUSMINUS:
            return "MINUSMINUS_OP";
        case PLUSEQUAL:
            return "PLUSEQUAL_OP";
        case MINUSEQUAL:
            return "MINUSEQUAL_OP";
        case NOTEQUAL:
            return "NOTEQUAL_OP";
        case STAREQUAL:
            return "STAREQUAL_OP";
        case DIVIDEEQUAL:
            return "DIVIDEEQUAL_OP";
        case XOREQUAL:
            return "XOREQUAL_OP";
        case PERCENTEQUAL:
            return "PERCENTEQUAL_OP";
        case OROR:
            return "OROR_OP";
        case ANDAND:
            return "ANDAND_OP";
        case EQUALEQUAL:
            return "EQUALEQUAL_OP";
        case LESSEQUAL:
            return "LESSEQUAL_OP";
        case GREATEREQUAL:
            return "GREATEREQUAL_OP";
        case IDENTIFIER:
            return "IDENT";
        case CHAR:
            return "CH";
        case STRING:
            return "STR";
        case EOFT:
            return "EOF";
        case K_MAIN:
            return "K_MAIN";
        case K_VAR:
            return "K_VAR";
        case K_IF:
            return "K_IF";
        case K_WHILE:
            return "K_WHILE";
        case K_ELSE:
            return "K_ELSE";
        case K_FOR:
            return "K_FOR";
        case K_BREAK:
            return "BREAK";
        case K_FUN:
            return "K_FUN";
        case K_RETURN:
            return "K_RETURN";
        case K_NULLPTR:
            return "K_NULLPTR";
        case OPEN_PARENTESIS:
            return "OPEN_PAR";
        case OPEN_SQ_PARENTESIS:
            return "OPEN_SQ_PAR";
        case OPEN_CUR_PARENTESIS:
            return "OPEN_CUR_PAR";
        case CLOSE_PARENTESIS:
            return "CLOSE_PAR";
        case CLOSE_SQ_PARENTESIS:
            return "CLOSE_SQ_PAR";
        case CLOSE_CUR_PARENTESIS:
            return "CLOSE_CUR_PAR";
        case NOT:
            return "NOT_OP";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
        case TYPE_I8:
            return "I8";
        case TYPE_I16:
            return "I16";
        case TYPE_I32:
            return "I32";
        case TYPE_I64:
            return "I64";
        case TYPE_U8:
            return "U8";
        case TYPE_U16:
            return "U16";
        case TYPE_U32:
            return "U32";
        case TYPE_U64:
            return "U64";
        case TYPE_F32:
            return "F32";
        case TYPE_F64:
            return "F64";
        case TYPE_C32:
            return "C32";
        case TYPE_C64:
            return "C64";
        case TYPE_CHAR:
            return "CHAR";
        case TYPE_STRING:
            return "STRING";
        case TYPE_BOOL:
            return "BOOL";
        case COMMENT:
            return "COMMENT";
        // Add more cases as needed
        case UNKNOWN:
            [[fallthrough]];
        default:
            return "UNKNOWN";
        }
    }
}  // namespace vnd

/**
 * This function is a formatter for TokenType using fmt.
 * \cond
 */
template <> struct fmt::formatter<vnd::TokenType> : fmt::formatter<std::string_view> {
    // Add a flag to use the compact form or not
    bool use_compact = false;

    // Parse function to toggle compact format flag based on user input
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator {
        auto it = ctx.begin();
        auto end = ctx.end();

        if(it != end && *it == 'c') {  // 'c' for compact
            use_compact = true;
            ++it;
        }
        return it;
    }

    // Format function, conditionally using comp_tokType based on the flag
    auto format(vnd::TokenType tokenType, format_context &ctx) const -> format_context::iterator {
        std::string_view name;
        if(use_compact) {
            name = std::string_view(vnd::comp_tokType(tokenType));  // Use compact form
        } else {
            // Default case-based verbose representation
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
            case UNKNOWN:
                [[fallthrough]];
            default:
                name = "UNKNOWN";
                break;
            }
        }

        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};

/** \endcond */

// NOLINTEND(*-include-cleaner)