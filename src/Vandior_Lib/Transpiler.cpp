#include "Vandior/Transpiler.hpp"

namespace vnd {
    Transpiler::Transpiler(std::vector<Instruction> instructions) noexcept
      : _tabs(0), _instructions(std::move(instructions)), _scope(Scope::createMain()), _main(0) {}

    Transpiler Transpiler::create(std::vector<Instruction> instructions) noexcept { return {std::move(instructions)}; }

    void Transpiler::transpile() {
        using enum TokenType;
        using enum InstructionType;
        _output.open("output.cpp");
        _text += "#include <iostream>\n";
        _text += "#include <cmath>\n";
        _text += "#include <vector>\n";
        _text += "class string : private std::string { public: string(): std::string(\"\") {}; string(std::string s): std::string(s) {}; int _size() { return this->size(); } };\n";
        _text += "int _test() {return 0;}\n";
        _text += "int _testPar(int a, int b) {return a + b;}\n";
        _text += "int _testPar(string s) {return s._size();}\n";
        _text += "class Object { public: int _a; string _s; float _f(float b) { return std::pow(b, 2); }; string _fs() { return string(); } };\n";
        _text += "Object _createObject() { return Object(); }\n";
        try {
            for(const Instruction &instruction : _instructions) {
                _text += std::string(C_ST(_tabs), '\t');
                switch(instruction.getLastType()) {
                case MAIN:
                    transpileMain(instruction);
                    break;
                case DECLARATION:
                case INITIALIZATION:
                    transpileDeclaration(instruction);
                    break;
                case OPEN_SCOPE:
                    _text += "{";
                    openScope();
                    checkTrailingBracket(instruction);
                    break;
                case CLOSE_SCOPE:
                    _text.pop_back();
                    if(_scope->isMainScope()) { throw TranspilerException("Unexpected }", instruction); }
                    checkTrailingBracket(instruction);
                    break;
                default:
                    break;
                }
                _text += "\n";
            }
            if(!_scope->isMainScope()) { throw TranspilerException("Expected }", Instruction::create("")); }
            _output << _text;
            _output.close();
            LINFO("Transpiling successfull");
        } catch(const TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
        }
    }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.getTokens().back().getType() == TokenType::CLOSE_CUR_PARENTESIS) {
            if(_main == 1 && _scope->getParent() && _scope->getParent()->isMainScope()) {
                if(instruction.getLastType() == InstructionType::MAIN) { _text += "\n"; }
                _text += FORMAT("{:\t^{}}return 0;\n{:\t^{}}", "", C_ST(_tabs), "", C_ST(_tabs) - 1);
                _main = -1;
            }
            _text += "}";
            closeScope();
        }
    }

    void Transpiler::transpileMain(const Instruction &instruction) {
        if(!_scope->isMainScope()) { throw TranspilerException("Cannot declare main here", instruction); }
        if(_main == -1) { throw TranspilerException("Main already declared", instruction); }
        if(_scope->getParent() != nullptr) { throw TranspilerException("Cannot declare main here", instruction); }
        _text += "int main(int argc, char **argv) {\n";
        _main = 1;
        openScope();
        _text += FORMAT("{:\t^{}}std::vector<string> _args(argv, argv + argc);", "", C_ST(_tabs));
        _scope->addVariable("args", "std::vector<string>", true);
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileDeclaration(const Instruction &instruction) {  // NOLINT(*-function-cognitive-complexity)
        std::vector<Token> tokens = instruction.getTokens();
        std::string type;
        auto iterator = tokens.begin();
        bool isConst = iterator->getValue() == "const";
        std::vector<std::string_view> variables = extractVariables(iterator, instruction);
        ExpressionFactory factory = ExpressionFactory::create(iterator, tokens.end(), _scope);
        type = (++iterator)->getValue();
        if(!_scope->checkType(type)) { throw TranspilerException(FORMAT("Type {} not valid", type), instruction); }
        while(iterator != tokens.end() && iterator->getType() != TokenType::EQUAL_OPERATOR) {
            if(iterator->getType() == TokenType::OPEN_SQ_PARENTESIS) { type = FORMAT("std::vector<{}>", type); }
            iterator++;
        }
        _text += FORMAT("{} ", type);
        if(iterator != tokens.end() && iterator->getType() == TokenType::EQUAL_OPERATOR) {
            iterator++;
            while(iterator != tokens.end()) {
                if(std::string error = factory.parse({TokenType::COMMA}); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
                if(iterator != tokens.end()) { iterator++; }
            }
        }
        if(isConst && variables.size() > factory.size()) {
            throw TranspilerException(
                FORMAT("Uninitialized constant: {} values for {} constants", factory.size(), variables.size()), instruction);
        }
        for(std::string_view jvar : variables) {
            _text += FORMAT("_{}", jvar);
            if(!factory.empty()) {
                Expression expression = factory.getExpression();
                if(!Scope::canAssign(type, expression.getType())) {
                    throw TranspilerException(FORMAT("Cannot assign {} to {}", expression.getType(), type), instruction);
                }
                _text += FORMAT(" ={}", expression.getText());
            }
            auto [check, shadowing] = _scope->checkVariable(jvar);
            if(check) {
                if(!shadowing) { throw TranspilerException(FORMAT("{} already defined", jvar), instruction); }
                LWARN("{} already defined", jvar);
            }
            _scope->addVariable(jvar, type, isConst);
            emplaceCommaColon(jvar == variables.back());
        }
    }

    std::vector<std::string_view> Transpiler::extractVariables(std::vector<Token>::iterator &iterator, const Instruction &instruction) {
        using enum TokenType;
        std::vector<std::string_view> result;
        if(iterator->getValue() == "const") { _text += "const "; }
        while(iterator->getType() != COLON) {
            if(iterator->getType() == IDENTIFIER) {
                if(_scope->checkType(iterator->getValue())) {
                    throw TranspilerException(FORMAT("Indentifier {} not allowed", iterator->getValue()), instruction);
                }
                result.emplace_back(iterator->getValue());
            }
            iterator++;
        }
        return result;
    }

    void Transpiler::openScope() noexcept {
        std::shared_ptr<Scope> newScope = Scope::create(_scope);
        _scope = newScope;
        _tabs++;
    }

    void Transpiler::closeScope() noexcept {
        std::shared_ptr<Scope> oldScope = _scope;
        _scope = _scope->getParent();
        oldScope->removeParent();
        _tabs--;
    }

    void Transpiler::emplaceCommaColon(const bool colon) noexcept {
        if(colon) {
            _text += ";";
            return;
        }
        _text += ", ";
    }
}  // namespace vnd
