#include "Vandior/Transpiler.hpp"

namespace vnd {

    Transpiler::Transpiler(std::vector<Instruction> instructions) noexcept
      : _tabs(0), _text(""), _instructions(instructions), _scope(Scope::createMain()), _main(0) {}

    Transpiler Transpiler::create(std::vector<Instruction> instructions) noexcept { return {instructions}; }

    void Transpiler::transpile() {
        using enum TokenType;
        using enum InstructionType;
        _output.open("output.cpp");
        _text += "#include <iostream>\n";
        _text += "#include <vector>\n";
        try {
            for(const Instruction &i : _instructions) {
                _text += std::string(_tabs, '\t');
                switch(i.getLastType()) {
                case MAIN:
                    transpileMain(i);
                    break;
                case DECLARATION:
                case INITIALIZATION:
                    transpileDeclaration(i);
                    break;
                case OPEN_SCOPE:
                    _text += "{";
                    openScope();
                    checkTrailingBracket(i);
                    break;
                case CLOSE_SCOPE:
                    _text.pop_back();
                    if(_scope->isMainScope()) { throw TranspilerException("Unexpected }", i); }
                    checkTrailingBracket(i);
                    break;
                }
                _text += "\n";
            }
            if(!_scope->isMainScope()) { throw TranspilerException("Expected }", Instruction::create()); }
            _output << _text;
            _output.close();
            LINFO("Transpiling successfull");
        } catch(TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
        }
    }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.getTokens().back().getType() == TokenType::CLOSE_CUR_PARENTESIS) {
            if(_main == 1 && _scope->getParent() && _scope->getParent()->isMainScope()) {
                if(instruction.getLastType() == InstructionType::MAIN) { _text += "\n"; }
                _text += std::string(_tabs, '\t');
                _text += "return 0;\n";
                _text += std::string(_tabs - 1, '\t');
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
        _text += "int main() {";
        _main = 1;
        openScope();
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileDeclaration(const Instruction& instruction) {
        std::vector<Token> tokens = instruction.getTokens();
        std::string type;
        std::vector<Token>::iterator i = tokens.begin();
        bool isConst = i->getValue() == "const";
        std::vector<std::string_view> variables = extractVariables(i);
        ExpressionFactory factory = ExpressionFactory::create(i, tokens.end());
        type = (++i)->getValue();
        if(!_scope->checkType(type)) { throw TranspilerException(FORMAT("Type {} not valid", type), instruction); }
        while(i != tokens.end() && i->getType() != TokenType::EQUAL_OPERATOR) {
            if(i->getType() == TokenType::OPEN_SQ_PARENTESIS) { type = "std::vector<" + type + ">"; }
            i++;
        }
        _text += type;
        _text += " ";
        if(i != tokens.end() && i->getType() == TokenType::EQUAL_OPERATOR) {
            i++;
            while(i != tokens.end()) {
                factory.parse(TokenType::COMMA);
                if(i != tokens.end()) { i++; }
            }
        }
        if(isConst && variables.size() > factory.size()) {
            throw TranspilerException(FORMAT("Uninitialized constant: {} values for {} constants",
                                      factory.size(), variables.size()), instruction);
        }
        for(std::string_view j : variables) {
            _text += j;
            if(!factory.empty()) {
                _text += " =";
                _text += factory.getExpression();
            }
            auto [check, shadowing] = _scope->checkVariable(j);
            if(check) {
                if(!shadowing) { throw TranspilerException(FORMAT("{} already defined", j), instruction); }
                LWARN("{} already defined", j);
            }
            _scope->addVariable(j, type, isConst);
            emplaceCommaColon(j == variables.back());
        }
    }

    std::vector<std::string_view> Transpiler::extractVariables(std::vector<Token>::iterator &iterator) noexcept {
        using enum TokenType;
        std::vector<std::string_view> result;
        if(iterator->getValue() == "const") { _text += "const "; }
        while(iterator->getType() != COLON) {
            if(iterator->getType() == IDENTIFIER) {
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