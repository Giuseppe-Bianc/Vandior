//
// Created by gbian on 01/01/2024.
//

#pragma once

#include "headers.hpp"
#include "Log.hpp"

// Define different types of AST nodes
enum class NodeType { BinaryExpression, UnaryExpression, Number, Variable };

template <> struct fmt::formatter<NodeType> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
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

// Base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual std::string print() const = 0;
};

// Binary expression node
class BinaryExpressionNode : public ASTNode {
public:
    BinaryExpressionNode(std::string_view _op, std::unique_ptr<ASTNode> _left, std::unique_ptr<ASTNode> _right)
      : op(_op), left(std::move(_left)), right(std::move(_right)) {}

    NodeType getType() const override { return NodeType::BinaryExpression; }

    std::string print() const override {
        return FORMAT("{}(op:\"{}\" left:{}, right:{})", getType(), op, left->print(), right->print());
    }

    const std::string_view &getOp() const { return op; }
    const std::unique_ptr<ASTNode> &getLeft() const { return left; }
    const std::unique_ptr<ASTNode> &getRight() const { return right; }

private:
    std::string_view op;
    std::unique_ptr<ASTNode> left, right;
};

// Unary expression node
class UnaryExpressionNode : public ASTNode {
public:
    UnaryExpressionNode(std::string_view _op, std::unique_ptr<ASTNode> _operand) : op(_op), operand(std::move(_operand)) {}

    NodeType getType() const override { return NodeType::UnaryExpression; }
    std::string print() const override { return FORMAT("{}(op:\"{}\" operand:{})", getType(), op, operand->print()); }
    const std::string_view &getOp() const { return op; }
    const std::unique_ptr<ASTNode> &getOperand() const { return operand; }

private:
    std::string_view op;
    std::unique_ptr<ASTNode> operand;
};

// Number node
class NumberNode : public ASTNode {
public:
    explicit NumberNode(int _value) : value(C_D(_value)) {}
    explicit NumberNode(double _value) : value(_value) {}

    NodeType getType() const override { return NodeType::Number; }

    std::string print() const override { return FORMAT("{}({})", getType(), value); }
    double getValue() const { return value; }

private:
    double value;
};

// Variable node
class VariableNode : public ASTNode {
public:
    explicit VariableNode(std::string_view _name) : name(_name) {}

    NodeType getType() const override { return NodeType::Variable; }

    std::string print() const override { return FORMAT("{} ({})", getType(), name); }
    const std::string_view &getName() const { return name; }

private:
    std::string_view name;
};


static inline constexpr void print_indent(int indent, const auto& label,const auto& value){
    LINFO("{: ^{}}{}: {}", "", indent, label, value);
}
static inline constexpr void print_indent_dl(int indent, const auto& label,const auto& value, const auto& labelnl){
    print_indent(indent, label, value);
    LINFO("{: ^{}}{}:", "", indent, labelnl);
}

static inline constexpr void prettyPrint(const ASTNode &node, int indent = 0) {
    // Recursively print children for Binary and Unary expression nodes
    if(const auto * binaryNode = dynamic_cast<const BinaryExpressionNode *>(&node)) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), binaryNode->getOp()), "Left");
        prettyPrint(*binaryNode->getLeft(), indent + 2);
        print_indent(indent,"Right", "");
        prettyPrint(*binaryNode->getRight(), indent + 2);
    } else if(const auto * unaryNode = dynamic_cast<const UnaryExpressionNode *>(&node)) {
        print_indent_dl(indent, "Node", FORMAT("(Type: {}, operation:\"{}\")", node.getType(), unaryNode->getOp()), "Operand");
        prettyPrint(*unaryNode->getOperand(), indent + 2);
    } else if(const auto * numberNode = dynamic_cast<const NumberNode *>(&node)) {
        print_indent(indent,"Node", FORMAT("(Type: {}, value:{})", node.getType(), numberNode->getValue()));
    } else if(const auto * variableNode = dynamic_cast<const VariableNode *>(&node)) {
        print_indent(indent,"Node", FORMAT("(Type: {}, value:{})", node.getType(), variableNode->getName()));
    }
}