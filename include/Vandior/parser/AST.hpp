//
// Created by gbian on 01/01/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "BinaryExpressionNode.hpp"
#include "IndexNode.hpp"
#include "LiteralNode.hpp"
#include "NullptrNode.hpp"
#include "NumberNode.hpp"
#include "TypeNode.hpp"
#include "UnaryExpressionNode.hpp"
#include "VariableNode.hpp"

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
static inline constexpr auto PRETTYPRINT_AST_FORMAT = "{}, op:\"{}\")";
static inline constexpr auto PRETTYPRINT_AST_FORMAT2 = "{}, val: {})";
static inline constexpr auto PRETTYPRINT_AST_FORMAT3 = "{}_{}, val: {})";
static inline constexpr auto PRETTYPRINT_AST_NULLPTR = "{} NULLPTR)";

static inline void printParentNode(const vnd::ASTNode &node, const std::string &indentmark) {
    const auto *const nodeParent = node.get_parent();
    if(nodeParent != nullptr) {
        const auto nodeParentType = nodeParent->getType();
        const auto nodeParentTypeStr = comp_NodeType(nodeParentType);
        if(nodeParentType == NodeType::Index) {
            LINFO("{} Parent of Node ({})", indentmark, nodeParentTypeStr);
        } else {
            LINFO("{} Parent of Node ({}){}", indentmark, nodeParentTypeStr, nodeParent->get_token().compat_to_string());
        }
    } else {
        LINFO("{} Node ({}) has no parent", indentmark, comp_NodeType(node.getType()));
    }
}
// NOLINTNEXTLINE(misc-no-recursion, *-function-cognitive-complexity)
static inline void prettyPrint(const vnd::ASTNode &node, const std::string &indent = "", bool isLast = true, const std::string &lorf = "") {
    const auto &indentmark = FORMAT("{}{}{}", indent, isLast ? "+-" : "|-", lorf);
    const auto &newindent = FORMAT("{}{}", indent, isLast ? "  " : "| ");
    const auto &imarknode = FORMAT("{}({}", indentmark, comp_NodeType(node.getType()));

    // printParentNode(node, indentmark);
    //  Determine the type of node and print information
    if(const auto *binaryNode = node.safe_as<vnd::BinaryExpressionNode>()) {
        LINFO(PRETTYPRINT_AST_FORMAT, imarknode, binaryNode->getOp());
        prettyPrint(*binaryNode->getLeft(), newindent, false, "L");
        prettyPrint(*binaryNode->getRight(), newindent, true, "R");
    } else if(const auto *unaryNode = node.safe_as<vnd::UnaryExpressionNode>()) {
        LINFO(PRETTYPRINT_AST_FORMAT, imarknode, unaryNode->getOp());
        prettyPrint(*unaryNode->getOperand(), newindent, true, "OPR");
    } else if(const auto *variableNode = node.safe_as<vnd::VariableNode>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, variableNode->getName());
        if(variableNode->is_call()) {
            if(const auto &callNode = variableNode->get_call()) { prettyPrint(*callNode, newindent, true, "CALL"); }
        }
        if(const auto &indexNode = variableNode->get_index()) { prettyPrint(*indexNode, newindent, true, "INDEX"); }
    } else if(const auto *intnumberNode = node.safe_as<VND_NUM_INT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(intnumberNode->getNumberType()), intnumberNode->get_value());
    } else if(const auto *cflnumberNode = node.safe_as<VND_NUM_CFLOAT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(cflnumberNode->getNumberType()), cflnumberNode->get_value());
    } else if(const auto *dflnumberNode = node.safe_as<VND_NUM_CDOUBLE>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(dflnumberNode->getNumberType()), dflnumberNode->get_value());
    } else if(const auto *flnumberNode = node.safe_as<VND_NUM_FLOAT>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(flnumberNode->getNumberType()), flnumberNode->get_value());
    } else if(const auto *dblnumberNode = node.safe_as<VND_NUM_DOUBLE>()) {
        LINFO(PRETTYPRINT_AST_FORMAT3, imarknode, NumNodeType_comp(dblnumberNode->getNumberType()), dblnumberNode->get_value());
    } else if(const auto *blliteralNode = node.safe_as<vnd::LiteralNode<bool>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, blliteralNode->get_value());
    } else if(const auto *chliteralNode = node.safe_as<vnd::LiteralNode<char>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, chliteralNode->get_value());
    } else if(const auto *svliteralNode = node.safe_as<vnd::LiteralNode<std::string_view>>()) {
        LINFO(PRETTYPRINT_AST_FORMAT2, imarknode, svliteralNode->get_value());
    } else if([[maybe_unused]] const auto *nullptrNode = node.safe_as<vnd::NullptrNode>()) {
        LINFO(PRETTYPRINT_AST_NULLPTR, imarknode);
    } else if(const auto *typeNode = node.safe_as<vnd::TypeNode>()) {
        LINFO("{}, {})", imarknode, typeNode->get_value());
        if(typeNode->get_index()) { prettyPrint(*typeNode->get_index(), newindent, true, "INDEX"); }
    } else if(const auto *indexNode = node.safe_as<vnd::IndexNode>()) {
        LINFO("{}", indentmark, node.comp_print());
        const auto &elementsNode = indexNode->get_elements();
        const auto &elementsIndexNode = indexNode->get_index();
        const auto &elementsArrayNode = indexNode->get_array();
        if(elementsNode) { prettyPrint(*elementsNode, newindent, true, ""); }
        if(elementsIndexNode) { prettyPrint(*elementsIndexNode, newindent, true, "INDEX"); }
        if(elementsArrayNode) { prettyPrint(*elementsArrayNode, newindent, true, "ELEM"); }
    } else if(const auto *arrayNode = node.safe_as<vnd::ArrayNode>()) {
        LINFO("{} {}", indentmark, node.comp_print());
        if(arrayNode->get_elements()) { prettyPrint(*arrayNode->get_elements(), newindent, true, ""); }
    } else {
        LERROR("Unknown or not handled node type: {}", node.getType());
    }
}
/** \cond */
DISABLE_WARNINGS_POP()
/** \endcond */

// NOLINTEND(*-include-cleaner)
