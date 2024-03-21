/**�
 * @brief Defines the ExpressionFactory class for parsing and creating expressions.
 */
#pragma once

#include "Expression.hpp"
#include "Scope.hpp"
#include "Token.hpp"

namespace vnd {
    using TokenVecIter = std::vector<Token>::iterator;

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
        [[nodiscard]] static ExpressionFactory create(TokenVecIter &iterator, const TokenVecIter &end, std::shared_ptr<Scope> scope,
                                                      const bool isConst, const bool sq = false) noexcept;

        /**
         * @brief Creates the text for the arguments of a function.
         * @param expressions Vector of the expressions passed as arguments to the function.
         * @param variadic Optional indicating the position of the first variadic argument, if present.
         * @return the transpiled text for the arguments.
         */
        [[nodiscard]] static std::string transpileFun(const std::vector<Expression> &expressions, std::optional<size_t> variadic) noexcept;

        /**
         * @brief Parses the token sequence until reaching the specified end tokens.
         * @param endToken Vector of token types indicating the end of parsing.
         * @return Parsed string.
         */
        [[nodiscard]] std::string parse(const std::vector<TokenType> &endToken) noexcept;

        /**
         * @brief Gets the size of the vector of parsed expressions.
         * @return Size of the vector of parsed expressions.
         */
        [[nodiscard]] std::size_t size() const noexcept;

        /**
         * @brief Checks if the size of the vector of parsed expressions is empty.
         * @return True if the vector is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const noexcept;

        /**
         * @brief Gets the first parsed expression removing it from the vector.
         * @return Parsed expression.
         */
        [[nodiscard]] Expression getExpression() noexcept;

        /**
         * @brief Gets the vector of parsed expressions clearing it.
         * @return Vector of parsed expressions.
         */
        [[nodiscard]] std::vector<Expression> getExpressions() noexcept;

        /*
         * @brief Checks if the factory contains only one expression and that this is a multiple return value function.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] bool isMultiplefun() const noexcept;

    private:
        /**
         * @brief Private constructor for creating an ExpressionFactory instance.
         * @param iterator Reference to the iterator pointing to the current position in the token sequence.
         * @param end Iterator pointing to the end of the token sequence.
         * @param scope Shared pointer to the current scope.
         * @param isConst Bool indicating if it's a const expression.
         * @param sq Bool indicating if it's a square expression.
         */
        ExpressionFactory(TokenVecIter &iterator, const TokenVecIter &end, std::shared_ptr<Scope> scope, const bool isConst,
                          const bool sq) noexcept;

        /**
         * @brief Type definition for a tuple used during parsing for representing the current expression type.
         */
        using TupType = std::tuple<bool, bool, std::string>;
        TokenVecIter &_iterator;                 ///< Iterator pointing to the current position in the token sequence.
        TokenVecIter _end;                       ///< Iterator pointing to the end of the token sequence.
        std::shared_ptr<Scope> _scope;           ///< Shared pointer to the current scope.
        std::vector<std::string> _text{};        ///< Vector storing the parsed text.
        std::vector<Expression> _expressions{};  ///< Vector storing the parsed expressions.
        std::optional<size_t> _operators;        ///< Optional value used to parse ^ and % operators.
        bool _divide = false;                    ///< Flag indicating division operation during parsing.
        bool _dot = false;                       ///< Flag used for indicate the presence of . token.
        bool _const;                             ///< Flag used to indicate if it's a const expression.
        bool _sq;                                ///< Flag used to indicate if it's a square expression.
        std::string _expressionText;             ///< String containing the text to evaluate a constant expression.
        std::string _type;                       ///< String containing the temp type of the operand.
        std::string _temp;                       ///< String containing the temp value of the operand.

        /**
         * @brief Checks if the type of a token is allowed for array indexing.
         * @param type String_view representing the token type.
         * @return Bool indicating if the type is allowed.
         */
        [[nodiscard]] static bool isSquareType(const std::string_view &type) noexcept;

        /**
         * @brief Gets the type of a literal integer value.
         * @param type String_view representing the token value.
         * @return String_view indicating the type.
         */
        [[nodiscard]] static std::string_view getIntType(const std::string_view &value) noexcept;

        /**
         * @brief Gets the type of the current token.
         * @return Type of the token as a string.
         */
        [[nodiscard]] std::string_view getTokenType() const noexcept;

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
        std::string evaluate(const std::string &expression) const noexcept;

        /**
         * @brief Writes the current token to the parsed text.
         * @return Parsed string of the current token.
         */
        [[nodiscard]] std::string writeToken() noexcept;

        /**
         * @brief Handles a function during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string handleFun(TupType &type) noexcept;

        /**
         * @brief Handles an inner expression during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string handleInnerExpression(TupType &type) noexcept;

        /**
         * @brief Handles an inner expression for array size or indexing during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string handleSquareExpression(TupType &type) noexcept;

        /**
         * @brief Handles a vector initialization.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string handleVectorInitialization(TupType &type) noexcept;

        /**
         * @brief Handles a token during parsing.
         * @param type Tuple representing the type information.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string handleToken(TupType &type) noexcept;

        /**
         * @brief Checks the type during parsing.
         * @param oldType Tuple representing the previous type.
         * @param newType The new type to check.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string checkType(TupType &oldType, const std::string_view newType) noexcept;

        /**
         * @brief Checks the presence of . or [ as next token.
         * @param type String representing the type of the current token.
         * @param value String representing the value of the current token.
         * @param value Bool if the next token is . or [.
         */
        [[nodiscard]] bool checkNextToken(const std::string_view &type, const std::string &value) noexcept;

        /**
         * @brief Checks if the current token can be followed by a ++ or --.
         * @param type String_view containing the type of the current token.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] bool checkUnaryOperator(const std::string_view &type) const noexcept;

        /**
         * @brief Checks and processes operators in the parsed value.
         * @param value Parsed value to check for operators.
         */
        void checkOperators(std::string &value) noexcept;

        /**
         * @brief Writes the value to the parsed text vector.
         * @param value String to write.
         * @param type String_view representing the type of the value.
         */
        void write(const std::string &value, const std::string_view &type) noexcept;

        /**
         * @brief Resets the value of variables used for parsing.
         */
        void resetVariables() noexcept;

        /**
         * @brief Handles the final part of expression parsing creating the expression object.
         * @param type Tuple representing the type information.
         */
        void handleFinalExpression(const TupType &type) noexcept;

        /**
         * @brief Clears the _type and _temp properties.
         */
        void clearData() noexcept;
        [[nodiscard]] bool isTokenOfType(const TokenVecIter &iterator, TokenType type) const noexcept;
        [[nodiscard]] std::string handleLogicalType(TupType &oldType) const noexcept;
        [[nodiscard]] std::string handleBooleanType(TupType &oldType) const noexcept;
        [[nodiscard]] bool isEnd(const TokenVecIter &nxtIter) const noexcept;
        bool oldAndNewCoincide(const std::pair<char, std::string> &oldParts, const std::pair<char, std::string_view> &newParts,
                               char cha) const;
        template <typename T> static bool isLessThanOrEqualToMax(uint64_t number) noexcept;
    };

}  // namespace vnd
