#pragma once
#include <vector>
#include <fstream>
#include <memory>
#include "Vandior/Instruction.hpp"
#include "Vandior/TranspilerException.hpp"
#include "Vandior/Scope.hpp"
#include "Vandior/ExpressionFactory.hpp"
#include "Vandior/Expression.hpp"

namespace vnd {

    class Transpiler {
    public:
        [[nodiscard]] static Transpiler create(std::vector<Instruction> instructions) noexcept;
        void transpile();
    private:
        Transpiler(std::vector<Instruction> _instructions) noexcept;
        int _tabs;
        std::string _text;
        std::ofstream _output;
        std::vector<Instruction> _instructions;
        std::shared_ptr<Scope> _scope;
        char _main;
        void checkTrailingBracket(const Instruction &instruction);
        void transpileMain(const Instruction &i);
        void transpileDeclaration(const Instruction &i);
        [[nodiscard]] std::vector<std::string_view> extractVariables(std::vector<Token>::iterator &iterator);
        void openScope() noexcept;
        void closeScope() noexcept;
        void emplaceCommaColon(const bool colon) noexcept;
    };

}  // namespace vnd