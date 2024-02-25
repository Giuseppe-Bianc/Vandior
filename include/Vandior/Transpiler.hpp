#pragma once
#include "Vandior/Expression.hpp"
#include "Vandior/ExpressionFactory.hpp"
#include "Vandior/Instruction.hpp"
#include "Vandior/Scope.hpp"
#include "Vandior/TranspilerException.hpp"
#include <fstream>

namespace vnd {
    /**
     * @brief A class for transpiling instructions.
     */
    class Transpiler {
    public:
        /**
         * @brief Creates a Transpiler instance.
         * @param instructions The vector of instructions to transpile.
         * @return The created Transpiler instance.
         */
        [[nodiscard]] static Transpiler create(const std::vector<Instruction> &instructions) noexcept;

        /**
         * @brief Transpile the instructions.
         * @return Bool indicating if the transpiling is successful.
         */
        bool transpile();

    private:
        /**
         * @brief Constructs a Transpiler with a vector of instructions.
         * @param instructions The vector of instructions.
         */
        Transpiler(const std::vector<Instruction> &instructions) noexcept;

        int _tabs;                               ///< Number of tabs for indentation.
        std::string _text;                       ///< The transpiled text.
        std::ofstream _output;                   ///< Output file stream.
        std::vector<Instruction> _instructions;  ///< Vector of instructions.
        std::shared_ptr<Scope> _scope;           ///< Shared pointer to the current scope.
        bool _main;                              ///< Flag used to check main transpiling.

        /**
         * @brief Tokenize a string.
         * @param str String to tokenize.
         * @return Vector of tokenized string.
         */
        static std::vector<std::string> tokenize(const std::string &str) noexcept;

        /**
         * @brief Checks if there's a trailing bracket in the instruction.
         * @param instruction The instruction to check.
         */
        void checkTrailingBracket(const Instruction &instruction);

        /**
         * @brief Transpile the main instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileMain(const Instruction &instruction);

        /**
         * @brief Transpile a declaration instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileDeclaration(const Instruction &instruction);

        /**
         * @brief Transpile an operation instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileOperation(const Instruction &instruction);

        /**
         * @brief Transpile an assignation instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileAssignation(const Instruction &instruction);

        /**
         * @brief Transpile a structure instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileStructure(const Instruction &instruction);

        /**
         * @brief Transpile an else instruction of the program.
         * @param instruction The instruction to transpile.
         */
        void transpileElse(const Instruction &instruction);

        /**
         * @brief Extracts identifiers of declared variables from a declaration instruction.
         * @param iterator The iterator to the instruction.
         * @param instruction The instruction to extract variables from.
         * @return Vector of extracted identifiers.
         */
        [[nodiscard]] std::vector<std::string_view> extractIdentifiers(std::vector<Token>::iterator &iterator,
                                                                       const Instruction &instruction) const;

        /**
         * @brief Extracts variables from an assignation instruction.
         * @param iterator The iterator to the instruction.
         * @param end Iterator pointing to the end of the token sequence.
         * @param instruction The instruction to extract variables from.
         * @return Vector of extracted variables and their types.
         */
        [[nodiscard]] std::vector<std::pair<std::string, std::string>> extractVariables(std::vector<Token>::iterator &iterator,
                                                                                        const std::vector<Token>::iterator &end,
                                                                                        const Instruction &instruction) const;

        /**
         * @brief Extracts a token from an assignation instruction.
         * @param iterator The iterator to the instruction.
         * @param end Iterator pointing to the end of the token sequence.
         * @param next Iterator pointing to the next token.
         * @param currentvariable String containing the current extracted variable.
         * @param type String containing the type of the extracted variable.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string extractToken(std::vector<Token>::iterator &iterator, const std::vector<Token>::iterator &end,
                                               const std::vector<Token>::iterator &next, std::string &currentVariable,
                                               std::string &type) const noexcept;

        /**
         * @brief Extracts a function from an assignation instruction.
         * @param iterator The iterator to the instruction.
         * @param end Iterator pointing to the end of the token sequence.
         * @param currentvariable String containing the current extracted variable.
         * @param type String containing the type of the extracted variable.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string extractFun(std::vector<Token>::iterator &iterator, const std::vector<Token>::iterator &end,
                                             std::string &currentVariable, std::string &type) const noexcept;

        /**
         * @brief Extracts a square expression from an assignation instruction.
         * @param iterator The iterator to the instruction.
         * @param end Iterator pointing to the end of the token sequence.
         * @param currentvariable String containing the current extracted variable.
         * @param type String containing the type of the extracted variable.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string extractSquareExpression(std::vector<Token>::iterator &iterator,
                                                          const std::vector<Token>::iterator &end, std::string &currentVariable,
                                                          std::string &type) const noexcept;

        /**
         * @brief transpile a multi return value function instruction.
         * @param variables Vector of identifiers and types of assigned variables.
         * @param expressiom Expression conataining the function.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string transpileMultipleFun(const std::vector<std::pair<std::string, std::string>> &variables,
                                                       const Expression &expression) noexcept;

        /**
         * @brief Transpile a type name.
         * @param iterator Iterator to the instruction.
         * @param end Iterator pointing to the end of the token sequence.
         * @param endTokens Vector of token types indicating the end of parsing.
         * @param instruction The instruction to extract type from.
         * @return Pair that represent the type and the value to transpile.
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::pair<std::string, std::string> transpileType(std::vector<Token>::iterator &iterator,
                                                                        const std::vector<Token>::iterator &end,
                                                                        const std::vector<TokenType> &endTokens,
                                                                        const Instruction &instruction);

        /**
         * @brief Transpile an if or while condition.
         * @param iterator The iterator pointing to the condition token sequence.
         * @param end The iterator pointing to the end of the instruction
         * @return Parsed string if there is an error. If no error occurs, an empty string is returned.
         */
        [[nodiscard]] std::string transpileCondition(std::vector<Token>::iterator &iterator, const std::vector<Token>::iterator &end) noexcept;

        /**
         * @brief Opens a new scope.
         * @param type ScopeType of the scope.
         */
        void openScope(const ScopeType &type) noexcept;

        /**
         * @brief Closes the current scope.
         */
        void closeScope() noexcept;

        /**
         * @brief Emplaces comma or colon based on the condition.
         * @param colon Boolean flag indicating if colon should be emplaced.
         */
        void emplaceCommaColon(const bool colon) noexcept;

        /**
         * @brief Checks if a token has a specific value.
         * @param iterator Iterator pointing to the token.
         * @param value String containing the value.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] bool iteratorIs(const std::vector<Token>::iterator &iterator, const std::string &value) const noexcept;
    };

}  // namespace vnd