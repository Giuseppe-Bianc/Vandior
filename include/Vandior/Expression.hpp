#pragma once

#include "Token.hpp"
#include <any>
#include <vector>

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

        /**
         * @brief Gets a flag that indicates if the expression is constant.
         * @return Bool that indicates if the expression is constant.
         */
        bool isConst() const noexcept;

        /**
         * Gets the compile time evaluated value of the expression.
         * @return A string containing the value of the expression.
         */
        std::string getValue() const noexcept;

    private:
        /**
         * @brief Private constructor for creating Expression objects.
         * This constructor is used internally to create Expression objects.
         * @param text The String representing the text of the expression.
         * @param type The type of the expression.
         * @param isConst Bool indicating if the expression is constant.
         * @param value The value of a constant expression.
         */
        Expression(const std::string &text, const std::string &type, const bool isConst,
                   const std::string &value) noexcept;

        std::string _text;  ///< String representing the text of the expression.
        std::string _type;  ///< Type of the expression.
        bool _const;        ///< A flag that indicates if the expression is constant.
        std::string _value; ///< String representing the compile time evaluated value of a constant expression.
    };

}  // namespace vnd