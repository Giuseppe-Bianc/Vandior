//
// Created by gbian on 30/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "ASTNode.hpp"

DISABLE_WARNINGS_PUSH(4625 4626 26445)

namespace vnd {

    /**
     * @brief Unary expression node class representing nodes with a single operand and an operator.
     */
    class UnaryExpressionNode : public ASTNode {
    public:
        /**
         * @brief Constructor for UnaryExpressionNode.
         * @param _op Operator for the unary expression.
         * @param op_Token token of the operator for the unary expression.
         * @param _operand Operand of the unary expression.
         */
        [[nodiscard]] UnaryExpressionNode(std::string_view _op, const Token &op_Token, std::unique_ptr<ASTNode> _operand) noexcept
          : ASTNode(op_Token), op(_op), operand(vnd_move_always_even_const(_operand)) {
            if(operand) operand->set_parent(this);
        }

        /**
         * @brief Constructor for UnaryExpressionNode.
         * @param op_Token token of the operator for the unary expression.
         * @param _operand Operand of the unary expression.
         */
        [[nodiscard]] UnaryExpressionNode(const Token &op_Token, std::unique_ptr<ASTNode> _operand) noexcept
          : ASTNode(op_Token), op(op_Token.getValue()), operand(vnd_move_always_even_const(_operand)) {
            if(operand) operand->set_parent(this);
        }

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::UnaryExpression; }
        [[nodiscard]] std::string print() const override { return FORMAT("{}(op:\"{}\" operand:{})", getType(), op, operand->print()); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("UNE(op:\"{}\" opr:{})", op, operand->comp_print()); }

        /**
         * @brief Gets the operator for the unary expression.
         * @return The operator for the unary expression.
         */
        [[nodiscard]] const std::string_view &getOp() const noexcept { return op; }
        /**
         * @brief Gets the operand of the unary expression.
         * @return The operand of the unary expression.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &getOperand() const noexcept { return operand; }
        /**
         * @brief Gets the operand of the unary expression.
         * @return The operand of the unary expression.
         */
        [[nodiscard]] const ASTNode &getOperandr() const noexcept { return *operand.get(); }

        friend void swap(UnaryExpressionNode &lhs, UnaryExpressionNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.op, rhs.op);
            swap(lhs.operand, rhs.operand);
        }

    private:
        std::string_view op;               ///< Operator for the unary expression.
        std::unique_ptr<ASTNode> operand;  ///< Operand of the unary expression.
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)
