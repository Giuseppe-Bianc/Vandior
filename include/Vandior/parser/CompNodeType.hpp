/*
 * Created by gbian on 16/09/2024.
 */

#pragma once

#include "NodeType.hpp"

DISABLE_WARNINGS_PUSH(4061)

[[nodiscard]] static inline const char *comp_NodeType(NodeType e) noexcept {
    switch(e) {
        using enum NodeType;
    case BinaryExpression:
        return "BIN_EXP";
    case UnaryExpression:
        return "UN_EXP";
    case Number:
        return "NUM";
    case Boolean:
        return "BOOL";
    case Char:
        return "CHAR";
    case String:
        return "STR";
    case Variable:
        return "VAR";
    case Nullptr:
        return "NULL";
    case Type:
        return "TYPE";
    case Index:
        return "IDX";
    case Array:
        return "ARR";
    default:
        return "UNKNOWN";
    }
}

/**
 * This function is a formatter for NodeType using fmt.
 * \cond
 */
/**
 * @brief Specialization of fmt::formatter for NodeType enumeration.
 */
template <> struct fmt::formatter<NodeType> : fmt::formatter<std::string_view> {
    // Add a flag to use the compact form or not
    bool use_compact = false;

    // Parse function to toggle compact format flag based on user input
    constexpr auto parse(format_parse_context &ctx) -> format_parse_context::iterator {
        auto it = ctx.begin();
        auto end = ctx.end();

        if(it != end && *it == 'c') {  // 'c' for compact
            use_compact = true;
            ++it;
        }
        return it;
    }

    /**
     * @brief Formats the NodeType for printing.
     * @param nodeType The node type to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    auto format(NodeType nodeType, format_context &ctx) const -> format_context::iterator {
        using enum NodeType;
        std::string_view name;
        if(use_compact) { name = std::string_view(comp_NodeType(nodeType)); }
        else {
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
            case Nullptr:
                name = "NULLPTR";
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
            case Statement:
                name = "STATEMENT";
                break;
            default:
                name = "UNKOWN";
                break;
            }
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */

DISABLE_WARNINGS_POP()
