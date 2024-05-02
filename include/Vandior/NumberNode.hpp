//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"
#include "NumberNodeType.hpp"
namespace vnd {

    /**
     * @brief Number node class representing numeric values in the AST.
     */
    class NumberNode : public ASTNode {
    public:
        /**
         * @brief Constructor for NumberNode with integer value.
         * @param _value Integer value of the number.
         */
        // explicit NumberNode(int _value) noexcept : value(C_D(_value)) {}
        /**
         * @brief Constructor for NumberNode with double value.
         * @param _value Double value of the number.
         */
        // explicit NumberNode(double _value) noexcept : value(_value) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Number; }
        [[nodiscard]] virtual NumberNodeType getNumberType() const = 0;

        /*[[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), value); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("NUM({})", value); }
        [[nodiscard]] double getValue() const noexcept { return value; }*/

    private:
        // double value;
    };

}  // namespace vnd
