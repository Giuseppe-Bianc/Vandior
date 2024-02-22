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
        /**
         * @brief Gtes the tokens of the instruction.
         * @return Vector containing the tokens of the instruction.
         */
        [[nodiscard]] std::vector<Token> getTokens() const noexcept;
        /**
         * @brief gets the last type of the instruction.
         * @return InstructionType representing the last type of the instruction.
         */
        [[nodiscard]] InstructionType getLastType() const noexcept;
        /**
         * @brief Gets a vector containing the the string representation of the instruction types.
         * @return Vector of the instruction type.
         */
        [[nodiscard]] std::vector<std::string> typeToString() const noexcept;
        /**
         * @brief Check the correctness of a token.
         * @param token The token to check.
         */
        void checkToken(const Token &token);
        /**
         * @brief Checks if the instruction can terminate.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool canTerminate() const noexcept;
        /**
         * @brief Gets a representation of the instruction in form of a string.
         * @return String representing the instruction.
         */
        [[nodiscard]] std::string toString() const noexcept;

    private:
        /**
         * @brief Constructs an Instruction.
         * @param filename String representing the filename that contains the instruction.
         */
        Instruction(const std::string_view filename) noexcept;
        static const std::vector<TokenType> _expressionStartTokens; ///< Vector of the token types that can start an expression.
        std::vector<TokenType> _allowedTokens;                      ///< Vector of tokens that are allowed.
        std::vector<Token> _tokens;                                 ///< Vector of previous tokens.
        std::vector<InstructionType> _types;                        ///< Vector of the types of the instruction.
        std::vector<bool> _booleanOperators;                        ///< Vector of the flags for boolean operators.
        std::string_view _filename;                                 ///< Filename that contains the instruction.

        /**
         * @brief Checks an identifier or unary operator token.
         * @param type TokenType of the token.
         */
        void checkIdentifier(const TokenType &type) noexcept;

        /**
         * @brief Checks a literal value token.
         * @param type TokenType of the token.
         */
        void checkValue(const TokenType &type) noexcept;

        /**
         * @brief Checks an operator token.
         * @param type TokenType of the token.
         */
        void checkOperator(const TokenType &type) noexcept;

        /**
         * @brief Checks an operator token.
         */
        void checkEqualOperator() noexcept;
    
        /**
         * @brief Checks a boolean or logical operator token.
         * @param type TokenType of the token.
         */
        void checkBooleanLogicalOperator(const TokenType &type) noexcept;

        /**
         * @brief Checks a comma token.
         */
        void checkComma() noexcept;

        /**
         * @brief Checks an open parentesis token.
         * @param type TokenType of the token.
         */
        void checkOpenParentesis(const TokenType &type) noexcept;

        /**
         * @brief Checks an closed parentesis token.
         * @param type TokenType of the token.
         */
        void checkClosedParentesis(const TokenType &type) noexcept;

        /**
         * @brief Checks an open curly parentesis token.
         */
        void checkOpenCurParentesis() noexcept;

        /**
         * @brief Checks an closed curly parentesis token.
         */
        void checkClosedCurParentesis() noexcept;

        /**
         * @brief Checks a main keyword token.
         */
        void checkKMain() noexcept;

        /**
         * @brief Checks a var keyword token.
         */
        void checkKVar() noexcept;

        /**
         * @brief Checks a structure keyword token.
         * @param value String_view containing the value of the token.
         */
        void checkKStructure(const std::string_view &value) noexcept;

        /**
         * @brief Checks an else keyword parentesis token.
         */
        void checkKElse() noexcept;

        /**
         * @brief Checks a for keyword token.
         */
        void checkKFor() noexcept;

        /**
         * @brief Checks a fun keyword token.
         */
        void checkKFun() noexcept;

        /**
         * @brief Checks a return keyword token.
         */
        void checkKReturn() noexcept;

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
        [[nodiscard]] inline bool lastTypeIs(const InstructionType &type) const noexcept;

        /**
         * @brief Gets the flag for the last boolean operator.
         * @return Bool containing the flag.
         */
        [[nodiscard]] inline bool getLastBooleanOperator() const noexcept;

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
         * @brief Checks if the the vector of the checked tokens is empty.
         * @return Bool containing the result of the check..
         */
        [[nodiscard]] inline bool isEmpty() const noexcept;

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

        /**
         * @brief Adds an allowed token type in base at the last type of the instruction.
         * @param instruction InstructionType that must be the last type.
         * @param token TokenType to add.
         * @return Bool containing true is the token type is added.
         */
        [[nodiscard]] inline bool emplaceTokenType(const InstructionType &instruction, const TokenType token) noexcept;

        /**
         * @brief Adds the token types that can start an expression to the allowed types.
         */
        void emplaceExpressionTokens() noexcept;

        /**
         * @brief Adds the comma and eoft token types to the allowed types.
         */
        inline void emplaceCommaEoft() noexcept;

        /**
         * @brief Adds the boolean operator token type to the allowed types if not already present in the instruction.
         */
        inline void emplaceBooleanOperator() noexcept;

        /**
         * @brief Adds the for instruction related token types to the allowed types.
         */
        inline bool emplaceForTokens() noexcept;

        /**
         * @brief Adds the unary operator related token types to the allowed types.
         */
        inline void emplaceUnaryOperator(const TokenType &type) noexcept;

    };
}  // namespace vnd

template <> struct fmt::formatter<vnd::Instruction> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const vnd::Instruction &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
