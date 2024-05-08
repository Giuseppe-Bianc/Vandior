//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "LiteralNode.hpp"
#include "NumberNodeType.hpp"

namespace vnd {

    /**
     * @brief Number node class representing numeric values in the AST.
     */
    template <typename T>
    class NumberNode : public LiteralNode<T> {
    public:
        NumberNode(T value, const Token &token, NumberNodeType number_type)
          : LiteralNode<T>(value, token, NodeType::Number), m_number_type(number_type) {}
        [[nodiscard]] virtual NumberNodeType getNumberType() const { return m_number_type; };
        friend void swap(NumberNode &lhs, NumberNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<LiteralNode<T> &>(lhs), static_cast<LiteralNode<T> &>(rhs));
            swap(lhs.m_number_type, rhs.m_number_type);
        }

    private:
        NumberNodeType m_number_type;
    };

}  // namespace vnd
