//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"

namespace vnd {

    /**
     * @brief Variable node class representing variable names in the AST.
     */
    class VariableNode : public vnd::ASTNode {
    public:
        /**
         * @brief Constructor for VariableNode.
         * @param _name Name of the variable.
         */
        explicit VariableNode(std::string_view _name) noexcept : name(_name) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Variable; }

        [[nodiscard]] std::string print() const override { return FORMAT("{} ({})", getType(), name); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("VAR({})", name); }
        [[nodiscard]] const std::string_view &getName() const noexcept { return name; }

    private:
        std::string_view name;
    };

}  // namespace vnd