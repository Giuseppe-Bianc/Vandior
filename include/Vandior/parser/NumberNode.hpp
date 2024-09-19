//
// Created by gbian on 30/04/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "LiteralNode.hpp"
#include "NumberNodeType.hpp"

namespace vnd {

    /**
     * @brief generic Number node class representing numeric values in the AST.
     */
    template <typename T> class NumberNode : public LiteralNode<T> {
    public:
        /**
         * @brief Creates a NumberNode.
         * @param value The value of the node.
         * @param token The token correspondent to the node.
         * @param number_type NumberNodeType of the node.
         */
        [[nodiscard]] NumberNode(T value, const Token &token, NumberNodeType number_type) noexcept
          : LiteralNode<T>(value, token, NodeType::Number), m_number_type(number_type) {}

        /**
         * @brief Gets the number type of the AST node.
         * @return NumberNodeType enumeration value.
         */
        [[nodiscard]] virtual NumberNodeType getNumberType() const noexcept { return m_number_type; };
        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override {
            return FORMAT("{}_{}({})", LiteralNode<T>::getType(), getNumberType(), LiteralNode<T>::get_value());
        }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override {
            return FORMAT("NUM_{}({})", NumNodeType_comp(getNumberType()), LiteralNode<T>::get_value());
        }

        friend void swap(NumberNode &lhs, NumberNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<LiteralNode<T> &>(lhs), static_cast<LiteralNode<T> &>(rhs));
            swap(lhs.m_number_type, rhs.m_number_type);
        }

    private:
        NumberNodeType m_number_type;
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)

#define VND_NUM_INT vnd::NumberNode<int>
#define VND_NUM_FLOAT vnd::NumberNode<float>
#define VND_NUM_DOUBLE vnd::NumberNode<double>
#define VND_NUM_CFLOAT vnd::NumberNode<std::complex<float>>
#define VND_NUM_CDOUBLE vnd::NumberNode<std::complex<double>>
