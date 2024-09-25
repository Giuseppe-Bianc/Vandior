//
// Created by potex02 on 14/05/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "IndexNode.hpp"

DISABLE_WARNINGS_PUSH(4625 4626 4820 26445)

namespace vnd {

    /**
     * @brief Node class representing type value in the AST.
     */
    class TypeNode : public ASTNode {
    public:
        /**
         * @brief Creates a TypeNode.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] explicit TypeNode(const Token &token) noexcept
          : ASTNode(token), m_value(token.getValue()), m_type(token.getType()), m_index(nullptr) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Type; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}_{}({})", getVariableType(), getType(), get_value()); }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}({})", getType(), get_value()); }

        /**
         * @brief Gets the value of the node.
         * @return The value of the node.
         */
        [[nodiscard]] std::string_view get_value() const noexcept { return m_value; }

        /**
         * @brief Gets the index node of the node.
         * @return The index node of the node.
         */
        [[nodiscard]] const std::unique_ptr<IndexNode> &get_index() const noexcept { return m_index; }

        /**
         * @brief Sets the index node of the node.
         * @param index The index node of the node.
         */
        void set_index(std::unique_ptr<IndexNode> index) noexcept {
            m_index = vnd_move_always_even_const(index);
            if(m_index) { m_index->set_parent(this); }
        }

        /**
         * @brief Gets the variable type of the node.
         * @return The variable type of the node.
         */
        [[nodiscard]] TokenType getVariableType() const noexcept { return m_type; }

        friend void swap(TypeNode &lhs, TypeNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.m_value, rhs.m_value);
            swap(lhs.m_type, rhs.m_type);
            swap(lhs.m_index, rhs.m_index);
        }

    private:
        std::string_view m_value;            ///< The value of the node.
        TokenType m_type;                    ///< The type of the token node.
        std::unique_ptr<IndexNode> m_index;  ///< The possible index node of an array type.
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)

DISABLE_WARNINGS_POP()
