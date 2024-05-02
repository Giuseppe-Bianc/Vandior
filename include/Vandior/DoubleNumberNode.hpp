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
         */
        explicit DoubleNumberNode(double value) noexcept : m_value(value) {}

        [[nodiscard]] NumberNodeType getNumberType() const override { return NumberNodeType::Double; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}_{}({})", getType(), getNumberType(), m_value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("NUM_{}({})", getNumberType(), m_value); }
        [[nodiscard]] double get_value() const noexcept { return m_value; }

    private:
        double m_value;
    };

}  // namespace vnd
