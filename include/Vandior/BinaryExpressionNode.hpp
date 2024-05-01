//
// Created by gbian on 30/04/2024.
//

#pragma once

#include "ASTNode.hpp"

namespace vnd {
    /**
     * @brief Binary expression node class representing nodes with two operands and an operator.
     */
    class BinaryExpressionNode : public ASTNode {
    public:
        /**
         * @brief Constructor for BinaryExpressionNode.
         * @param _op Operator for the binary expression.
         * @param _left Left operand.
         * @param _right Right operand.
         */
        BinaryExpressionNode(std::string_view _op, std::unique_ptr<ASTNode> _left, std::unique_ptr<ASTNode> _right) noexcept
          : op(_op), left(std::move(_left)), right(std::move(_right)) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::BinaryExpression; }

        [[nodiscard]] std::string print() const override {
            return FORMAT("{}(op:\"{}\" left:{}, right:{})", getType(), op, left->print(), right->print());
        }
        [[nodiscard]] std::string comp_print() const override {
            return FORMAT("BINE(op:\"{}\" l:{}, r:{})", op, left->comp_print(), right->comp_print());
        }

        [[nodiscard]] const std::string_view &getOp() const noexcept { return op; }
        [[nodiscard]] const std::unique_ptr<ASTNode> &getLeft() const noexcept { return left; }
        [[nodiscard]] const std::unique_ptr<ASTNode> &getRight() const noexcept { return right; }
        [[nodiscard]] const ASTNode &getLeftr() const noexcept { return *left.get(); }
        [[nodiscard]] const ASTNode &getRightr() const noexcept { return *right.get(); }

    private:
        std::string_view op;
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
    };  // namespace vnd::ASTNode
}  // namespace vnd