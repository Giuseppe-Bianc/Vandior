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
static inline constexpr auto PRETTYPRINT_AST_FORMAT = "{}, op:\"{}\")";
static inline constexpr auto PRETTYPRINT_AST_FORMAT2 = "{}, val: {})";
static inline constexpr auto PRETTYPRINT_AST_FORMAT3 = "{}_{}, val: {})";
static inline constexpr auto PRETTYPRINT_AST_NULLPTR = "{} NULLPTR)";

/**
 * @brief Prints the parent node of the AST.
 *
 * @param node The AST node to be printed.
 * @param indentmark The indentation mark for formatting.
 */
void printParentNode(const vnd::ASTNode &node, const std::string &indentmark);

/**
 * @brief Prints a binary expression node of the AST.
 *
 * @param node The binary expression node to be printed.
 * @param imarknode The indentation mark for the node.
 * @param newindent The new indentation for the next level.
 */
void printBinaryNode(const vnd::BinaryExpressionNode &node, const std::string &imarknode, const std::string &newindent);

/**
 * @brief Prints a unary expression node of the AST.
 *
 * @param node The unary expression node to be printed.
 * @param imarknode The indentation mark for the node.
 * @param newindent The new indentation for the next level.
 */
void printUnaryNode(const vnd::UnaryExpressionNode &node, const std::string &imarknode, const std::string &newindent);

/**
 * @brief Prints a variable node of the AST.
 *
 * @param node The variable node to be printed.
 * @param imarknode The indentation mark for the node.
 * @param newindent The new indentation for the next level.
 */
void printVariableNode(const vnd::VariableNode &node, const std::string &imarknode, const std::string &newindent);

/**
 * @brief Prints an index node of the AST.
 *
 * @param node The index node to be printed.
 * @param indentmark The indentation mark for formatting.
 * @param newindent The new indentation for the next level.
 */
void printIndexNode(const vnd::IndexNode &node, const std::string &indentmark, const std::string &newindent);

/**
 * @brief Prints an array node of the AST.
 *
 * @param node The array node to be printed.
 * @param indentmark The indentation mark for formatting.
 * @param newindent The new indentation for the next level.
 */
void printArrayNode(const vnd::ArrayNode &node, const std::string &indentmark, const std::string &newindent);

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
void prettyPrint(const vnd::ASTNode &node, const std::string &indent = "", bool isLast = true, const std::string &lorf = "");

/** \cond */
DISABLE_WARNINGS_POP()
/** \endcond */

// NOLINTEND(*-include-cleaner)
