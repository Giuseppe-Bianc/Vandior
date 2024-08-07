//
// Created by gbian on 29/04/2024.
//

#pragma once
#include "../headers.hpp"
/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NodeType { BinaryExpression, UnaryExpression, Number, Boolean, Char, String, Variable, Type, Index, Array };

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
    auto format(NodeType nodeType, format_context &ctx) const -> format_context::iterator {
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
        case Boolean:
            name = "BOOLEAN";
            break;
        case Char:
            name = "CHAR";
            break;
        case String:
            name = "STRING";
            break;
        case Variable:
            name = "VARIABLE";
            break;
        case Type:
            name = "TYPE";
            break;
        case Index:
            name = "INDEX";
            break;
        case Array:
            name = "ARRAY";
            break;
        default:
            name = "UNKOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */
