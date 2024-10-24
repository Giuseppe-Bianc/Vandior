#pragma once

#include "ASTNode.hpp"
#include "ArrayNode.hpp"

DISABLE_WARNINGS_PUSH(4625 4626 26445)

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
        [[nodiscard]] IndexNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept;

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override;

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override;

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override;

        /**
         * @brief Gets the elements of the node.
         * @return The elements of the node.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &get_elements() const noexcept;

        /**
         * @brief Gets the index node of the node.
         * @return The index node of the node.
         */
        [[nodiscard]] const std::unique_ptr<IndexNode> &get_index() const noexcept;

        /**
         * @brief Sets the index node of the node.
         * @param index The index node of the node.
         */
        void set_index(std::unique_ptr<IndexNode> index) noexcept;
        /**
         * @brief Gets the array node of the node.
         * @return The array node of the node.
         */
        [[nodiscard]] const std::unique_ptr<ArrayNode> &get_array() const noexcept;

        /**
         * @brief Sets the array node of the node.
         * @param _array The array node of the node.
         */
        void set_array(std::unique_ptr<ArrayNode> _array) noexcept;

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

DISABLE_WARNINGS_POP()
