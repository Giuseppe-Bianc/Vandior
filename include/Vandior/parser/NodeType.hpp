//
// Created by gbian on 29/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"
/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NodeType { BinaryExpression, UnaryExpression, Number, Boolean, Char, String, Variable, Nullptr, Type, Index, Array, Statement };

// NOLINTEND(*-include-cleaner)