//
// Created by gbian on 30/04/2024.
//

#pragma once

#include "ASTNode.hpp"

namespace vnd {

    /**
     * @brief Unary expression node class representing nodes with a single operand and an operator.
     */
    class UnaryExpressionNode : public ASTNode {
    public:
        /**
         * @brief Constructor for UnaryExpressionNode.
         * @param _op Operator for the unary expression.
         * @param _operand Operand of the unary expression.
         */
        UnaryExpressionNode(std::string_view _op, std::unique_ptr<ASTNode> _operand) noexcept
          : op(_op), operand(vnd_move_always_even_const(_operand)) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::UnaryExpression; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}(op:\"{}\" operand:{})", getType(), op, operand->print()); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("UNE(op:\"{}\" opr:{})", op, operand->comp_print()); }
        [[nodiscard]] const std::string_view &getOp() const noexcept { return op; }
        [[nodiscard]] const std::unique_ptr<ASTNode> &getOperand() const noexcept { return operand; }
        [[nodiscard]] const ASTNode &getOperandr() const noexcept { return *operand.get(); }

        friend void swap(UnaryExpressionNode &lhs, UnaryExpressionNode &rhs) noexcept {
            using std::swap;
            swap(lhs.op, rhs.op);
            swap(lhs.operand, rhs.operand);
        }

    private:
        std::string_view op;
        std::unique_ptr<ASTNode> operand;
    };

}  // namespace vnd
