//
// Created by potex02 on 20/09/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "ASTNode.hpp"

namespace vnd {

    class Statement : public ASTNode {
    public:
        [[nodiscard]] explicit Statement(const Token &token, const std::vector<std::string> &_funData) noexcept
          : ASTNode(token), funData(_funData) {}

        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] NodeType getType() const noexcept override { return NodeType::Statement; }

        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override { return ""; }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override { return ""; }

        /**
         * @brief Returns the vector of the statment nodes.
         * @return The vector of the statment nodes.
         */
        [[nodiscard]] const std::vector<std::unique_ptr<ASTNode>> &get_nodes() const noexcept { return std::move(nodes); }

        /**
         * @brief Returns the vector of the function return types.
         * @return The vector of the function return types. If the statmement is not a function, an empty vector is returned.
         */
        [[nodiscard]] const std::vector<std::string> &get_funData() const noexcept { return funData; }

        friend void swap(Statement &lhs, Statement &rhs) noexcept {
            using std::swap;
            swap(static_cast<ASTNode &>(lhs), static_cast<ASTNode &>(rhs));
            swap(lhs.nodes, rhs.nodes);
        }

        void addNode(std::unique_ptr<ASTNode> node) { nodes.emplace_back(std::move(node)); }

    private:
        std::vector<std::unique_ptr<ASTNode>> nodes;
        std::vector<std::string> funData;
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)