//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "../Log.hpp"
#include "BinaryExpressionNode.hpp"
#include "LiteralNode.hpp"
#include "NumberNode.hpp"
#include "UnaryExpressionNode.hpp"
#include "VariableNode.hpp"
#include "TypeNode.hpp"
#include "IndexNode.hpp"

/** \cond */
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
static inline void prettyPrint(const vnd::ASTNode &node, const std::string &indent = "", bool isLast = true, const std::string &lorf = "") {
    const auto &indentmark = FORMAT("{}{}{}{}", indent, isLast ? "+-" : "|-", lorf, lorf.empty() ? "" : " ");
    const auto &newindent = FORMAT("{}{}", indent, isLast ? "  " : "| ");
    const auto &imarknode = FORMAT("{}(Type: {}", indentmark, node.getType());
    const auto &imarknnum = FORMAT("{}(Type: NUM", indentmark);

    // Determine the type of node and print information
    if(const auto *binaryNode = node.safe_as<vnd::BinaryExpressionNode>()) {
        LINFO("{}(Type: BIN_EXPR, op:\"{}\"){}", indentmark, binaryNode->getOp(), node.get_token().compat_to_string());
        prettyPrint(*binaryNode->getLeft(), newindent, false, "LEFT");
        prettyPrint(*binaryNode->getRight(), newindent, true, "RIGHT");
    } else if(const auto *unaryNode = node.safe_as<vnd::UnaryExpressionNode>()) {
        LINFO("{}(Type: UNA_EXPR, op:\"{}\"){}", indentmark, unaryNode->getOp(), node.get_token().compat_to_string());
        prettyPrint(*unaryNode->getOperand(), newindent, true, "OPERAND");
    } else if(const auto *variableNode = node.safe_as<vnd::VariableNode>()) {
        LINFO("{}(Type: VAR, val: {}){}", indentmark, variableNode->getName(), node.get_token().compat_to_string());
    } else if(const auto *intnumberNode = node.safe_as<vnd::NumberNode<int>>()) {
        LINFO("{}_{}, val: {}){}", imarknnum, NumNodeType_comp(intnumberNode->getNumberType()), intnumberNode->get_value(),
              node.get_token().compat_to_string());
    } else if(const auto *cflnumberNode = node.safe_as<vnd::NumberNode<std::complex<float>>>()) {
        LINFO("{}_{}, val: {}){}", imarknnum, NumNodeType_comp(cflnumberNode->getNumberType()), cflnumberNode->get_value(),
              node.get_token().compat_to_string());
    } else if(const auto *dflnumberNode = node.safe_as<vnd::NumberNode<std::complex<double>>>()) {
        LINFO("{}_{}, val: {}){}", imarknnum, NumNodeType_comp(dflnumberNode->getNumberType()), dflnumberNode->get_value(),
              node.get_token().compat_to_string());
    } else if(const auto *flnumberNode = node.safe_as<vnd::NumberNode<float>>()) {
        LINFO("{}_{}, val: {}){}", imarknnum, NumNodeType_comp(flnumberNode->getNumberType()), flnumberNode->get_value(),
              node.get_token().compat_to_string());
    } else if(const auto *dblnumberNode = node.safe_as<vnd::NumberNode<double>>()) {
        LINFO("{}_{}, val: {}){}", imarknnum, NumNodeType_comp(dblnumberNode->getNumberType()), dblnumberNode->get_value(),
              node.get_token().compat_to_string());
    } else if(const auto *blliteralNode = node.safe_as<vnd::LiteralNode<bool>>()) {
        LINFO("{}, val: {}){}", imarknode, blliteralNode->get_value(), node.get_token().compat_to_string());
    } else if(const auto *chliteralNode = node.safe_as<vnd::LiteralNode<char>>()) {
        LINFO("{}, val: {}){}", imarknode, chliteralNode->get_value(), node.get_token().compat_to_string());
    } else if(const auto *svliteralNode = node.safe_as<vnd::LiteralNode<std::string_view>>()) {
        LINFO("{}, val: {}){}", imarknode, svliteralNode->get_value(), node.get_token().compat_to_string());
    } else if(const auto *typeNode = node.safe_as<vnd::TypeNode>()) {
        LINFO("{}, type: {}){}", imarknode, typeNode->get_value(), node.get_token().compat_to_string());
        if(typeNode->get_index()) { prettyPrint(*typeNode->get_index(), newindent, true, "INDEX"); }
    } else if(const auto *indexNode = node.safe_as<vnd::IndexNode>()) {
        LINFO("{}", indentmark, node.comp_print());
        if(indexNode->get_elements()) { prettyPrint(*indexNode->get_elements(), newindent, true, ""); }
    } else {
        LERROR("Unknown or not handled node type: {}", node.getType());
    }
}
/** \cond */
DISABLE_WARNINGS_POP()
/** \endcond */
