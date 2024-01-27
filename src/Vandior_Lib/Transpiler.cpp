#include "Vandior/Transpiler.hpp"

namespace vnd {

    Transpiler::Transpiler(std::vector<Instruction> instructions) noexcept
      : _tabs(0), _text(""), _instructions(instructions), _scope(Scope::createMain()), _main(false) {}

    Transpiler Transpiler::create(std::vector<Instruction> instructions) noexcept { return {instructions}; }

    void Transpiler::transpile() {
        using enum TokenType;
        using enum InstructionType;
        _output.open("output.cpp");
        write("#include <iostream>\n");
        write("#include <vector>\n");
        try {
            for(const Instruction &i : _instructions) {
                _text = "";
                switch(i.getLastType()) {
                case MAIN:
                    transpileMain(i);
                    break;
                case DECLARATION:
                case INITIALIZATION:
                    transpileDeclaration(i);
                    break;
                }
                write(_text + "\n");
            }
        } catch(TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
        }
        _output.close();
    }

    void Transpiler::write(const std::string &str) noexcept { _output << str; }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.getTokens().back().getType() == TokenType::CLOSE_CUR_PARENTESIS) {
            _text += "}";
            closeScope();
        }
    }

    void Transpiler::transpileMain(const Instruction &instruction) {
        if(_main) { throw TranspilerException("Main already declared", instruction); }
        if(_scope->getParent() != nullptr) { throw TranspilerException("Cannot declare main here", instruction); }
        _text += "int main() {";
        openScope();
        this->checkTrailingBracket(instruction);
        _main = true;
    }

    void Transpiler::transpileDeclaration(const Instruction& instruction) {
        std::vector<Token> tokens = instruction.getTokens();
        std::string_view type;
        std::vector<Token>::iterator i = tokens.begin();
        std::vector<std::string_view> variables = transpileDeclarationIndentifiers(i);
        type = (++i)->getValue();
        _text += type;
        _text += " ";
        for(std::string_view j : variables) {
            _text += j;
            if(j != variables.back()) {
                _text += ", ";
            } else {
                _text += ";";
            }
        }
    }

    std::vector<std::string_view> Transpiler::transpileDeclarationIndentifiers(std::vector<Token>::iterator &iterator) {
        using enum TokenType;
        std::vector<std::string_view> result;
        if(iterator->getValue() == "const") { _text += "const "; }
        while(iterator->getType() != COLON) {
            if(iterator->getType() == IDENTIFIER) {
                // this->scope->checkVariable(i->getValue());
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

    /*void Transpiler::writeDeclaration(const Instruction &instruction) {
        using enum TokenType;

        std::vector<Token> tokens = instruction.getTokens();
        std::vector<std::string> variables;
        std::string current;
        std::vector<Token>::iterator i = tokens.begin();

        while(i->getType() != COLON) {
            if(i->getType() == IDENTIFIER) {
                this->scope->checkVariable(i->getValue());
                variables.emplace_back(i->getValue());
            }
            i++;
        }
        for(std::string k : variables) { LINFO(k); }
        i++;
        if(i->getType() != IDENTIFIER) { throw TranspilerException("Expected type", instruction); }
        LINFO("{} {}", i->getValue(), this->scope->checkType(i->getValue()));
    }*/

}  // namespace vnd*/