//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "ASTNode.hpp"
#include "NumberNodeType.hpp"
namespace vnd {

    /**
     * @brief Number node class representing numeric values in the AST.
     */
    class NumberNode : public ASTNode {
    public:
        [[nodiscard]] explicit NumberNode(const Token &m_token) : ASTNode(m_token) {}
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Number; }
        [[nodiscard]] virtual NumberNodeType getNumberType() const = 0;
    };

}  // namespace vnd
