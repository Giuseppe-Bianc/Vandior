//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "BinaryExpressionNode.hpp"
#include "Log.hpp"
#include "NumberNode.hpp"
#include "UnaryExpressionNode.hpp"
#include "VariableNode.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26445 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

/**
 * @brief Utility function for printing with indentation.
 * @param indent Number of spaces for indentation.
 * @param label Label for the printed information.
 * @param value Value to be printed.
 */
// NOLINTNEXTLINE
static inline constexpr void print_indent(int indent, const auto &label, const auto &value) {
    LINFO("{: ^{}}{}: {}", "", indent, label, value);
}
/**
 * @brief Utility function for printing with indentation, followed by a new line.
 * @param indent Number of spaces for indentation.
 * @param label Label for the printed information.
 * @param value Value to be printed.
 * @param labelnl Label for the new line.
 */
// NOLINTNEXTLINE
static inline constexpr void print_indent_dl(int indent, const auto &label, const auto &value, const auto &labelnl) {
    print_indent(indent, label, value);
    LINFO("{: ^{}}{}:", "", indent, labelnl);
}

/**
 * @brief Pretty prints the AST starting from the given node with optional indentation.
 * @param node The root of the AST to be pretty printed.
 * @param indent Number of spaces for indentation.
 */
// NOLINTNEXTLINE(misc-no-recursion)
static void prettyPrint(const vnd::ASTNode &node, int indent = 0) {
    // Recursively print children for Binary and Unary expression nodes
    if(const auto *binaryNode = node.as<vnd::BinaryExpressionNode>()) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), binaryNode->getOp()), "Left");
        prettyPrint(*binaryNode->getLeft(), indent + 2);
        print_indent(indent, "Right", "");
        prettyPrint(*binaryNode->getRight(), indent + 2);
    } else if(const auto *unaryNode = node.as<vnd::UnaryExpressionNode>()) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), unaryNode->getOp()), "Operand");
        prettyPrint(*unaryNode->getOperand(), indent + 2);
    } else if(const auto *numberNode = node.as<vnd::NumberNode>()) {
        print_indent(indent, "Node", FORMAT("(Type: {}, value:{})", node.getType(), numberNode->getValue()));
    } else if(const auto *variableNode = node.as<vnd::VariableNode>()) {
        print_indent(indent, "Node", FORMAT("(Type: {}, value:{})", node.getType(), variableNode->getName()));
    }
}

DISABLE_WARNINGS_POP()
