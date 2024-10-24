//
// Created by gbian on 30/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "ASTNode.hpp"

DISABLE_WARNINGS_PUSH(4625 4626 26445)

namespace vnd {

    /**
     * @brief Binary expression node class representing nodes with two operands and an operator.
     */
    class BinaryExpressionNode : public ASTNode {
    public:
        /**
         * @brief Constructor for BinaryExpressionNode.
         * @param _op Operator for the binary expression.
         * @param op_Token Token of operator for the binary expression.
         * @param _left Left operand.
         * @param _right Right operand.
         */
        [[nodiscard]] BinaryExpressionNode(std::string_view _op, const Token &op_Token, std::unique_ptr<ASTNode> _left,
                                           std::unique_ptr<ASTNode> _right) noexcept;

        /**
         * @brief Constructor for BinaryExpressionNode.
         * @param op_Token Token of operator for the binary expression.
         * @param _left Left operand.
         * @param _right Right operand.
         */
        [[nodiscard]] BinaryExpressionNode(const Token &op_Token, std::unique_ptr<ASTNode> _left, std::unique_ptr<ASTNode> _right) noexcept;

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override;

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override;

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override;

        /**
         * @brief Gets the operator of the binary expression.
         * @return The operator as a string view.
         */
        [[nodiscard]] std::string_view getOp() const noexcept;

        /**
         * @brief Gets the left operand of the binary expression.
         * @return A constant reference to the unique pointer of the left operand.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &getLeft() const noexcept;

        /**
         * @brief Gets the right operand of the binary expression.
         * @return A constant reference to the unique pointer of the right operand.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &getRight() const noexcept;
        [[nodiscard]] const ASTNode &getLeftr() const noexcept;
        [[nodiscard]] const ASTNode &getRightr() const noexcept;

        friend void swap(BinaryExpressionNode &lhs, BinaryExpressionNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.op, rhs.op);
            swap(lhs.left, rhs.left);
            swap(lhs.right, rhs.right);
        }

    private:
        std::string_view op;             ///< The operator for the binary expression.
        std::unique_ptr<ASTNode> left;   ///< The left operand.
        std::unique_ptr<ASTNode> right;  ///< The right operand.
    };
}  // namespace vnd

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)