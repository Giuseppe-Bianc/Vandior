//
// Created by potex02 on 14/05/2024.
//

#pragma once
#include "ASTNode.hpp"

namespace vnd {

    /**
     * @brief Node class representing type value in the AST.
     */
    class TypeNode : public ASTNode {
    public:
        /**
         * @brief Creates a TypeNode.
         * @param token The token correspondent to the node.
         */
        [[nodiscard]] explicit TypeNode(const Token &token) noexcept : ASTNode(token), m_value(token.getValue()), m_type(token.getType()) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Type; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return FORMAT("{}_{}({})", getVariableType(), getType(), get_value()); }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return FORMAT("{}({})", getType(), get_value()); }

        /**
         * @brief Gets the value of the node.
         * @return The value of the node.
         */
        [[nodiscard]] std::string_view get_value() const noexcept { return m_value; }

        /**
         * @brief Gets the variable type of the node.
         * @return The variable type of the node.
         */
        [[nodiscard]] TokenType getVariableType() const noexcept { return m_type; }

        friend void swap(TypeNode &lhs, TypeNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.m_value, rhs.m_value);
            swap(lhs.m_type, rhs.m_type);
        }

    private:
        std::string_view m_value;  ///< The value of the node.
        TokenType m_type;          ///< The type of the token node.
    };

}  // namespace vnd
