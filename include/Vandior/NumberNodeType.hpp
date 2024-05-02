//
// Created by gbian on 02/05/2024.
//

#pragma once

#include "headers.hpp"
/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NumberNodeType { Integer, Double };


/**
 * This function is a formatter for NodeType using fmt.
 * \cond
 */
/**
 * @brief Specialization of fmt::formatter for NodeType enumeration.
 */
template <> struct fmt::formatter<NumberNodeType> : fmt::formatter<std::string_view> {
    /**
     * @brief Formats the NodeType for printing.
     * @param nodeType The node type to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(NumberNodeType nodeType, FormatContext &ctx) {
        std::string_view name;
        switch(nodeType) {
        case NumberNodeType::Integer:
            name = "INTEGER";
            break;
        case NumberNodeType::Double:
            name = "DOUBLE";
            break;
        default:
            name = "UNKOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */
