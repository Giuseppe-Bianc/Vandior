#pragma once

#include "Token.hpp"
#include <vector>
#include <any>

namespace vnd {

    /**
     * @brief Represents an expression composed of tokens.
     * This class is responsible for creating and managing expressions
     * based on a vector of strings (text) and a specified type.
     */
    class Expression {
    public:
        /**
         * @brief Creates an Expression object.
         * This static method creates an Expression object based on the provided
         * text and type.
         * @param text The vector of strings representing the expression.
         * @param type The type of the expression.
         * @param isConst Bool indicating if the expression is constant.
         * @param value The value of a constant expression.
         * @return An Expression object.
         */
        [[nodiscard]] static Expression create(const std::vector<std::string> &text, const std::string &type, const bool isConst,
                                                const std::string &value) noexcept;

        /**
         * @brief Creates an Expression object.
         * This static method creates an Expression object based on the provided
         * text and type.
         * @param text The vector of strings representing the expression.
         * @param type The type of the expression.
         * @param isConst Bool indicating if the expression is constant.
         * @return An Expression object.
         */
        [[nodiscard]] static Expression create(const std::vector<std::string> &text, const std::string &type, const bool isConst) noexcept;

        /**
         * @brief Gets the text representation of the expression.
         * @return A string containing the text representation of the expression.
         */
        std::string getText() const noexcept;

        /**
         * @brief Gets the type of the expression.
         * @return A string containing the type of the expression.
         */
        std::string getType() const noexcept;

        bool isConst() const noexcept;

        std::string getValue() const noexcept;

    private:
        /**
         * @brief Private constructor for creating Expression objects.
         * This constructor is used internally to create Expression objects.
         * @param text The vector of strings representing the expression.
         * @param type The type of the expression.
         */
        Expression(const std::string &text, const std::string &type, const bool isConst,
                   const std::string &value) noexcept;

        std::string _text;  ///< Vector of strings representing the expression.
        std::string _type;               ///< Type of the expression
        bool _const;
        std::string _value;
    };

}  // namespace vnd