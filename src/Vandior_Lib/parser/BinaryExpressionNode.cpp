//
// Created by gbian on 30/04/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/parser/BinaryExpressionNode.hpp"

namespace vnd {
    BinaryExpressionNode::BinaryExpressionNode(std::string_view _op, const Token &op_Token, std::unique_ptr<ASTNode> _left,
                                               std::unique_ptr<ASTNode> _right) noexcept
      : ASTNode(op_Token), op(_op), left(vnd_move_always_even_const(_left)), right(vnd_move_always_even_const(_right)) {
        if(left) { left->set_parent(this); }
        if(right) { right->set_parent(this); }
    }

    BinaryExpressionNode::BinaryExpressionNode(const Token &op_Token, std::unique_ptr<ASTNode> _left,
                                               std::unique_ptr<ASTNode> _right) noexcept
      : ASTNode(op_Token), op(op_Token.getValue()), left(vnd_move_always_even_const(_left)), right(vnd_move_always_even_const(_right)) {
        if(left) { left->set_parent(this); }
        if(right) { right->set_parent(this); }
    }

    NodeType BinaryExpressionNode::getType() const noexcept { return NodeType::BinaryExpression; }

    std::string BinaryExpressionNode::print() const {
        return FORMAT("{}(op:\"{}\" left:{}, right:{})", getType(), op, left->print(), right->print());
    }

    std::string BinaryExpressionNode::comp_print() const {
        return FORMAT("BINE(op:\"{}\" l:{}, r:{})", op, left->comp_print(), right->comp_print());
    }

    std::string_view BinaryExpressionNode::getOp() const noexcept { return op; }

    const std::unique_ptr<ASTNode> &BinaryExpressionNode::getLeft() const noexcept { return left; }

    const std::unique_ptr<ASTNode> &BinaryExpressionNode::getRight() const noexcept { return right; }
    const ASTNode &BinaryExpressionNode::getLeftr() const noexcept { return *left; }
    const ASTNode &BinaryExpressionNode::getRightr() const noexcept { return *right; }

}  // namespace vnd

// NOLINTEND(*-include-cleaner)