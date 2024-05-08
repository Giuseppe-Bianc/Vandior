//
// Created by gbian on 07/05/2024.
//

#pragma once

#include "TokenType.hpp"

namespace vnd {
    inline std::string comp_tokType_to_string(TokenType type) {
        using enum TokenType;
        switch(type) {
            using enum TokenType;
        case INTEGER:
            return "INT";
        case DOUBLE:
            return "DBL";
        case BOOLEAN:
            return "BOOL";
        case OPERATOR:
            return "OPER";
        case MINUS_OPERATOR:
            return "MINUS_OP";
        case EQUAL_OPERATOR:
            return "EQUAL_OP";
        case DOT_OPERATOR:
            return "DOT_OP";
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
        case OPERATION_EQUAL:
            return "OP_EQUAL";
        case BOOLEAN_OPERATOR:
            return "BOOLEAN_OP";
        case NOT_OPERATOR:
            return "NOT_OP";
        case LOGICAL_OPERATOR:
            return "LOGICAL_OP";
        case UNARY_OPERATOR:
            return "UNARY_OP";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
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
