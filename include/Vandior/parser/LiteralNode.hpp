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
        LiteralNode(T value, const Token &token) noexcept : ASTNode(token), m_value(value) {
            if(std::is_same_v<T, bool>) {
                m_type = NodeType::Boolean;
                return;
            }
            if(std::is_same_v<T, char>) {
                m_type = NodeType::Char;
                return;
            }
            if(std::is_same_v<T, std::string_view>) {
                m_type = NodeType::String;
                return;
            }
        }

        [[nodiscard]] NodeType getType() const noexcept override { return m_type; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), m_value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}", m_value); }
        [[nodiscard]] T get_value() const noexcept { return m_value; }
        friend void swap(LiteralNode<T> &lhs, LiteralNode<T> &rhs) noexcept {
            using std::swap;
            swap(static_cast<LiteralNode<T> &>(lhs), static_cast<LiteralNode<T> &>(rhs));
            swap(lhs.m_value, rhs.m_value);
        }

    private:
        T m_value;
        NodeType m_type;
    };

}  // namespace vnd
