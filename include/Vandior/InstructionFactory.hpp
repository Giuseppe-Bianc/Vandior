#pragma once
#include "Instruction.hpp"

namespace vnd {
    /**
     * @brief A class to that check the correctness of an instruction.
     */
    class InstructionFactory {
    public:
        /**
         * @brief Create an instruction instance.
         * @param filename String representing the filename that contains the instruction.
         * @return the created Instruction instance.
         */
        [[nodiscard]] static InstructionFactory create(const std::string_view filename, const TokenVec &tokens) noexcept;

        /**
         * @brief Check the correctness of a token.
         * @param token The token to check.
         */
        void checkToken(const Token &token);

    private:
        explicit InstructionFactory(const std::string_view filename, const TokenVec &tokens) noexcept;
        static const TokenTypeVec _expressionStartTokens;  ///< Vector of the token types that can start an expression.
        TokenVec _tokens;                                  ///< Tokens of the instructions.
        Instruction _instruction;                          ///< Current instruction.
        std::vector<Instruction> _instructions;            ///< Vector of the isntructios.
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
                                       eofTokenType};  ///< Vector of tokens that are allowed.
        std::string_view _filename;                    ///< Filename that contains the instruction.

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
         * @brief Checks an if or while keyword token.
         */
        void checkKStructure() noexcept;

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
    };

}  // namespace vnd
