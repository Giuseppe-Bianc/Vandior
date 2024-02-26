#include "Vandior/Transpiler.hpp"
#include <sstream>

namespace vnd {
    // NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)
    Transpiler::Transpiler(const std::vector<Instruction> &instructions) noexcept
      : _tabs(0), _instructions(instructions), _scope(Scope::createMain()), _main(0) {}

    Transpiler Transpiler::create(const std::vector<Instruction> &instructions) noexcept { return {instructions}; }

    std::vector<std::string> Transpiler::tokenize(const std::string &str) noexcept {
        std::vector<std::string> result;
        std::istringstream iss(str);
        std::string strr;
        while(iss >> strr) { result.emplace_back(std::move(strr)); }
        return result;
    }

    bool Transpiler::transpile() {
        using enum TokenType;
        using enum InstructionType;
        _output.open("output.cpp");
        _text += R"(#include "../../../base.hpp")";
        try {
            for(const auto &instruction : _instructions) {
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
                case OPERATION:
                    transpileOperation(instruction);
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
                    if(_scope->isMainScope()) { throw TranspilerException("Unexpected '}'", instruction); }
                    checkTrailingBracket(instruction);
                    break;
                default:
                    break;
                }
                _text += "\n";
            }
            if(!_scope->isMainScope()) { throw TranspilerException("Expected '}'", Instruction::create("")); }
            _output << _text;
            _output.close();
            LINFO("Transpiling successfully");
        } catch(const TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
            return false;
        }
        return true;
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
        if(!_scope->isMainScope()) { throw TranspilerException("Cannot declare main here", instruction); }
        if(_main == -1) { throw TranspilerException("Main already declared", instruction); }
        if(_scope->getParent() != nullptr) { throw TranspilerException("Cannot declare main here", instruction); }
        _text += "\nint main(int argc, char **argv) {\n";
        _main = 1;
        openScope();
        auto value = FORMAT("{:\t^{}}const vnd::vector<string> _args(argv, argv + argc);", "", C_ST(_tabs));
        _text += value;
        _scope->addConstant("args", "string[]", value);
        checkTrailingBracket(instruction);
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    void Transpiler::transpileDeclaration(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto isConst = iteratorIs(iterator, "const");
        const auto isVal = iteratorIs(iterator, "val");
        auto variables = extractIdentifiers(iterator, instruction);
        const auto endToken = tokens.end();
        auto factory = ExpressionFactory::create(iterator, tokens.end(), _scope, isConst);
        if(isConst || isVal) { _text += "const "; }
        auto [type, typeValue] = transpileType(iterator, tokens.end(), {TokenType::EQUAL_OPERATOR}, instruction);
        _text += FORMAT("{} ", typeValue);
        if(isConst && !Scope::isPrimitive(type)) {
            throw TRANSPILER_EXCEPTIONF(instruction, "Cannot declare const variables of {} type", type);
        }
        // Handle initialization
        if(iterator != endToken && iterator->getType() == TokenType::EQUAL_OPERATOR) {
            iterator++;
            while(iterator != endToken) {
                if(auto error = factory.parse({TokenType::COMMA}); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
                if(iterator != endToken) { iterator++; }
            }
        }
        if((isConst || isVal) && variables.size() > factory.size()) {
            throw TRANSPILER_EXCEPTIONF(instruction, "Uninitialized constant: {} values for {} constants", factory.size(),
                                        variables.size());
        }
        for(const auto &jvar : variables) {
            std::string value;
            if(!jvar.empty() && jvar.at(0) == '_') {
                _text += FORMAT("v{}", jvar);
            } else {
                _text += FORMAT("_{}", jvar);
            }
            if(!factory.empty()) {
                auto expression = factory.getExpression();
                if(!_scope->canAssign(type, expression.getType())) {
                    throw TRANSPILER_EXCEPTIONF(instruction, "Cannot assign {} to {}", expression.getType(), type);
                }
                if(isConst) {
                    if(expression.isConst()) [[likely]] {
                        value = expression.getValue();
                        if(type == "int") { value = FORMAT("{}", value.substr(0, value.find('.'))); }
                        _text += FORMAT(" = {}", value);
                    } else [[unlikely]] {
                        throw TRANSPILER_EXCEPTIONF(instruction, "Cannot evaluate {} at compile time", jvar);
                    }
                } else {
                    _text += FORMAT(" = {}", expression.getText());
                }
            }
            auto [check, shadowing] = _scope->checkVariable(jvar);
            if(check) {
                if(!shadowing) { throw TranspilerException(FORMAT("{} already defined", jvar), instruction); }
                LWARN("{} already defined", jvar);
            }
            if(isConst) {
                _scope->addConstant(jvar, type, value);
            } else {
                _scope->addVariable(jvar, type, isVal);
            }
            emplaceCommaColon(jvar == variables.back());
        }
    }
    bool Transpiler::iteratorIs(const TokenVecIter &iterator, const std::string &value) const noexcept {
        return iterator->getValue() == value;
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    void Transpiler::transpileAssignation(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        std::vector<std::string> tmp;
        Token equalToken;
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        auto variables = extractVariables(iterator, endToken, instruction);
        equalToken = *iterator;
        iterator++;
        for(auto &var : variables) {
            if(var.first != "_") {
                auto typeValue = Scope::getTypeValue(var.second);
                auto key = _scope->addTmp(var.first, var.second);
                tmp.push_back(FORMAT("std::any_cast<{}>(vnd::tmp[\"{}\"])", typeValue, key));
            }
        }
        while(iterator != endToken) {
            if(std::string error = factory.parse({TokenType::COMMA}); !error.empty()) {
                throw TranspilerException(error, instruction);
            }
            if(iterator != endToken) { iterator++; }
        }
        if(factory.isMultiplefun()) {
            if(equalToken.getType() == TokenType::OPERATION_EQUAL) {
                throw TRANSPILER_EXCEPTIONF(instruction, "incompatible operator {}", equalToken.getValue());
            }
            if(auto error = transpileMultipleFun(variables, factory.getExpression()); !error.empty()) {
                throw TranspilerException(error, instruction);
            }
            return;
        }
        _scope->eachTmp([this](const std::string &key) -> void {
            _text += FORMAT("vnd::tmp[\"{}\"] = {};\n{:\t^{}}", key, key, "", C_ST(_tabs));
        });
        if(variables.size() != factory.size()) {
            throw TRANSPILER_EXCEPTIONF(instruction, "inconsistent assignation: {} values for {} variables", factory.size(),
                                        variables.size());
        }
        for(auto &var : variables) {
            auto expression = factory.getExpression();
#ifdef __llvm__
            bool exprContainsSpace = expression.getType().find(' ') != std::string::npos;
#else
            bool exprContainsSpace = expression.getType().contains(' ');
#endif
            if(exprContainsSpace) {
                throw TranspilerException("Multiple return value functions must be used alone", instruction);
            }
            if(equalToken.getType() == TokenType::OPERATION_EQUAL && !Scope::isNumber(var.second)) {
                if(var.first == "_") {
                    throw TRANSPILER_EXCEPTIONF(instruction, "incompatible operator {} for blank identifier",
                                                equalToken.getValue(), var.second);
                }
                throw TRANSPILER_EXCEPTIONF(instruction, "incompatible operator {} for {} type", equalToken.getValue(),
                                            var.second);
            }
            if(var.first == "_") {
                if(expression.getType() == "void") {
                    throw TranspilerException("Cannot assign void expression", instruction);
                } else {
                    _text += FORMAT("{};\n{:\t^{}}", expression.getText(), "", C_ST(_tabs));
                    continue;
                }
            }
            if(!_scope->canAssign(var.second, expression.getType())) {
                throw TRANSPILER_EXCEPTIONF(instruction, "Cannot assign {} to {}", expression.getType(), var.second);
            }
            std::string text = var.first;
            if(!var.first.ends_with('(')) { text += " = "; }
            if(equalToken.getType() == TokenType::OPERATION_EQUAL) {
                if(equalToken.getValue() == "^=") {
                    text += FORMAT("std::pow({},", tmp.front());
                } else {
                    text += FORMAT("{} {}", tmp.front(), equalToken.getValue().at(0));
                }
            }
            text += expression.getText();
            if(equalToken.getValue() == "^=") { text += ")"; }
            if(var.first.ends_with('(')) { text += ")"; }
            tmp.erase(tmp.begin());
            _text += FORMAT("{};\n{:\t^{}}", text, "", C_ST(_tabs));
        }
        _text += "vnd::tmp.clear();";
        _scope->clearTmp();
    }

    void Transpiler::transpileOperation(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        std::vector<Expression> expressions;
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        while(iterator != endToken) {
            if(auto error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
            if(iterator != endToken) { iterator++; }
        }
        for(const auto &expression : factory.getExpressions()) {
            std::string text = expression.getText();
            if(expression.getType() != "void" && !(text.ends_with("++") || text.ends_with("--"))) {
                throw TRANSPILER_EXCEPTIONF(instruction, "Invalid operation {}", text);
            }
            _text += FORMAT("{};\n{:\t^{}}", text, "", C_ST(_tabs));
        }
        _text.erase(_text.size() - C_ST(_tabs) - 1, C_ST(_tabs) + 1);
    }

    std::vector<std::string_view> Transpiler::extractIdentifiers(TokenVecIter &iterator, const Instruction &instruction) const {
        using enum TokenType;
        std::vector<std::string_view> result;
        while(iterator->getType() != COLON) {
            if(iterator->getType() == IDENTIFIER) {
                if(_scope->checkType(iterator->getValue())) {
                    throw TRANSPILER_EXCEPTIONF(instruction, "Identifier {} not allowed", iterator->getValue());
                }
                if(iterator->getValue() == "_") {
                    throw TranspilerException("Cannot declare variables using blank identifier", instruction);
                }
                result.emplace_back(iterator->getValue());
            }
            iterator++;
        }
        return result;
    }

    std::vector<std::pair<std::string, std::string>> Transpiler::extractVariables(TokenVecIter &iterator, const TokenVecIter &end,
                                                                                  const Instruction &instruction) const {
        using enum TokenType;
        std::vector<std::pair<std::string, std::string>> result;
        std::string currentVariable;
        std::string type;
        while(iterator != end && iterator->getType() != EQUAL_OPERATOR && iterator->getType() != OPERATION_EQUAL) {
            const auto next = std::next(iterator);
            if(iterator->getType() == IDENTIFIER) {
                if(next != end && next->getType() == OPEN_PARENTESIS) {
                    if(auto error = extractFun(iterator, end, currentVariable, type); !error.empty()) {
                        throw TranspilerException(error, instruction);
                    }
                } else if(auto error = extractToken(iterator, end, next, currentVariable, type); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
            } else if(iterator->getType() == OPEN_SQ_PARENTESIS) {
                if(auto error = extractSquareExpression(iterator, end, currentVariable, type); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
            } else if(iterator->getType() == UNARY_OPERATOR) {
                throw TRANSPILER_EXCEPTIONF(instruction, "Cannot use {} at the left side of an assignation",
                                            iterator->getValue());
            } else if(iterator->getType() == COMMA) {
                if(currentVariable.ends_with("->")) {
                    currentVariable.pop_back();
                    currentVariable.pop_back();
                }
                result.emplace_back(currentVariable, type);
                currentVariable.clear();
                type.clear();
            }
            iterator++;
        }
        if(currentVariable.ends_with("->")) {
            currentVariable.pop_back();
            currentVariable.pop_back();
        }
        result.emplace_back(currentVariable, type);
        return result;
    }

    std::string Transpiler::extractToken(TokenVecIter &iterator, const TokenVecIter &end, const TokenVecIter &next,
                                         std::string &currentVariable, std::string &type) const noexcept {
        using enum TokenType;
        auto value = iterator->getValue();
        if(value == "_") {
            if(currentVariable.empty() && (next == end || next->getType() == COMMA || next->getType() == EQUAL_OPERATOR ||
                                           next->getType() == OPERATION_EQUAL)) {
                currentVariable = "_";
                type = "";
                return {};
            } else {
                return "Cannot use blank identifier here";
            }
        }
        auto [newType, assignable] = std::make_pair(_scope->getVariableType(type, value),
                                                    _scope->getConstValue(type, value).empty());
        if(next != end && (next->getType() == COMMA || next->getType() == EQUAL_OPERATOR || next->getType() == OPERATION_EQUAL)) {
            assignable = !_scope->isConstant(type, value);
        }
        if(newType.empty()) { return FORMAT("Cannot find identifier {}.{}", type, value); }
        if(!assignable) { return FORMAT("Cannot assign {}.{}", type, value); }
        type = newType;
        if(currentVariable.empty()) {
            currentVariable += FORMAT("{}->", value);
            if(currentVariable.at(0) == '_') {
                currentVariable = FORMAT("v{}", currentVariable);
            } else {
                currentVariable = FORMAT("_{}", currentVariable);
            }
        } else if(next != end && next->getType() == DOT_OPERATOR) {
            currentVariable += FORMAT("get{}{}()->", char(std::toupper(value[0])), value.substr(1));
        } else {
            currentVariable += FORMAT("set{}{}(", char(std::toupper(value[0])), value.substr(1));
        }
        return {};
    }

    std::string Transpiler::extractFun(TokenVecIter &iterator, const TokenVecIter &end, std::string &currentVariable,
                                       std::string &type) const noexcept {
        auto identifier = iterator->getValue();
        auto factory = ExpressionFactory::create(iterator, end, _scope, false);
        std::vector<Expression> expressions;
        std::string params;
        iterator++;
        while(iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            iterator++;
            if(iterator->getType() != TokenType::CLOSE_PARENTESIS) {
                if(auto error = factory.parse({TokenType::COMMA, TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
            }
        }
        expressions = factory.getExpressions();
        auto [newType, constructor, variadic] = _scope->getFunType(type, identifier, expressions);
        if(newType.empty()) {
            std::string value;
            std::string paramTypes;
            for(const auto &expression : expressions) { paramTypes += expression.getType() + ","; }
            if(!paramTypes.empty()) { paramTypes.pop_back(); }
            value = FORMAT("{}.{}({})", type, identifier, paramTypes);
            LINFO(value);
            if(value.starts_with(".")) { value.erase(0, 1); }
            return FORMAT("Function {} not found", value);
        }
        type = newType;
        params = ExpressionFactory::transpileFun(expressions, variadic);
        if(constructor) {
            currentVariable += FORMAT("std::make_shared<{}>({})->", newType, params);
        } else {
            const bool empty = currentVariable.empty();
            currentVariable += FORMAT("{}({})->", identifier, params);
            if(empty) {
                if(currentVariable.at(0) == '_') {
                    currentVariable = FORMAT("v{} ", currentVariable);
                } else {
                    currentVariable = FORMAT("_{}", currentVariable);
                }
            }
        }
        return {};
    }

    std::string Transpiler::extractSquareExpression(TokenVecIter &iterator, const TokenVecIter &end, std::string &currentVariable,
                                                    std::string &type) const noexcept {
        std::string newType;
        if(currentVariable.ends_with("->")) {
            currentVariable.pop_back();
            currentVariable.pop_back();
        }
        if(!Scope::checkVector(type)) { return FORMAT("Indexing not allowed for {} type", type); }
        auto factory = ExpressionFactory::create(iterator, end, _scope, false, true);
        iterator++;
        if(auto error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        newType = expression.getType();
        if(newType != "int") { return FORMAT("{} index not allowed", newType); }
        currentVariable += FORMAT(".at({})->", expression.getText().substr(1));
        return {};
    }

    std::string Transpiler::transpileMultipleFun(const std::vector<std::pair<std::string, std::string>> &variables,
                                                 const Expression &expression) noexcept {
        auto types = Transpiler::tokenize(expression.getType());
        std::string values;
        std::vector<std::string> tmp;
        std::size_t typeIndex = 0;
        if(variables.size() != types.size()) {
            return FORMAT("Inconsistent assignation: {} return values for {} variables", types.size(), variables.size());
        }
        for(size_t i = 0; i != types.size(); i++) {
            values += FORMAT("vnd::tmp[\"{}\"], ", i);
            tmp.emplace_back(FORMAT("vnd::tmp.at(\"{}\")", i));
        }
        values.pop_back();
        values.pop_back();
        _text += FORMAT("std::tie({}) = {};\n{:\t^{}}", values, expression.getText(), "", C_ST(_tabs));
        for(const auto &[first, second] : variables) {
            if(first != "_") {
                auto type = types.at(typeIndex);
                auto typeValue = Scope::getTypeValue(type);
                if(!_scope->canAssign(second, type)) { return FORMAT("Cannot assign {}.{}", type, first); }
                typeIndex++;
                if(first.ends_with('(')) {
                    _text += FORMAT("{}std::any_cast<{}>({}));\n{:\t^{}}", first, typeValue, tmp.front(), "", C_ST(_tabs));
                } else if(first != "_") {
                    _text += FORMAT("{} = std::any_cast<{}>({});\n{:\t^{}}", first, typeValue, tmp.front(), "", C_ST(_tabs));
                }
            }
            tmp.erase(tmp.begin());
        }
        _text += "vnd::tmp.clear();";
        _scope->clearTmp();
        return {};
    }

    std::pair<std::string, std::string> Transpiler::transpileType(TokenVecIter &iterator, const TokenVecIter &end,
                                                                  const std::vector<TokenType> &endTokens,
                                                                  const Instruction &instruction) {
        std::string type;
        std::string typeValue;
        std::string prefix;
        std::string suffix;
        type = (++iterator)->getValue();
        typeValue = type;
        if(typeValue == "void" || typeValue == "any") {
            throw TranspilerException(FORMAT("Cannot declare {} variables", typeValue), instruction);
        }
        if(!_scope->checkType(type)) { throw TranspilerException(FORMAT("Type {} not valid", type), instruction); }
        if(!_scope->isPrimitive(type)) { typeValue = FORMAT("std::shared_ptr<{}>", type); }
        while(iterator != end && std::ranges::find(endTokens, iterator->getType()) == endTokens.end()) {
            if(iterator->getType() == TokenType::OPEN_SQ_PARENTESIS) {
                if((iterator + 1)->getType() == TokenType::CLOSE_SQ_PARENTESIS) {
                    prefix += "vnd::vector<";
                    suffix = FORMAT(">{}", suffix);
                    type += "[]";
                } else {
                    std::string size;
                    iterator++;
                    ExpressionFactory factory = ExpressionFactory::create(iterator, end, _scope, true, true);
                    if(auto error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS, TokenType::EQUAL_OPERATOR}); !error.empty()) {
                        throw TranspilerException(error, instruction);
                    };
                    Expression expression = factory.getExpression();
                    if(!expression.isConst()) {
                        throw TranspilerException("Cannot evaluate array dimension at compile time", instruction);
                    }
                    size = expression.getValue().substr(0, expression.getValue().find('.'));
                    if(std::stoi(size) < 0) { throw TranspilerException("Array cannot have negative size", instruction); }
                    prefix += "vnd::array<";
                    suffix = FORMAT(", {}>{}", size, suffix);
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
    // NOLINTEND(*-include-cleaner, *-easily-swappable-parameters)
}  // namespace vnd
