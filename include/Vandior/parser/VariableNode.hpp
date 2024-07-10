//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"
#include "IndexNode.hpp"

DISABLE_WARNINGS_PUSH(26445)

namespace vnd {

    /**
     * @brief Variable node class representing variable names in the AST.
     */
    class VariableNode : public ASTNode {
    public:
        /**
         * @brief Constructor for VariableNode.
         * @param _name Name of the variable.
         * @param name_Token token of the name of the variable.
         */
        explicit VariableNode(std::string_view _name, const Token &name_Token) noexcept : ASTNode(name_Token), name(_name), m_index(nullptr) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Variable; }

        [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), name); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("VAR({})", name); }
        [[nodiscard]] const std::string_view &getName() const noexcept { return name; }

        
        /**
         * @brief Gets the index node of the node.
         * @return The index node of the node.
         */
        [[nodiscard]] const std::unique_ptr<IndexNode> &get_index() const noexcept { return m_index; }

        /**
         * @brief Sets the index node of the node.
         * @param index The index node of the node.
         */
        [[nodiscard]] void set_index(std::unique_ptr<IndexNode> index) noexcept { m_index = vnd_move_always_even_const(index); }

        friend void swap(VariableNode &lhs, VariableNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.name, rhs.name);
        }

    private:
        std::string_view name;
        std::unique_ptr<IndexNode> m_index;  ///< The possible index node of an array type.
    };

}  // namespace vnd
DISABLE_WARNINGS_POP()
