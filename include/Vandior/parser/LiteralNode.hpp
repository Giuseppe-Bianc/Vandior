//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"
namespace vnd {

    /**
     * @brief Node class representing a literal values in the AST.
     */
    template <typename T>
    class LiteralNode : public ASTNode {
    public:
        LiteralNode(T value, const Token &token, NodeType type) noexcept : ASTNode(token), m_value(value), m_type(type) {}

        [[nodiscard]] NodeType getType() const noexcept override { return m_type; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), m_value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}", m_value); }
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
