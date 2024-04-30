//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "ASTNode.hpp"
#include "Log.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26445 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

/**
 * @brief Binary expression node class representing nodes with two operands and an operator.
 */
class BinaryExpressionNode : public vnd::ASTNode {
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
    [[nodiscard]] const std::unique_ptr<vnd::ASTNode> &getLeft() const noexcept { return left; }
    [[nodiscard]] const std::unique_ptr<vnd::ASTNode> &getRight() const noexcept { return right; }
    [[nodiscard]] const vnd::ASTNode &getLeftr() const noexcept { return *left.get(); }
    [[nodiscard]] const vnd::ASTNode &getRightr() const noexcept { return *right.get(); }

private:
    std::string_view op;
    std::unique_ptr<vnd::ASTNode> left;
    std::unique_ptr<vnd::ASTNode> right;
};

/**
 * @brief Unary expression node class representing nodes with a single operand and an operator.
 */
class UnaryExpressionNode : public vnd::ASTNode {
public:
    /**
     * @brief Constructor for UnaryExpressionNode.
     * @param _op Operator for the unary expression.
     * @param _operand Operand of the unary expression.
     */
    UnaryExpressionNode(std::string_view _op, std::unique_ptr<vnd::ASTNode> _operand) noexcept : op(_op), operand(std::move(_operand)) {}

    [[nodiscard]] NodeType getType() const noexcept override { return NodeType::UnaryExpression; }
    [[nodiscard]] std::string print() const override { return FORMAT("{}(op:\"{}\" operand:{})", getType(), op, operand->print()); }
    [[nodiscard]] std::string comp_print() const override { return FORMAT("UNE(op:\"{}\" opr:{})", op, operand->comp_print()); }
    [[nodiscard]] const std::string_view &getOp() const noexcept { return op; }
    [[nodiscard]] const std::unique_ptr<vnd::ASTNode> &getOperand() const noexcept { return operand; }
    [[nodiscard]] const vnd::ASTNode &getOperandr() const noexcept { return *operand.get(); }

private:
    std::string_view op;
    std::unique_ptr<ASTNode> operand;
};

/**
 * @brief Number node class representing numeric values in the AST.
 */
class NumberNode : public vnd::ASTNode {
public:
    /**
     * @brief Constructor for NumberNode with integer value.
     * @param _value Integer value of the number.
     */
    explicit NumberNode(int _value) noexcept : value(C_D(_value)) {}
    /**
     * @brief Constructor for NumberNode with double value.
     * @param _value Double value of the number.
     */
    explicit NumberNode(double _value) noexcept : value(_value) {}

    [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Number; }

    [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), value); }
    [[nodiscard]] std::string comp_print() const override { return FORMAT("NUM({})", value); }
    [[nodiscard]] double getValue() const noexcept { return value; }

private:
    double value;
};

/**
 * @brief Variable node class representing variable names in the AST.
 */
class VariableNode : public vnd::ASTNode {
public:
    /**
     * @brief Constructor for VariableNode.
     * @param _name Name of the variable.
     */
    explicit VariableNode(std::string_view _name) noexcept : name(_name) {}

    [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Variable; }

    [[nodiscard]] std::string print() const override { return FORMAT("{} ({})", getType(), name); }
    [[nodiscard]] std::string comp_print() const override { return FORMAT("VAR({})", name); }
    [[nodiscard]] const std::string_view &getName() const noexcept { return name; }

private:
    std::string_view name;
};

/**
 * @brief Utility function for printing with indentation.
 * @param indent Number of spaces for indentation.
 * @param label Label for the printed information.
 * @param value Value to be printed.
 */
// NOLINTNEXTLINE
static inline constexpr void print_indent(int indent, const auto &label, const auto &value) {
    LINFO("{: ^{}}{}: {}", "", indent, label, value);
}
/**
 * @brief Utility function for printing with indentation, followed by a new line.
 * @param indent Number of spaces for indentation.
 * @param label Label for the printed information.
 * @param value Value to be printed.
 * @param labelnl Label for the new line.
 */
// NOLINTNEXTLINE
static inline constexpr void print_indent_dl(int indent, const auto &label, const auto &value, const auto &labelnl) {
    print_indent(indent, label, value);
    LINFO("{: ^{}}{}:", "", indent, labelnl);
}

/**
 * @brief Pretty prints the AST starting from the given node with optional indentation.
 * @param node The root of the AST to be pretty printed.
 * @param indent Number of spaces for indentation.
 */
// NOLINTNEXTLINE(misc-no-recursion)
static inline constexpr void prettyPrint(const vnd::ASTNode &node, int indent = 0) {
    // Recursively print children for Binary and Unary expression nodes
    if(const auto *binaryNode = dynamic_cast<const BinaryExpressionNode *>(&node)) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), binaryNode->getOp()), "Left");
        prettyPrint(*binaryNode->getLeft(), indent + 2);
        print_indent(indent, "Right", "");
        prettyPrint(*binaryNode->getRight(), indent + 2);
    } else if(const auto *unaryNode = dynamic_cast<const UnaryExpressionNode *>(&node)) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), unaryNode->getOp()), "Operand");
        prettyPrint(*unaryNode->getOperand(), indent + 2);
    } else if(const auto *numberNode = dynamic_cast<const NumberNode *>(&node)) {
        print_indent(indent, "Node", FORMAT("(Type: {}, value:{})", node.getType(), numberNode->getValue()));
    } else if(const auto *variableNode = dynamic_cast<const VariableNode *>(&node)) {
        print_indent(indent, "Node", FORMAT("(Type: {}, value:{})", node.getType(), variableNode->getName()));
    }
}

DISABLE_WARNINGS_POP()
