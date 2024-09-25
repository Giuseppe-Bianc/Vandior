//
// Created by gbian on 30/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "ASTNode.hpp"
#include "IndexNode.hpp"

DISABLE_WARNINGS_PUSH(4625 4626 4820 26445)

namespace vnd {

    /**
     * @brief Variable node class representing variable names in the AST.
     */
    class VariableNode : public ASTNode {
    public:
        /**
         * @brief Constructor for VariableNode.
         * @param _name Name of the variable.
         * @param name_Token token of the name of the variable.
         */
        explicit VariableNode(std::string_view _name, const Token &name_Token) noexcept
          : ASTNode(name_Token), name(_name), m_is_call(false), m_index(nullptr), m_call(nullptr) {}

        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Variable; }

        [[nodiscard]] std::string print() const override { return FORMAT("{}({})", getType(), name); }
        [[nodiscard]] std::string comp_print() const override { return FORMAT("VAR({})", name); }
        [[nodiscard]] const std::string_view &getName() const noexcept { return name; }

        [[nodiscard]] bool is_call() const noexcept { return m_is_call; }

        /**
         * @brief Gets the index node of the node.
         * @return The index node of the node.
         */
        [[nodiscard]] const std::unique_ptr<IndexNode> &get_index() const noexcept { return m_index; }

        /**
         * @brief Sets the index node of the node.
         * @param index The index node of the node.
         */
        void set_index(std::unique_ptr<IndexNode> index) noexcept {
            m_index = vnd_move_always_even_const(index);
            if(m_index) { m_index->set_parent(this); }
        }

        /**
         * @brief Gets the call node of the node.
         * @return The call node of the node.
         */
        [[nodiscard]] const std::unique_ptr<ASTNode> &get_call() const noexcept { return m_call; }

        /**
         * @brief Sets the call node of the node.
         * @param call The call node of the node.
         */
        void set_call(std::unique_ptr<ASTNode> call = nullptr) noexcept {
            m_is_call = true;
            m_call = vnd_move_always_even_const(call);
            if(m_call) { m_call->set_parent(this); }
        }

        friend void swap(VariableNode &lhs, VariableNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.name, rhs.name);
            swap(lhs.m_index, rhs.m_index);
        }

    private:
        std::string_view name;               ///< The name of the variable.
        bool m_is_call;                      ///< A bool indicates if the indentifier is a call to a function.
        std::unique_ptr<IndexNode> m_index;  ///< The possible index node of an array type.
        std::unique_ptr<ASTNode> m_call;     ///< The possible call node of an array type.
    };

}  // namespace vnd
DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner)
