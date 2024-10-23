//
// Created by potex02 on 10/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/parser/ArrayNode.hpp"

namespace vnd {
    ArrayNode::ArrayNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept
      : ASTNode(token), m_elements(vnd_move_always_even_const(elements)) {
        if(m_elements) { m_elements->set_parent(this); }
    }

    NodeType ArrayNode::getType() const noexcept { return NodeType::Array; }

    std::string ArrayNode::print() const {
        if(m_elements) { return FORMAT("{}({})", getType(), m_elements->comp_print()); }
        return FORMAT("{}()", getType());
    }

    std::string ArrayNode::comp_print() const { return FORMAT("{}", getType()); }

    const std::unique_ptr<ASTNode> &ArrayNode::get_elements() const noexcept { return m_elements; }
}  // namespace vnd
   // NOLINTEND(*-include-cleaner)