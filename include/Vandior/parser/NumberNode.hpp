//
// Created by gbian on 30/04/2024.
//

#pragma once
#include "LiteralNode.hpp"
#include "NumberNodeType.hpp"

/**
 * This function is a formatter for complex value using fmt.
 * \cond
 */
/**
 * @brief Specialization of fmt::formatter for std::complex.
 */
template <typename T> struct fmt::formatter<std::complex<T>> : fmt::formatter<std::string_view> {
    /**
     * @brief Formats the std::complex for printing.
     * @param nodeType The value to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(std::complex<T> num, FormatContext &ctx) {
        using enum NumberNodeType;
        std::string_view name = FORMAT("({}, {})", std::real(num), std::imag(num));
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */

template <typename T> struct is_complex_t : public std::false_type {};
template <typename T> struct is_complex_t<std::complex<T>> : public std::true_type {};

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
        [[nodiscard]] NumberNode(T value, const Token &token, NumberNodeType number_type)
          : LiteralNode<T>(value, token, NodeType::Number), m_number_type(number_type) {}

        /**
         * @brief Gets the number type of the AST node.
         * @return NumberNodeType enumeration value.
         */
        [[nodiscard]] virtual NumberNodeType getNumberType() const { return m_number_type; };
        /**
         * @brief Returns a string representation of the AST node.
         * @return String representation of the AST node.
         */
        [[nodiscard]] std::string print() const override {
            if(is_complex_t<T>()) { return FORMAT("{}_{}(img)", LiteralNode<T>::getType(), getNumberType()); }
            return FORMAT("{}_{}({})", LiteralNode<T>::getType(), getNumberType(), LiteralNode<T>::get_value());
        }

        /**
         * @brief Returns a compact string representation of the AST node for compilation purposes.
         * @return Compact string representation of the AST node.
         */
        [[nodiscard]] std::string comp_print() const override {
            if(is_complex_t<T>()) { return FORMAT("{}_{}(img)", LiteralNode<T>::getType(), getNumberType()); }
            return FORMAT("NUM_{}({})", NumNodeType_comp(getNumberType()), LiteralNode<T>::get_value());
        }

        friend void swap(NumberNode &lhs, NumberNode &rhs) noexcept {
            using std::swap;
            swap(static_cast<NumberNode &>(lhs), static_cast<NumberNode &>(rhs));
            swap(lhs.m_number_type, rhs.m_number_type);
        }

    private:
        NumberNodeType m_number_type;
    };

}  // namespace vnd
