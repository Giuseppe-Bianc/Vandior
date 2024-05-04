//
// Created by gbian on 29/04/2024.
//

#pragma once

#include "NodeType.hpp"
#include "Token.hpp"

namespace vnd {

    /**
     * @brief Base class for Abstract Syntax Tree (AST) nodes.
     *
     * This class serves as the base for all AST nodes in the system.
     */
    class ASTNode {
    public:
        explicit ASTNode(const Token &token) : m_token(token) {}

        virtual ~ASTNode() = default;
        /**
         * @brief Gets the type of the AST node.
         * @return NodeType enumeration value.
         */
        [[nodiscard]] virtual NodeType getType() const = 0;
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
        /**
         * @brief Converts this ASTNode to the specified type.
         * @tparam T The type to convert the ASTNode to.
         * @return A pointer to the specified type if conversion is possible; otherwise, returns nullptr.
         */
        template <typename T> T *as() {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) { return dynamic_cast<T *>(this); }
            return nullptr;
        }

        /**
         * @brief Converts this ASTNode to the specified type.
         * @tparam T The type to convert the ASTNode to.
         * @return A constant pointer to the specified type if conversion is possible; otherwise, returns nullptr.
         */
        template <typename T> const T *as() const {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) { return dynamic_cast<const T *>(this); }
            return nullptr;
        }

        [[nodiscard]] const Token &get_token() const noexcept { return m_token; }

    private:
        Token m_token;
    };
}  // namespace vnd
