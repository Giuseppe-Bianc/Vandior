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

        [[nodiscard]] Instruction getInstruction() const noexcept;

        [[nodiscard]] std::vector<Instruction> getInstructions() const noexcept;

        void addInstruction() noexcept;
        
        /**
         * @brief Checks if the current instruction can terminate.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool canTerminate() const noexcept;

        /**
         * @brief Check the correctness of a token.
         * @param token The token to check.
         */
        void checkToken(const Token &token);

    private:
        explicit InstructionFactory(const std::string_view filename, const TokenVec &tokens) noexcept;
        static const TokenTypeVec _expressionStartTokens;  ///< Vector of the token types that can start an expression.
        static const TokenTypeVec _startTokens;            ///< Vector of tokens that are allowed.
        TokenVec _tokens;                                  ///< Tokens of the instructions.
        Instruction _instruction;                          ///< Current instruction.
        std::vector<Instruction> _instructions;            ///< Vector of the instructios.
        TokenTypeVec _allowedTokens;
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

         /**
         * @brief Adds an allowed token type in base at the last type of the instruction.
         * @param instruction InstructionType that must be the last type.
         * @param token TokenType to add.
         * @return Bool containing true is the token type is added.
         */
        [[nodiscard]] inline bool emplaceTokenType(const InstructionType &type, const TokenType token) noexcept;

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
