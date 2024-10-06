//
// Created by gbian on 29/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../lexer/Token.hpp"
#include "CompNodeType.hpp"

namespace vnd {

    /**
     * @brief Base class for Abstract Syntax Tree (AST) nodes.
     *
     * This class serves as the base for all AST nodes in the system.
     */
    class ASTNode {  // NOLINT(*-special-member-functions)
    public:
        /**
         * @brief Constructor for ASTNode.
         * @param token The token associated with this AST node.
         */
        [[nodiscard]] explicit ASTNode(const Token &token) noexcept : m_token(token), parent(nullptr) {}

        /**
         * @brief Destructor for ASTNode.
         */
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
        template <typename T> [[nodiscard]] T *as() noexcept {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) { return dynamic_cast<T *>(this); }
            return nullptr;
        }

        /**
         * @brief Converts this ASTNode to the specified type.
         * @tparam T The type to convert the ASTNode to.
         * @return A constant pointer to the specified type if conversion is possible; otherwise, returns nullptr.
         */
        template <typename T> [[nodiscard]] const T *as() const noexcept {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) { return dynamic_cast<const T *>(this); }
            return nullptr;
        }

        /**
         * @brief Safely converts this ASTNode to the specified type.
         * @tparam T The type to convert the ASTNode to.
         * @return A pointer to the specified type if conversion is possible; otherwise, returns nullptr.
         */
        template <typename T> [[nodiscard]] T *safe_as() {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) {
                return dynamic_cast<T *>(this);
            } else {
                LERROR("Type {} is not derived from ASTNode.", typeid(T).name());
            }
            return nullptr;
        }

        /**
         * @brief Safely converts this ASTNode to the specified type.
         * @tparam T The type to convert the ASTNode to.
         * @return A constant pointer to the specified type if conversion is possible; otherwise, returns nullptr.
         */
        template <typename T> [[nodiscard]] const T *safe_as() const {
            // Check if the type T is a derived type of ASTNode
            if(std::is_base_of_v<ASTNode, T>) {
                return dynamic_cast<const T *>(this);
            } else {
                LERROR("Type {} is not derived from ASTNode.", typeid(T).name());
            }
            return nullptr;
        }

        /**
         * @brief Gets the token associated with this AST node.
         * @return The token.
         */
        [[nodiscard]] const Token &get_token() const noexcept { return m_token; }

        /**
         * @brief Get the parent node of the current ASTNode.
         *
         * @return The parent node of the current ASTNode.
         */
        [[nodiscard]] ASTNode *get_parent() const noexcept { return parent; }
        /**
         * @brief Sets the parent of the ASTNode.
         *
         * This function sets the parent of the ASTNode to the given pointer.
         *
         * @param p A pointer to the parent ASTNode.
         */
        void set_parent(ASTNode *p) noexcept { parent = p; }

        /**
         * @brief Swaps two ASTNode objects.
         * @param lhs The first ASTNode.
         * @param rhs The second ASTNode.
         */
        friend void swap(ASTNode &lhs, ASTNode &rhs) noexcept {
            std::swap(lhs.m_token, rhs.m_token);
            std::swap(lhs.parent, rhs.parent);
        }

    private:
        Token m_token;
        ASTNode *parent;  // Parent node reference
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)