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
        // Inherit all constructors from the base class ASTNode
        LiteralNode(T value, const Token &token) noexcept : ASTNode(token), m_value(value) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Literal; }
        [[nodiscard]] std::string print() const override { return "literal"; }
        [[nodiscard]] std::string comp_print() const override { return "l"; }
        [[nodiscard]] T get_value() const noexcept { return m_value; }
        friend void swap(LiteralNode<T> &lhs, LiteralNode<T> &rhs) noexcept {
            using std::swap;
            swap(static_cast<LiteralNode<T> &>(lhs), static_cast<LiteralNode<T> &>(rhs));
            swap(lhs.m_value, rhs.m_value);
        }

    private:
        T m_value;
    };

}  // namespace vnd
