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
    const std::unique_ptr<ASTNode> &getLeft() const { return std::move(left); }
    const std::unique_ptr<ASTNode> &getRight() const { return std::move(right); }

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
    const std::unique_ptr<ASTNode> &getOperand() const { return std::move(operand); }

private:
    std::string_view op;
    std::unique_ptr<ASTNode> operand;
};

// Number node
class NumberNode : public ASTNode {
public:
    explicit NumberNode(int value) : value(C_D(value)) {}
    explicit NumberNode(double value) : value(value) {}

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

static inline void prettyPrint(const ASTNode &node, int indent = 0) {
    // Recursively print children for Binary and Unary expression nodes
    if(const auto *binaryNode = dynamic_cast<const BinaryExpressionNode *>(&node)) {
        LINFO("{: ^{}}Node: (Type: {}, operation:\"{}\") ","",indent,node.getType(), binaryNode->getOp());
        LINFO("{: ^{}}Left:","",indent);
        prettyPrint(*binaryNode->getLeft(), indent + 2);
        LINFO("{: ^{}}Right:","",indent);
        prettyPrint(*binaryNode->getRight(), indent + 2);
    } else if(const auto *unaryNode = dynamic_cast<const UnaryExpressionNode *>(&node)) {
        LINFO("{: ^{}}Node: (Type: {}, operation:\"{}\") ","",indent,node.getType(), unaryNode->getOp());
        LINFO("{: ^{}}Operand:","",indent);
        prettyPrint(*unaryNode->getOperand(), indent + 2);
    } else if (const auto *numberNode = dynamic_cast<const NumberNode*>(&node)){
        LINFO("{: ^{}}Node: (Type: {}, value:{}) ","",indent,node.getType(), numberNode->getValue());
    } else if(const auto*variableNode = dynamic_cast<const VariableNode*>(&node)){
        LINFO("{: ^{}}Node: (Type: {}, value:{}) ","",indent,node.getType(), variableNode->getName());
    }
}