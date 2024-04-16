#pragma once
#include "InstructionException.hpp"
#include "InstructionType.hpp"
#include "Token.hpp"
#include <vector>

namespace vnd {
    /**
     * @brief A class to that check the correctness of an instruction.
     */
    class Instruction {
    public:
        /**
         * @brief Create an instruction instance.
         * @param filename String representing the filename that contains the instruction.
         * @return the created Instruction instance.
         */
        [[nodiscard]] static Instruction create(const std::string_view filename) noexcept;

        void emplaceToken(const Token& token) { _tokens.emplace_back(token); }

        /**
         * @brief Gtes the tokens of the instruction.
         * @return Vector containing the tokens of the instruction.
         */
        [[nodiscard]] TokenVec getTokens() const noexcept;
        /**
         * @brief gets the last type of the instruction.
         * @return InstructionType representing the last type of the instruction.
         */
        [[nodiscard]] InstructionType getLastType() const noexcept;
        /**
         * @brief Gets a vector containing the the string representation of the instruction types.
         * @return Vector of the instruction type.
         */
        [[nodiscard]] StringVec typeToString() const noexcept;

        /**
         * @brief Gets a representation of the instruction in form of a string.
         * @return String representing the instruction.
         */
        [[nodiscard]] std::string toString() const noexcept;
        
        /**
         * @brief Constructs an Instruction.
         * @param filename String representing the filename that contains the instruction.
         */
        explicit Instruction(const std::string_view filename) noexcept;
        static const TokenTypeVec _expressionStartTokens;  ///< Vector of the token types that can start an expression.
        TokenTypeVec _allowedTokens = {TokenType::K_MAIN,
                                       TokenType::K_VAR,
                                       TokenType::K_IF,
                                       TokenType::K_WHILE,
                                       TokenType::K_FOR,
                                       TokenType::K_FUN,
                                       TokenType::K_RETURN,
                                       TokenType::K_BREAK,
                                       TokenType::IDENTIFIER,
                                       TokenType::OPEN_CUR_PARENTESIS,
                                       TokenType::CLOSE_CUR_PARENTESIS,
                                       eofTokenType};                    ///< Vector of tokens that are allowed.
        TokenVec _tokens;                                                ///< Vector of previous tokens.
        std::vector<InstructionType> _types = {InstructionType::BLANK};  ///< Vector of the types of the instruction.
        std::vector<bool> _booleanOperators = {false};                   ///< Vector of the flags for boolean operators.
        std::string_view _filename;                                      ///< Filename that contains the instruction.

        /**
         * @brief Sets the last type of the instruction.
         * @param type InstructionType to set.
         */
        void setLastType(const InstructionType &type) noexcept;

        /**
         * @brief Adds a type at the instruction.
         * @param type InstructionType to add.
         */
        void addType(const InstructionType &type) noexcept;

        /**
         * @brief Removes the last type of the instruction.
         */
        void removeType() noexcept;

        /**
         * @brief Checks the last type of the instruction.
         * @param type InstructionType to check.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool lastTypeIs(const InstructionType &type) const noexcept;
        /**
         * @brief Checks the last types of the instruction if is any of the inputs.
         * @param types InstructionTypes to check.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool lastTypeIsAny_of(const std::initializer_list<InstructionType> &types) const noexcept;

        /**
         * @brief Gets the flag for the last boolean operator.
         * @return Bool containing the flag.
         */
        [[nodiscard]] bool getLastBooleanOperator() const noexcept;

        /**
         * @brief Sets the flag for the last boolean operator.
         * @param present Bool containing the flag to set.
         */
        void setLastBooleanOperator(const bool present) noexcept;

        /**
         * @brief Adds a flag for the boolean operator.
         */
        void addBooleanOperator() noexcept;

        /**
         * @brief Removes the last flag for the boolean operator.
         */
        void removeBooleanOperator() noexcept;

        /**
         * @brief Gets the type of the last checked token.
         * @return TokenType of the token.
         */
        [[nodiscard]] TokenType getLastTokenType() const noexcept;

        /**
         * @brief Gets the type of the last checked token and control if the type is in the types in the input.
         * @param tokenTypes the types to verify.
         * @return true if the type correspond to any of the elements in input.
         */
        [[nodiscard]] bool isLastTokenTypeAny_of(const std::initializer_list<TokenType> &tokenTypes) const noexcept;
        /**
         * @brief Checks if the the vector of the checked tokens is empty.
         * @return Bool containing the result of the check..
         */
        [[nodiscard]] bool isEmpty() const noexcept;

        /**
         * @brief Checks if the last type of the instruction is an expression.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool isExpression() const noexcept;

        /**
         * @brief Checks if the last type of the instruction is an expression inside a for instruction.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool isForExpression() const noexcept;
    };
}  // namespace vnd

/**
 * This function is a formatter for Instruction using fmt.
 * \cond
 */
// NOLINTNEXTLINE
template <> struct fmt::formatter<vnd::Instruction> : fmt::formatter<std::string_view> {
    template <typename FormatContext> auto format(const vnd::Instruction &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
/** \endcond */
