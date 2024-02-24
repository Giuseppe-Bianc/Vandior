#pragma once
#include "headers.hpp"

namespace vnd {
    /**
     * @enum InstructionType
     * @brief Represents the types of instruction in the programming language.
     */
    enum class InstructionType : short {
        PARAMETER_EXPRESSION,   ///< An epxression used as function parameter.
        OPERATION,              ///< An operation (call to function or use of ++ or --).
        ASSIGNATION,            ///< An assignation.
        EXPRESSION,             ///< An expression.
        SQUARE_EXPRESSION,      ///< An expression used as array size or index.
        RETURN_EXPRESSION,      ///< An expression used as return value of a function.
        DECLARATION,            ///< A declaration of variables.
        INITIALIZATION,         ///< A declaration with the initialization.
        ARRAY_INIZIALIZATION,   ///< An array literals.
        MAIN,                   ///< The main instruction.
        STRUCTURE,              ///< A if or while structure.
        ELSE,                   ///< An else structure.
        FOR_STRUCTURE,          ///< A for structure.
        BREAK,                  ///< A break or continue instruction.
        FOR_INITIALIZATION,     ///< The initialization part of a for.
        FOR_CONDITION,          ///< The condition part of a for.
        FOR_STEP,               ///< The step part of a for.
        DEFINITION,             ///< A function definition instruction.
        PARAMETER_DEFINITION,   ///< The parameter definition of a function.
        RETURN_DEFINITION,      ///< The return type definition of a function.
        OPEN_SCOPE,             ///< An open scope instruction.
        CLOSE_SCOPE,            ///< A close scope instruction.
        BLANK                   ///< A blank instruction.
    };
}

template <> struct fmt::formatter<vnd::InstructionType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(vnd::InstructionType instructionType, FormatContext &ctx) {
        std::string_view name;
        switch(instructionType) {
            using enum vnd::InstructionType;
        case PARAMETER_EXPRESSION:
            name = "PARAMETER_EXPRESSION";
            break;
        case OPERATION:
            name = "OPERATION";
            break;
        case ASSIGNATION:
            name = "ASSIGNATION";
            break;
        case EXPRESSION:
            name = "EXPRESSION";
            break;
        case SQUARE_EXPRESSION:
            name = "SQUARE_EXPRESSION";
            break;
        case RETURN_EXPRESSION:
            name = "RETURN_EXPRESSION";
            break;
        case DECLARATION:
            name = "DECLARATION";
            break;
        case INITIALIZATION:
            name = "INITIALIZATION";
            break;
        case ARRAY_INIZIALIZATION:
            name = "ARRAY_INIZIALIZATION";
            break;
        case MAIN:
            name = "MAIN";
            break;
        case STRUCTURE:
            name = "STRUCTURE";
            break;
        case ELSE:
            name = "ELSE";
            break;
        case FOR_STRUCTURE:
            name = "FOR_STRUCTURE";
            break;
        case BREAK:
            name = "BREAK";
            break;
        case FOR_INITIALIZATION:
            name = "FOR_INITIALIZATION";
            break;
        case FOR_CONDITION:
            name = "FOR_CONDITION";
            break;
        case FOR_STEP:
            name = "FOR_STEP";
            break;
        case DEFINITION:
            name = "DEFINITION";
            break;
        case PARAMETER_DEFINITION:
            name = "PARAMETER_DEFINITION";
            break;
        case RETURN_DEFINITION:
            name = "RETURN_DEFINITION";
            break;
        case OPEN_SCOPE:
            name = "OPEN_SCOPE";
            break;
        case CLOSE_SCOPE:
            name = "CLOSE_SCOPE";
            break;
        case BLANK:
            name = "BLANK";
            break;
        default:
            name = "UNKNOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};