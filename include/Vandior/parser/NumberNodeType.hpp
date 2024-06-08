//
// Created by gbian on 02/05/2024.
//

#pragma once

#include "../headers.hpp"
/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NumberNodeType { Integer, Float, Double, ImaginaryFloat, Imaginary };
[[nodiscard]] inline std::string NumNodeType_comp(NumberNodeType type) {
    using enum NumberNodeType;
    switch(type) {
    case Integer:
        return "INT";
    case Float:
        return "FLT";
    case Double:
        return "DBL";
    case ImaginaryFloat:
        return "IMF";
    case Imaginary:
        return "IMG";
    default:
        return "UNKOWN";
    }
}
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
        using enum NumberNodeType;
        std::string_view name;
        switch(nodeType) {
        case Integer:
            name = "INTEGER";
            break;
        case Float:
            name = "FLOAT";
            break;
        case Double:
            name = "DOUBLE";
            break;
        case ImaginaryFloat:
            name = "IMAGINARY_F";
            break;
        case Imaginary:
            name = "IMAGINARY";
            break;
        default:
            name = "UNKOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */
