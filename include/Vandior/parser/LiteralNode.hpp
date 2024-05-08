//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"
namespace vnd {

    /**
     * @brief generic Node class representing a literal values in the AST.
     */
    template <typename T> class LiteralNode : public ASTNode {
    public:
        /**
         * @brief Creates a LiteralNode.
         * @param value The value of the node.
         * @param token The token correspondent to the node.
         * @param type NodeType of the node.
         */
        LiteralNode(T value, const Token &token, NodeType type) noexcept : ASTNode(token), m_value(value), m_type(type) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return m_type; }
        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}_LIT({})", m_type, m_value); }
        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}({})", m_type, m_value); }
        /**
         * @brief Gets the value of the node.
         * @return The value of the node.
         */
        [[nodiscard]] T get_value() const noexcept { return m_value; }
        friend void swap(LiteralNode<T> &lhs, LiteralNode<T> &rhs) noexcept {
            using std::swap;
            swap(static_cast<LiteralNode<T> &>(lhs), static_cast<LiteralNode<T> &>(rhs));
            swap(lhs.m_value, rhs.m_value);
            swap(lhs.m_type, rhs.m_type);
        }

    private:
        T m_value;
        NodeType m_type;
    };

}  // namespace vnd
