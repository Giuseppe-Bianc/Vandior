//
// Created by potex02 on 10/07/2024.
//

#pragma once

#include "ASTNode.hpp"
DISABLE_WARNINGS_PUSH(4625 4626 26445)
namespace vnd {

    /**
     * @brief Node class representing literal array values in the AST.
     */
    class ArrayNode : public ASTNode {
    public:
        /**
         * @brief Constructs a ArrayNode.
         * @param elements The elements of the array.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] ArrayNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept
          : ASTNode(token), m_elements(vnd_move_always_even_const(elements)) {
            if(m_elements) m_elements->set_parent(this);
        }

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Array; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override {
            if(m_elements) { return FORMAT("{}({})", getType(), m_elements->comp_print()); }
            return FORMAT("{}()", getType());
        }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}", getType()); }

        /**
         * @brief Gets the elements of the node.
         * @return The elements of the node.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &get_elements() const noexcept { return m_elements; }

        /**
         * @brief Swaps the contents of two LiteralNode objects.
         * @param lhs The first LiteralNode.
         * @param rhs The second LiteralNode.
         */
        friend void swap(ArrayNode &lhs, ArrayNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.m_elements, rhs.m_elements);
        }

    private:
        std::unique_ptr<ASTNode> m_elements;  ///< The elements child of the array.
    };

}  // namespace vnd
DISABLE_WARNINGS_POP()