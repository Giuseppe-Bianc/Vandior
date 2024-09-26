//
// Created by potex02 on 20/09/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "ASTNode.hpp"

namespace vnd {

    /**
     * @brief Node class representing null value in the AST.
     */
    class NullptrNode : public ASTNode {
    public:
        /**
         * @brief Creates a NullptrNode.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] explicit NullptrNode(const Token &token) noexcept : ASTNode(token) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Nullptr; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return "NULLPTR"; }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return "NULL"; }

        friend void swap(NullptrNode &lhs, NullptrNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
        }
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)