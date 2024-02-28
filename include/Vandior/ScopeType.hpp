#pragma once
#include "headers.hpp"

namespace vnd {
    /**
     * @enum ScopeType
     * @brief Represents the types of scopes in the programming language.
     */
    enum class ScopeType : short {
        GLOBAL_SCOPE,           ///< The global scope of the program.
        MAIN_SCOPE,             ///< The scope of the main function.
        FUNCTION_SCOPE,         ///< A function definition scope.
        IF_SCOPE,               ///< An if scope.
        ELSE_SCOPE,             ///< An else scope.
        LOOP_SCOPE,             ///< A while or for scope.
        SCOPE                   ///< A scope defined with a curly bracket.
    };
}  // namespace vnd

template <> struct fmt::formatter<vnd::ScopeType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(vnd::ScopeType scopeType, FormatContext &ctx) {
        std::string_view name;
        switch(scopeType) {
            using enum vnd::ScopeType;
        case GLOBAL_SCOPE:
            name = "GLOBAL_SCOPE";
            break;
        case MAIN_SCOPE:
            name = "MAIN_SCOPE";
            break;
        case FUNCTION_SCOPE:
            name = "FUNCTION_SCOPE";
            break;
        case IF_SCOPE:
            name = "IF_SCOPE";
            break;
        case ELSE_SCOPE:
            name = "ELSE_SCOPE";
            break;
        case LOOP_SCOPE:
            name = "LOOP_SCOPE";
            break;
        case SCOPE:
            [[fallthrough]];
        default:
            name = "SCOPE";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};