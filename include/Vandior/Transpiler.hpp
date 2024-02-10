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
         */
        void transpile();

    private:
        /**
         * @brief Constructs a Transpiler with a vector of instructions.
         * @param _instructions The vector of instructions.
         */
        Transpiler(const std::vector<Instruction> &_instructions) noexcept;

        int _tabs;                               ///< Number of tabs for indentation.
        std::string _text;                       ///< The transpiled text.
        std::ofstream _output;                   ///< Output file stream.
        std::vector<Instruction> _instructions;  ///< Vector of instructions.
        std::shared_ptr<Scope> _scope;           ///< Shared pointer to the current scope.
        char _main;                              ///< Main character.

        /**
         * @brief Checks if there's a trailing bracket in the instruction.
         * @param instruction The instruction to check.
         */
        void checkTrailingBracket(const Instruction &instruction);

        /**
         * @brief Transpile the main part of the instruction.
         * @param i The instruction to transpile.
         */
        void transpileMain(const Instruction &i);

        /**
         * @brief Transpile the declaration part of the instruction.
         * @param i The instruction to transpile.
         */
        void transpileDeclaration(const Instruction &i);

        /**
         * @brief Extracts variables from the instruction.
         * @param iterator The iterator to the instruction.
         * @param instruction The instruction to extract variables from.
         * @return Vector of extracted variables.
         */
        [[nodiscard]] std::vector<std::string_view> extractVariables(std::vector<Token>::iterator &iterator,
                                                                     const Instruction &instruction);
        /**
         * @brief Opens a new scope.
         */
        void openScope() noexcept;

        /**
         * @brief Closes the current scope.
         */
        void closeScope() noexcept;

        /**
         * @brief Emplaces comma or colon based on the condition.
         * @param colon Boolean flag indicating if colon should be emplaced.
         */
        void emplaceCommaColon(const bool colon) noexcept;
    };

}  // namespace vnd