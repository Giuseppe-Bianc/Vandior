//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "Log.hpp"
#include "headers.hpp"

/**
 * @brief Enum representing different types of AST nodes.
 */
enum class NodeType { BinaryExpression, UnaryExpression, Number, Variable };

/**
 * @brief Specialization of fmt::formatter for NodeType enumeration.
 */
template <> struct fmt::formatter<NodeType> : fmt::formatter<std::string_view> {
    /**
     * @brief Formats the NodeType for printing.
     * @param nodeType The node type to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(NodeType nodeType, FormatContext &ctx) {
        std::string_view name;
        switch(nodeType) {
        case NodeType::BinaryExpression:
            name = "BINARY_EXPRESION";
            break;
        case NodeType::UnaryExpression:
            name = "UNARY_EXPRESION";
            break;
        case NodeType::Number:
            name = "NUMBER";
            break;
        case NodeType::Variable:
            name = "VARIABLE";
            break;
        default:
            name = "UNKOWN";
            break;
        }
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};

/**
 * @brief Base class for Abstract Syntax Tree (AST) nodes.
 *
 * This class serves as the base for all AST nodes in the system.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    /**
     * @brief Gets the type of the AST node.
     * @return NodeType enumeration value.
     */
    [[nodiscard]]virtual NodeType getType() const = 0;
    /**
     * @brief Returns a string representation of the AST node.
     * @return String representation of the AST node.
     */
    [[nodiscard]] virtual std::string print() const = 0;
    /**
     * @brief Returns a compact string representation of the AST node for compilation purposes.
     * @return Compact string representation of the AST node.
     */
    [[nodiscard]] virtual std::string comp_print() const = 0;
};

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

private:
    std::string_view op;
    std::unique_ptr<ASTNode> left, right;
};

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
      : op(_op), operand(std::move(_operand)) {}

    [[nodiscard]] NodeType getType() const noexcept override { return NodeType::UnaryExpression; }
    [[nodiscard]] std::string print() const override { return FORMAT("{}(op:\"{}\" operand:{})", getType(), op, operand->print()); }
    [[nodiscard]] std::string comp_print() const override { return FORMAT("UNE(op:\"{}\" opr:{})", op, operand->comp_print()); }
    [[nodiscard]] const std::string_view &getOp() const noexcept { return op; }
    [[nodiscard]] const std::unique_ptr<ASTNode> &getOperand() const noexcept { return operand; }

private:
    std::string_view op;
    std::unique_ptr<ASTNode> operand;
};

/**
 * @brief Number node class representing numeric values in the AST.
 */
class NumberNode : public ASTNode {
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
class VariableNode : public ASTNode {
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
static inline constexpr void print_indent_dl(int indent, const auto &label, const auto &value, const auto &labelnl) {
    print_indent(indent, label, value);
    LINFO("{: ^{}}{}:", "", indent, labelnl);
}

/**
 * @brief Pretty prints the AST starting from the given node with optional indentation.
 * @param node The root of the AST to be pretty printed.
 * @param indent Number of spaces for indentation.
 */
 //NOLINTNEXTLINE(misc-no-recursion)
static inline constexpr void prettyPrint(const ASTNode &node, int indent = 0) {
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
