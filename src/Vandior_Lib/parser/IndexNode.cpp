//
// Created by potex02 on 06/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/parser/IndexNode.hpp"

namespace vnd {
    IndexNode::IndexNode(std::unique_ptr<ASTNode> elements, const Token &token) noexcept
      : ASTNode(token), m_elements(vnd_move_always_even_const(elements)), m_index(nullptr), m_array(nullptr) {
        if(m_elements) { m_elements->set_parent(this); }
    }

    NodeType IndexNode::getType() const noexcept { return NodeType::Index; }

    std::string IndexNode::print() const {
        if(m_elements) { return FORMAT("{}({})", getType(), m_elements->comp_print()); }
        return FORMAT("{}()", getType());
    }

    std::string IndexNode::comp_print() const { return FORMAT("{}", getType()); }

    const std::unique_ptr<ASTNode> &IndexNode::get_elements() const noexcept { return m_elements; }

    const std::unique_ptr<IndexNode> &IndexNode::get_index() const noexcept { return m_index; }

    void IndexNode::set_index(std::unique_ptr<IndexNode> index) noexcept {
        m_index = vnd_move_always_even_const(index);
        if(m_index) { m_index->set_parent(this); }
    }

    const std::unique_ptr<ArrayNode> &IndexNode::get_array() const noexcept { return m_array; }

    void IndexNode::set_array(std::unique_ptr<ArrayNode> _array) noexcept {
        m_array = vnd_move_always_even_const(_array);
        if(m_array) { m_array->set_parent(this); }
    }

}  // namespace vnd
   // NOLINTEND(*-include-cleaner)