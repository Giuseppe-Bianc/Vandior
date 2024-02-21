/**�
 * @brief Defines the ExpressionFactory class for parsing and creating expressions.
 */
#pragma once

#include "Expression.hpp"
#include "Scope.hpp"
#include "Token.hpp"
#include "exprtk.hpp"

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
         * @param isConst Bool indicating if it's a const expression.
         * @param sq Bool indicating if it's a square expression.
         * @return ExpressionFactory instance.
         */
        [[nodiscard]] static ExpressionFactory create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                      std::shared_ptr<Scope> scope, const bool isConst,
                                                      const bool sq = false) noexcept;

        [[nodiscard]] static std::string transpileFun(const std::vector<Expression> &expressions, std::optional<size_t> variadic) noexcept;

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
         * @param isConst Bool indicating if it's a const expression.
         * @param sq Bool indicating if it's a square expression.
         */
        ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end, std::shared_ptr<Scope> scope,
                          const bool isConst, const bool sq) noexcept;

        /**
         * @brief Type definition for a tuple used during parsing.
         */
        using TupType = std::tuple<bool, bool, std::string>;

        std::vector<Token>::iterator &_iterator;  ///< Iterator pointing to the current position in the token sequence.
        std::vector<Token>::iterator _end;        ///< Iterator pointing to the end of the token sequence.
        std::shared_ptr<Scope> _scope;            ///< Shared pointer to the current scope.
        std::vector<std::string> _text;           ///< Vector storing the parsed text.
        std::vector<Expression> _expressions;     ///< Vector storing the created expressions.
        std::optional<size_t> _power;             ///< Power value used during parsing.
        bool _divide;                             ///< Flag indicating division operation during parsing.
        bool _dot;
        bool _sq;
        bool _const;
        std::string _expressionText;
        std::string _type;
        std::string _temp;

        /**
         * @brief Checks if the type of a token is allowed for array indexing.
         * @param type String_view representing the token type.
         * @return Bool indicating if the type is allowed.
         */
        [[nodiscard]] static bool isSquareType(const std::string_view &type) noexcept;

        /**
         * @brief Gets the type of the current token.
         * @return Type of the token as a string.
         */
        [[nodiscard]] std::string_view getTokenType() noexcept;

        /**
         * @brief Emplaces the current token into the parsed text vector.
         * @param type String_view representing the type of the token.
         */
        void emplaceToken(const std::string_view &type) noexcept;

        /**
        * @brief Evaluate a compile time number expression.
        * @param expression The expression to evaluate
        * @return String containing the result.
        */
        std::string evaluate(const std::string &expression) noexcept;

        /**
         * @brief Writes the current token to the parsed text.
         * @return Parsed string of the current token.
         */
        [[nodiscard]] std::string writeToken() noexcept;

        /**
         * @brief Handles a function during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string handleFun(TupType &type) noexcept;

        /**
         * @brief Handles an inner expression during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string handleInnerExpression(TupType &type) noexcept;

        /**
         * @brief Handles an inner expression for array indexing during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string handleSquareExpression(TupType &type) noexcept;

        /**
         * @brief Handles a vector initialization.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string handleVectorInitialization(TupType &type) noexcept;

        /**
         * @brief Handles a token during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string handleToken(TupType &type) noexcept;

        /**
         * @brief Checks the type during parsing.
         * @param oldType Tuple representing the previous type.
         * @param newType The new type to check.
         * @return Checked type as a string if there is an error. If no error occurs, an empty string is returned
         */
        [[nodiscard]] std::string checkType(TupType &oldType, const std::string_view newType) noexcept;

        /**
         * @brief Checks the presence of . or [ as next token.
         * @param type String representing the type of the value.
         * @param value Bool if the next token is . or [.
         */
        [[nodiscard]] bool checkNextToken(const std::string &type, const std::string &value) noexcept;

        /**
         * @brief Checks if the current token can be followed by a ++ or --.
         * @param type String_view conatining the type of the current token.
         * @return Bool indicating if the current token can be followed by a ++ or --.
         */
        [[nodiscard]] bool checkUnaryOperator(const std::string_view &type) const noexcept;

        /**
         * @brief Checks and processes operators in the parsed value.
         * @param value Parsed value to check for operators.
         */
        [[nodiscard]] void checkOperators(std::string &value) noexcept;

        /**
         * @brief Writes the value to the parsed text.
         * @param value String to write.
         * @param type String_view representing the type of the value.
         */
        void write(const std::string &value, const std::string_view &type) noexcept;
        void resetVariables() noexcept;
        void handleFinalExpression(exprtk::expression<double> &expression, exprtk::parser<double> &parser,
                                   const std::tuple<bool, bool, std::string> &type) noexcept;
        void clearData() noexcept;
    };

}  // namespace vnd
