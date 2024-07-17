//
// Created by potex02 on 06/07/2024.
//

#pragma once

#include "ASTNode.hpp"
#include "ArrayNode.hpp"

namespace vnd {

    /**
     * @brief Node class representing an index values in the AST.
     */
    class IndexNode : public ASTNode {
    public:
        /**
         * @brief Constructs a IndexNode.
         * @param elements The root element of the index.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] IndexNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept
          : ASTNode(token), m_elements(vnd_move_always_even_const(elements)), m_index(nullptr), m_array(nullptr) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Index; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), m_elements->comp_print()); }

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
         * @brief Gets the index node of the node.
         * @return The index node of the node.
         */
        [[nodiscard]] const std::unique_ptr<IndexNode> &get_index() const noexcept { return m_index; }

        /**
         * @brief Sets the index node of the node.
         * @param index The index node of the node.
         */
        void set_index(std::unique_ptr<IndexNode> index) noexcept { m_index = vnd_move_always_even_const(index); }

        /**
         * @brief Gets the array node of the node.
         * @return The array node of the node.
         */
        [[nodiscard]] const std::unique_ptr<ArrayNode> &get_array() const noexcept { return m_array; }

        /**
         * @brief Sets the array node of the node.
         * @param _array The array node of the node.
         */
        void set_array(std::unique_ptr<ArrayNode> _array) noexcept { m_array = vnd_move_always_even_const(_array); }

        /**
         * @brief Swaps the contents of two LiteralNode objects.
         * @param lhs The first LiteralNode.
         * @param rhs The second LiteralNode.
         */
        friend void swap(IndexNode &lhs, IndexNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.m_elements, rhs.m_elements);
            swap(lhs.m_index, rhs.m_index);
            swap(lhs.m_array, rhs.m_array);
        }

    private:
        std::unique_ptr<ASTNode> m_elements;  ///< The elements child of the index.
        std::unique_ptr<IndexNode> m_index;   ///< The possible index node of an array type.
        std::unique_ptr<ArrayNode> m_array;   ///< The array elements of the index.
    };

}  // namespace vnd
