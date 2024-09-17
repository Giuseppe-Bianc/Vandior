/*
 * Created by gbian on 16/09/2024.
 * Copyright (c) 2024 All rights reserved.
 */

#pragma once

#include "NodeType.hpp"

[[nodiscard]] static inline const char *comp_NodeType(NodeType e) noexcept {
    switch(e) {
    case NodeType::BinaryExpression:
        return "BIN_EXP";
    case NodeType::UnaryExpression:
        return "UN_EXP";
    case NodeType::Number:
        return "NUM";
    case NodeType::Boolean:
        return "BOOL";
    case NodeType::Char:
        return "CHAR";
    case NodeType::String:
        return "STR";
    case NodeType::Variable:
        return "VAR";
    case NodeType::Type:
        return "TYPE";
    case NodeType::Index:
        return "IDX";
    case NodeType::Array:
        return "ARR";
    default:
        return "UNKNOWN";
    }
}