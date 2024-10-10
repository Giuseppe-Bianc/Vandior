/*
 * Created by gbian on 16/09/2024.
 */

#pragma once

#include "NodeType.hpp"

DISABLE_WARNINGS_PUSH(4061)

[[nodiscard]] static inline const char *comp_NodeType(NodeType e) noexcept {
    switch(e) {
        using enum NodeType;
    case BinaryExpression:
        return "BIN_EXP";
    case UnaryExpression:
        return "UN_EXP";
    case Number:
        return "NUM";
    case Boolean:
        return "BOOL";
    case Char:
        return "CHAR";
    case String:
        return "STR";
    case Variable:
        return "VAR";
    case Nullptr:
        return "NULL";
    case Type:
        return "TYPE";
    case Index:
        return "IDX";
    case Array:
        return "ARR";
    default:
        return "UNKNOWN";
    }
}

DISABLE_WARNINGS_POP()
