//
// Created by gbian on 02/05/2024.
//

#pragma once

#include "NumberNode.hpp"

namespace vnd {

    class IntegerNumberNode : public NumberNode {
    public:
        /**
         * @brief Constructor for IntegerNumberNode.
         * @param value Integer value of the number.
         * @param value_Token token of the value.
         */
        explicit IntegerNumberNode(int value, const Token &value_Token) noexcept : NumberNode(value_Token), m_value(value) {}

        [[nodiscard]] NumberNodeType getNumberType() const override { return NumberNodeType::Integer; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}_{}({})", getType(), getNumberType(), m_value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("NUM_{}({})", getNumberType(), m_value); }
        [[nodiscard]] int get_value() const noexcept { return m_value; }

        friend void swap(IntegerNumberNode &lhs, IntegerNumberNode &rhs) noexcept { std::swap(lhs.m_value, rhs.m_value); }

    private:
        int m_value;
    };

}  // namespace vnd
