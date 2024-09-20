/*
 * Created by gbian on 16/09/2024.
 * 
 */

#pragma once

#include "NodeType.hpp"

[[nodiscard]] static inline const char *comp_NodeType(NodeType e) noexcept {
    using enum NodeType;
    switch(e) {
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