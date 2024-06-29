//
// Created by potex02 on 23/06/2024.
//

#pragma once

#include "ASTNode.hpp"

namespace vnd {

    /**
     * @brief Generic Node class representing literal array values in the AST.
     * @tparam T The type of the literal array values.
     */
    class ArrayNode : public ASTNode {
    public:
        /**
         * @brief Constructs a ArrayNode.
         * @param root The root element of the array.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] ArrayNode(std::unique_ptr<ASTNode> dimension, std::unique_ptr<ASTNode> elements, const Token &token) noexcept
          : ASTNode(token), m_dimension(vnd_move_always_even_const(dimension)), m_elements(vnd_move_always_even_const(elements)) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Array; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}<{}>", getType(), m_dimension->comp_print()); }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override {
            return FORMAT("{}<{}>{}", getType(), m_dimension->comp_print(), m_elements->comp_print());
        }

        /**
         * @brief Gets the dimension of the node.
         * @return The dimension of the node.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &get_dimension() const noexcept { return m_dimension; }

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
            swap(lhs.m_dimension, rhs.m_dimension);
            swap(lhs.m_elements, rhs.m_elements);
        }

    private:
        std::unique_ptr<ASTNode> m_dimension;  ///< The dimension of the array.
        std::unique_ptr<ASTNode> m_elements;   ///< The elements child of the array.
    };

}  // namespace vnd
