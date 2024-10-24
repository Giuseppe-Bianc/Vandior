//
// Created by potex02 on 10/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
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
        [[nodiscard]] ArrayNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept;

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

// NOLINTEND(*-include-cleaner)