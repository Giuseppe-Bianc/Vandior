//
// Created by gbian on 02/05/2024.
//

#pragma once
#include "NumberNode.hpp"

namespace vnd {

    class DoubleNumberNode : public NumberNode {
    public:
        /**
         * @brief Constructor for DoubleNumberNode.
         * @param value Double value of the number.
         * @param value_Token token of the value.
         */
        explicit DoubleNumberNode(double value, const Token &value_Token) noexcept : NumberNode(value_Token), m_value(value) {}

        [[nodiscard]] NumberNodeType getNumberType() const override { return NumberNodeType::Double; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}_{}({})", getType(), getNumberType(), m_value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("NUM_{}({})", getNumberType(), m_value); }
        [[nodiscard]] double get_value() const noexcept { return m_value; }

        friend void swap(DoubleNumberNode &lhs, DoubleNumberNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<NumberNode &>(lhs), static_cast<NumberNode &>(rhs));
            swap(lhs.m_value, rhs.m_value);
        }

    private:
        double m_value;
    };

}  // namespace vnd
