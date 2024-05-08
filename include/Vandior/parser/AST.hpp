//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "../Log.hpp"
#include "BinaryExpressionNode.hpp"
#include "DoubleNumberNode.hpp"
#include "IntegerNumberNode.hpp"
#include "UnaryExpressionNode.hpp"
#include "VariableNode.hpp"

/**
 * This macro disable some msvc warnigs.
 * \cond
 */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26445 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)
/** \endcond */

/**
 * @brief Recursively prints the structure of an AST (Abstract Syntax Tree).
 *
 * This function is designed to traverse and print the structure of an AST, represented by instances
 * of the `vnd::ASTNode` class and its derived classes. The printed structure provides information
 * about the type of each node, any associated operators or values, and the position of each node in
 * the tree (e.g., left or right subtree).
 *
 * @param node The root node of the AST or subtree to be printed.
 * @param indent string specifying the indentation for each level of the tree. This argument
 *               is used internally during recursion to maintain the visual hierarchy.
 *               Defaults to an empty string.
 * @param isLast An optional Boolean value indicating whether the current node is the last node in
 *               its subtree. Used internally during recursion to determine the formatting.
 *               Defaults to `true`.
 * @param lorf An optional string specifying whether the node is in the left or right subtree of its
 *             parent node. It can take the values "LEFT" or "RIGHT". Defaults to an empty string.
 *
 * @note The function uses a series of conditional statements to determine the type of the AST node
 * and prints the appropriate information depending on the node type. It also uses recursion to
 * traverse the entire AST structure.
 */
// NOLINTNEXTLINE(misc-no-recursion)
static void prettyPrint(const vnd::ASTNode &node, const std::string &indent = "", bool isLast = true, const std::string &lorf = "") {
    const auto &indentmark = FORMAT("{}{}{}{}", indent, isLast ? "+-" : "|-", lorf, lorf.empty() ? "" : " ");
    const auto &newindent = FORMAT("{}{}", indent, isLast ? "  " : "| ");

    // Determine the type of node and print information
    if(const auto *binaryNode = node.as<vnd::BinaryExpressionNode>()) {
        LINFO("{}(Type: BIN_EXPR, op:\"{}\"){}", indentmark, binaryNode->getOp(), node.get_token().compat_to_string());
        prettyPrint(*binaryNode->getLeft(), newindent, false, "LEFT");
        prettyPrint(*binaryNode->getRight(), newindent, true, "RIGHT");
    } else if(const auto *unaryNode = node.as<vnd::UnaryExpressionNode>()) {
        LINFO("{}(Type: UNA_EXPR, op:\"{}\"){}", indentmark, unaryNode->getOp(), node.get_token().compat_to_string());
        prettyPrint(*unaryNode->getOperand(), newindent, true, "OPERAND");
    } else if(const auto *integerNumberNode = node.as<vnd::IntegerNumberNode>()) {
        LINFO("{}(Type: NUM, NumType: {}, val:{}){}", indentmark, NumNodeType_comp_to_string(integerNumberNode->getNumberType()),
              integerNumberNode->get_value(), node.get_token().compat_to_string());
    } else if(const auto *doubleNumberNode = node.as<vnd::DoubleNumberNode>()) {
        LINFO("{}(Type: NUM, NumType: {}, val:{}){}", indentmark, NumNodeType_comp_to_string(doubleNumberNode->getNumberType()),
              doubleNumberNode->get_value(), node.get_token().compat_to_string());
    } else if(const auto *variableNode = node.as<vnd::VariableNode>()) {
        LINFO("{}(Type: VAR, val:{}){}", indentmark, variableNode->getName(), node.get_token().compat_to_string());
    }
}
/**
 * This macro disable some msvc warnigs.
 * \cond
 */
DISABLE_WARNINGS_POP()
/** \endcond */