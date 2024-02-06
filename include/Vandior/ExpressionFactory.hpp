/**ì
 * @brief Defines the ExpressionFactory class for parsing and creating expressions.
 */
#pragma once

#include "Expression.hpp"
#include "Scope.hpp"
#include "Token.hpp"

namespace vnd {

    /**
     * @brief Factory class for parsing and creating expressions from a sequence of tokens.
     */
    class ExpressionFactory {
    public:
        /**
         * @brief Creates an ExpressionFactory instance.
         * @param iterator Reference to the iterator pointing to the current position in the token sequence.
         * @param end Iterator pointing to the end of the token sequence.
         * @param scope Shared pointer to the current scope.
         * @return ExpressionFactory instance.
         */
        [[nodiscard]] static ExpressionFactory create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                      std::shared_ptr<Scope> scope) noexcept;

        /**
         * @brief Parses the token sequence until reaching the specified end tokens.
         * @param endToken Vector of token types indicating the end of parsing.
         * @return Parsed string.
         */
        [[nodiscard]] std::string parse(const std::vector<TokenType> &endToken) noexcept;

        /**
         * @brief Gets the size of the parsed text.
         * @return Size of the parsed text.
         */
        [[nodiscard]] std::size_t size() const noexcept;

        /**
         * @brief Checks if the parsed text is empty.
         * @return True if the parsed text is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const noexcept;

        /**
         * @brief Gets the parsed expression.
         * @return Parsed expression.
         */
        [[nodiscard]] Expression getExpression() noexcept;

        /**
         * @brief Gets the vector of parsed expressions.
         * @return Vector of parsed expressions.
         */
        [[nodiscard]] std::vector<Expression> getExpressions() noexcept;

    private:
        /**
         * @brief Private constructor for creating an ExpressionFactory instance.
         * @param iterator Reference to the iterator pointing to the current position in the token sequence.
         * @param end Iterator pointing to the end of the token sequence.
         * @param scope Shared pointer to the current scope.
         */
        ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                          std::shared_ptr<Scope> scope) noexcept;

        /**
         * @brief Type definition for a tuple used during parsing.
         */
        using TupType = std::tuple<bool, bool, std::string>;

        std::vector<Token>::iterator &_iterator;  ///< Iterator pointing to the current position in the token sequence.
        std::vector<Token>::iterator _end;        ///< Iterator pointing to the end of the token sequence.
        std::shared_ptr<Scope> _scope;            ///< Shared pointer to the current scope.
        std::vector<std::string> _text;           ///< Vector storing the parsed text.
        std::vector<Expression> _expressions;     ///< Vector storing the created expressions.
        int _power;                               ///< Power value used during parsing.
        bool _divide;                             ///< Flag indicating division operation during parsing.
        bool _dot;
        std::string _types;
        std::string _temp;
        /**
         * @brief Gets the type of a token.
         * @param token The token to get the type from.
         * @return Type of the token as a string.
         */
        [[nodiscard]] std::string_view getTokenType(const Token &token) const noexcept;

        /**
         * @brief Emplaces the current token into the parsed text vector.
         */
        void emplaceToken(const std::string_view &type) noexcept;

        /**
         * @brief Writes the current token to the parsed text.
         * @return Parsed string of the current token.
         */
        [[nodiscard]] std::string writeToken() noexcept;

        /**
         * @brief Handles a function during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string.
         */
        [[nodiscard]] std::string handleFun(TupType &type) noexcept;

        /**
         * @brief Handles an inner expression during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string.
         */
        [[nodiscard]] std::string handleInnerExpression(TupType &type) noexcept;

        /**
         * @brief Handles a token during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string.
         */
        [[nodiscard]] std::string handleToken(TupType &type) noexcept;

        /**
         * @brief Checks the type during parsing.
         * @param oldType Tuple representing the previous type.
         * @param newType The new type to check.
         * @return Checked type as a string.
         */
        [[nodiscard]] std::string checkType(TupType &oldType, const std::string_view newType) noexcept;

        /**
         * @brief Checks and processes operators in the parsed value.
         * @param value Parsed value to check for operators.
         */
        void checkOperators(std::string &value) noexcept;
        void write(std::string value, const std::string_view &type) noexcept;

    };

}  // namespace vnd
