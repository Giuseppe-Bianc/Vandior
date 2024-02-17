#include "Vandior/Transpiler.hpp"

namespace vnd {
    Transpiler::Transpiler(const std::vector<Instruction> &instructions) noexcept
      : _tabs(0), _instructions(std::move(instructions)), _scope(Scope::createMain()), _main(0) {}

    Transpiler Transpiler::create(const std::vector<Instruction> &instructions) noexcept { return {std::move(instructions)}; }

    void Transpiler::transpile() {
        using enum TokenType;
        using enum InstructionType;
        _output.open("output.cpp");
        _text += "#include \"../../../base.hpp\"\n\n";
        try {
            for(const Instruction &instruction : _instructions) {
                _text += std::string(C_ST(_tabs), '\t');
                switch(instruction.getLastType()) {
                case MAIN:
                    transpileMain(instruction);
                    break;
                case DECLARATION:
                    [[fallthrough]];
                case INITIALIZATION:
                    transpileDeclaration(instruction);
                    break;
                case ASSIGNATION:
                    transpileAssignation(instruction);
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
            LINFO("Transpiling successfully");
        } catch(const TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
        }
    }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.getTokens().back().getType() == TokenType::CLOSE_CUR_PARENTESIS) {
            auto parent = _scope->getParent();
            if(_main == 1 && parent && parent->isMainScope()) {
                if(instruction.getLastType() == InstructionType::MAIN) { _text += "\n"; }
                _text += FORMAT("{:\t^{}}return 0;\n{:\t^{}}", "", C_ST(_tabs), "", C_ST(_tabs) - 1);
                _main = -1;
            }
            _text += "}";
            closeScope();
        }
    }

    void Transpiler::transpileMain(const Instruction &instruction) {
        std::string value;
        if(!_scope->isMainScope()) { throw TranspilerException("Cannot declare main here", instruction); }
        if(_main == -1) { throw TranspilerException("Main already declared", instruction); }
        if(_scope->getParent() != nullptr) { throw TranspilerException("Cannot declare main here", instruction); }
        _text += "int main(int argc, char **argv) {\n";
        _main = 1;
        openScope();
        value = FORMAT("{:\t^{}}const vnd::vector<string> _args(argv, argv + argc);", "", C_ST(_tabs));;
        _text += value;
        _scope->addConstant("args", "string[]", value);
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileDeclaration(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto isConst = iterator->getValue() == "const";
        const auto isVal = iterator->getValue() == "val";
        std::vector<std::string_view> variables = extractIdenfifiers(iterator, instruction);
        auto endToken = tokens.end();
        ExpressionFactory factory = ExpressionFactory::create(iterator, endToken, _scope, isConst);
        if (isConst || isVal) { _text += "const "; }
        auto [type, typeValue] = transpileType(iterator, tokens.end(), { TokenType::EQUAL_OPERATOR }, instruction);
        _text += FORMAT("{} ", typeValue);
        if(isConst && !Scope::isPrimitive(type)) { throw TranspilerException(FORMAT("Cannot declare const variables of {} type", type), instruction); }
        // Handle initialization
        if(iterator != endToken && iterator->getType() == TokenType::EQUAL_OPERATOR) {
            iterator++;
            while(iterator != endToken) {
                if(std::string error = factory.parse({ TokenType::COMMA }); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
                if(iterator != endToken) { iterator++; }
            }
        }
        if((isConst || isVal) && variables.size() > factory.size()) {
            throw TranspilerException(
                FORMAT("Uninitialized constant: {} values for {} constants", factory.size(), variables.size()), instruction);
        }
        for(std::string_view jvar : variables) {
            std::string value;
            if (!jvar.empty() && jvar.at(0) == '_') {
                _text += FORMAT("v{}", jvar);
            }
            else {
                _text += FORMAT("_{}", jvar);
            }
            if (!factory.empty()) {
                auto expression = factory.getExpression();
                if (!_scope->canAssign(type, expression.getType())) {
                    throw TranspilerException(FORMAT("Cannot assign {} to {}", expression.getType(), type), instruction);
                }
                if (isConst) {
                    if (expression.isConst()) {
                        value = expression.getValue();
                        if (type == "int") { value = FORMAT("{}", value.substr(0, value.find('.'))); }
                        _text += FORMAT(" = {}", value);
                    }
                    else {
                        throw TranspilerException(FORMAT("Cannot evaluate {} at compile time", jvar), instruction);
                    }
                }
                else {
                    _text += FORMAT(" = {}", expression.getText());
                }
            }
            auto [check, shadowing] = _scope->checkVariable(jvar);
            if(check) {
                if (!shadowing) { throw TranspilerException(FORMAT("{} already defined", jvar), instruction); }
                LWARN("{} already defined", jvar);
            }
            if(isConst) {
                _scope->addConstant(jvar, type, value);
            }
            else {
                _scope->addVariable(jvar, type, isVal);
            }
            emplaceCommaColon(jvar == variables.back());
        }
    }

    void Transpiler::transpileAssignation(const Instruction& instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        auto endToken = tokens.end();
        Token equalToken;
        ExpressionFactory factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        std::vector<std::pair<std::string, std::string>> variables = extractvariables(iterator, tokens.end(), instruction);
        equalToken = *iterator;
        iterator++;
        while(iterator != endToken) {
            if(std::string error = factory.parse({TokenType::COMMA}); !error.empty()) {
                throw TranspilerException(error, instruction);
            }
            if(iterator != endToken) { iterator++; }
        }
        if(variables.size() != factory.size()) {
            throw TranspilerException( FORMAT("Unconsistent assignation: {} values for {} variables", factory.size(), variables.size()), instruction);
        }
        for(auto& var : variables) {
            Expression expression = factory.getExpression();
            if(equalToken.getType() == TokenType::OPERATION_EQUAL && !Scope::isNumber(var.second)) {
                throw TranspilerException(FORMAT("incompatible operator {} for {} type", equalToken.getValue(), var.second), instruction);
            }
            if(!_scope->canAssign(var.second, expression.getType())) {
                throw TranspilerException(FORMAT("Cannot assign {} to {}", expression.getType(), var.second), instruction);
            }
            if(var.first.at(0) == '_') {
                var.first = FORMAT("v{} ", var.first);
            } else {
                var.first = FORMAT("_{}", var.first);
            }
            if(var.first.contains("->")) {
                if(equalToken.getType() == TokenType::EQUAL_OPERATOR) {
                     _text += FORMAT("{}{}); ", var.first, expression.getText());
                } else if(equalToken.getValue() == "^=") {
                    std::string getter = FORMAT("{})", var.first);
                    getter.replace(getter.find("->set"), 5, "->get");
                    _text += FORMAT("{}std::pow({},{})); ", var.first, getter, expression.getText());
                } else {
                    std::string getter = FORMAT("{})", var.first);
                    getter.replace(getter.find("->set"), 5, "->get");
                    _text += FORMAT("{}{}{}{}); ", var.first, getter, equalToken.getValue().at(0), expression.getText());
                }
            } else {
                if(equalToken.getValue() != "^=") {
                    _text += FORMAT("{} {} {}; ", var.first, equalToken.getValue(), expression.getText());
                } else {
                    _text += FORMAT("{} = std::pow({}, {}); ", var.first, var.first, expression.getText());
                }
            }
        }
    }

    std::vector<std::string_view> Transpiler::extractIdenfifiers(std::vector<Token>::iterator& iterator,
        const Instruction& instruction) const {
        using enum TokenType;
        std::vector<std::string_view> result;
        while (iterator->getType() != COLON) {
            if (iterator->getType() == IDENTIFIER) {
                if (_scope->checkType(iterator->getValue())) {
                    throw TranspilerException(FORMAT("Identifier {} not allowed", iterator->getValue()), instruction);
                }
                result.emplace_back(iterator->getValue());
            }
            iterator++;
        }
        return result;
    }

    std::vector<std::pair<std::string, std::string>> Transpiler::extractvariables(
        std::vector<Token>::iterator& iterator,
        const std::vector<Token>::iterator& end, const Instruction& instruction) const {
        using enum TokenType;
        std::vector<std::pair<std::string, std::string>> result;
        std::string currentVariable;
        std::string type;
        while(iterator != end && iterator->getType() != EQUAL_OPERATOR && iterator->getType() != OPERATION_EQUAL) {
            const std::string_view value = iterator->getValue();
            const std::vector<Token>::iterator next = iterator + 1;
            if (iterator->getType() == IDENTIFIER) {
                if (next != end && next->getType() == OPEN_PARENTESIS) {
                }
                else {
                    auto [newType, assignable] = std::make_pair(_scope->getVariableType(type, value),
                        _scope->getConstValue(type, value).empty());
                    if(next != end && (next->getType() == COMMA || next->getType() == EQUAL_OPERATOR ||
                        next->getType() == OPERATION_EQUAL)) {
                        assignable = !_scope->isConstant(type, value);
                    }
                    if(newType.empty()) { throw TranspilerException(FORMAT("Cannot find identifier {}", value), instruction); }
                    if(!assignable) { throw TranspilerException(FORMAT("Cannot assign {}.{}", type, value), instruction); }
                    type = newType;
                    if(currentVariable.empty()) {
                        currentVariable += FORMAT("{}->", value);
                    } else if(next != end && next->getType() == DOT_OPERATOR) {
                        currentVariable += FORMAT("get{}{}()->", char(std::toupper(value[0])), value.substr(1));
                    } else {
                        currentVariable += FORMAT("set{}{}(", char(std::toupper(value[0])), value.substr(1));
                    }
                }
            }
            if(iterator->getType() == UNARY_OPERATOR) {
                throw TranspilerException(FORMAT("Cannot use {} at the left side of an assignation", value), instruction);
            }
            if(iterator->getType() == COMMA) {
                if(currentVariable.ends_with("->")) {
                    currentVariable.pop_back();
                    currentVariable.pop_back();
                }
                result.emplace_back(std::make_pair(currentVariable, type));
                currentVariable.clear();
                type.clear();
            }
            iterator++;
        }
        if(currentVariable.ends_with("->")) {
            currentVariable.pop_back();
            currentVariable.pop_back();
        }
        result.emplace_back(std::make_pair(currentVariable, type));
        return result;
    }

    std::pair<std::string, std::string> Transpiler::transpileType(std::vector<Token>::iterator &iterator, const std::vector<Token>::iterator end,
                                          const std::vector<TokenType> &endTokens, const Instruction &instruction) {
        std::string type;
        std::string typeValue; 
        std::string prefix = "";
        std::string suffix = "";
        type = (++iterator)->getValue();
        typeValue = type;
        if(!_scope->checkType(type)) { throw TranspilerException(FORMAT("Type {} not valid", type), instruction); }
        if(!_scope->isPrimitive(type)) { typeValue = FORMAT("std::shared_ptr<{}>", type); }
        while(iterator != end && std::ranges::find(endTokens, iterator->getType()) == endTokens.end()) {
            if(iterator->getType() == TokenType::OPEN_SQ_PARENTESIS) {
                if((iterator + 1)->getType() == TokenType::CLOSE_SQ_PARENTESIS) {
                    prefix += "vnd::vector<";
                    suffix = ">" + suffix;
                    type += "[]";
                } else {
                    std::string size;
                    iterator++;
                    ExpressionFactory factory = ExpressionFactory::create(iterator, end, _scope, true, true);
                    if(std::string error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS, TokenType::EQUAL_OPERATOR}); error != "") {
                        throw TranspilerException(error, instruction);
                    };
                    Expression expression = factory.getExpression();
                    if(!expression.isConst()) {
                        throw TranspilerException("Canno evaluate array dimension at compile time", instruction);
                    }
                    size = expression.getValue().substr(0, expression.getValue().find('.'));
                    if(std::stoi(size) < 0) { throw TranspilerException("Array cannot have negative size", instruction); }
                    prefix += "vnd::array<";
                    suffix = FORMAT(", {}>{}", size,
                                    suffix);
                    type += FORMAT("[{}]", size);
                }
            }
            iterator++;
        }
        return {type, prefix + typeValue + suffix};
    }

    void Transpiler::openScope() noexcept {
        auto newScope = Scope::create(_scope);
        _scope = newScope;
        _tabs++;
    }

    void Transpiler::closeScope() noexcept {
        auto oldScope = _scope;
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
