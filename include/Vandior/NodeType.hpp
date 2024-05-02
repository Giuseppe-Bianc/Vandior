//
// Created by gbian on 29/04/2024.
//

#pragma once
#include "headers.hpp"
/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NodeType { BinaryExpression, UnaryExpression, Number, Variable };

/**
 * This function is a formatter for NodeType using fmt.
 * \cond
 */
/**
 * @brief Specialization of fmt::formatter for NodeType enumeration.
 */
template <> struct fmt::formatter<NodeType> : fmt::formatter<std::string_view> {
    /**
     * @brief Formats the NodeType for printing.
     * @param nodeType The node type to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(NodeType nodeType, FormatContext &ctx) {
        using enum NodeType;
        std::string_view name;
        switch(nodeType) {
        case BinaryExpression:
            name = "BINARY_EXPRESION";
            break;
        case UnaryExpression:
            name = "UNARY_EXPRESION";
            break;
        case Number:
            name = "NUMBER";
            break;
        case Variable:
            name = "VARIABLE";
            break;
        default:
            name = "UNKOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */
