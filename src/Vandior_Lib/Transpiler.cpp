#include "Vandior/Transpiler.hpp"

namespace vnd {

    Transpiler::Transpiler(std::vector<Instruction> instructions) noexcept
      : _tabs(0), _text(""), _instructions(instructions), _scope(Scope::create()), _main(false) {}

    Transpiler Transpiler::create(std::vector<Instruction> instructions) noexcept { return Transpiler(instructions); }

    /* void Transpiler::transpile() {
        this->output.open("output.cpp");
        this->write("#include <iostream>\n");
        using enum TokenType;
        using enum InstructionType;
        try {
            for(const Instruction i : this->instructions) {
                switch(i.getType()) {
                case MAIN:
                    this->writeMain(i);
                    break;
                case DECLARATION:
                case INITIALIZATION:
                    LINFO(i.toString());
                    this->writeDeclaration(i);
                }
            }
        } catch(TranspilerException &e) { LERROR("{}", e.what()); }
        output.close();
    }

    void Transpiler::write(const std::string &str) { this->output << str; }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.size() == 0) { return; }
        if(instruction.getTokens().back().getType() == TokenType::CLOSED_CURLY_BRACKETS) {
            this->write("}");
            this->closeScope();
        }
    }

    void Transpiler::openScope() {
        std::shared_ptr<Scope> newScope = std::make_shared<Scope>(this->scope);

        this->scope = newScope;
        tabs++;
    }

    void Transpiler::closeScope() {
        std::shared_ptr<Scope> oldScope = this->scope;

        this->scope = this->scope->getParent();
        oldScope->removeParent();
        tabs--;
    }

    void Transpiler::writeMain(const Instruction &instruction) {
        if(this->main) { throw TranspilerException("Main already declared", instruction); }
        this->write("int main() {");
        this->openScope();
        this->checkTrailingBracket(instruction);
        this->write("\n");
        this->main = true;
    }

    void Transpiler::writeDeclaration(const Instruction &instruction) {
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