 #pragma once
#include <vector>
#include <fstream>
#include <memory>
#include "Vandior/Instruction.hpp"
#include "Vandior/TranpilerException.hpp"
#include "Vandior/Scope.hpp"

namespace vnd {

    class Transpiler {
    public:
        static Transpiler create(std::vector<Instruction> instructions) noexcept;
        void transpile() noexcept;
    private:
        Transpiler(std::vector<Instruction> _instructions) noexcept;
        int _tabs;
        std::string_view _text;
        std::ofstream _output;
        std::vector<Instruction> _instructions;
        std::shared_ptr<Scope> _scope;
        bool _main;
        void write(const std::string &str) noexcept;
        /*void checkTrailingBracket(const Instruction &instruction);
        void openScope();
        void closeScope();
        void writeMain(const Instruction &instruction);
        void writeDeclaration(const Instruction &instruction);*/
    };

}  // namespace vnd*/