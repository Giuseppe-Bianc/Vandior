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

// NOLINTEND(*-include-cleaner)