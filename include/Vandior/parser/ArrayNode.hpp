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
    template <typename T> class ArrayNode : public ASTNode {
    public:
        /**
         * @brief Constructs a ArrayNode.
         * @param root The root element of the array.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] ArrayNode(std::unique_ptr<ASTNode> root, const Token &token) noexcept
          : ASTNode(token), m_root(vnd_move_always_even_const(root)) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Array; }
        
        /**
         * @brief Gets the demangled name of the type T.
         * @return Demangled name of the type T if using GCC/Clang, otherwise mangled name.
         */
        [[nodiscard]] std::string_view getTypeIDName() const noexcept { return typeid(T).name(); }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}<{}>", getType(), getTypeIDName()); }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}", getType()); }

        /**
         * @brief Gets the root of the node.
         * @return The root of the node.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &get_root() const noexcept { return m_root; }

        /**
         * @brief Swaps the contents of two LiteralNode objects.
         * @param lhs The first LiteralNode.
         * @param rhs The second LiteralNode.
         */
        friend void swap(ArrayNode &lhs, ArrayNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.m_root, rhs.m_root);
        }

    private:
        std::unique_ptr<ASTNode> m_root;  ///< The root child of the array.
    };

}  // namespace vnd
