#include "Vandior/Transpiler.hpp"
namespace vnd {
    // NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)

    // Types aren't allowed in global scope
    static inline constexpr std::initializer_list<InstructionType> disallowedTypesInGlobalScope = {
        InstructionType::OPERATION,     InstructionType::ASSIGNATION, InstructionType::STRUCTURE,
        InstructionType::FOR_CONDITION, InstructionType::FOR_STEP,    InstructionType::OPEN_SCOPE};
    Transpiler::Transpiler(const std::vector<Instruction> &instructions) noexcept : _instructions(instructions) {}

    Transpiler Transpiler::create(const std::vector<Instruction> &instructions) noexcept { return Transpiler{instructions}; }

    StringVec Transpiler::tokenize(const std::string &str) noexcept {
        StringVec result;
        std::istringstream iss(str);
        std::string strr;
        while(iss >> strr) { result.emplace_back(std::move(strr)); }
        return result;
    }

    bool Transpiler::checkGlobalScope(const InstructionType &type) noexcept {
        return std::ranges::none_of(disallowedTypesInGlobalScope, [&](InstructionType typ) { return typ == type; });
    }

    void Transpiler::printPrecisionLossWarning(const Instruction &instruction, bool loss, const std::string &left,
                                               const std::string &right) noexcept {
        if(loss) { LWARN("{}\nPossible loss of precision from {} to {}", instruction.toString(), left, right); }
    }

    std::pair<bool, std::string> Transpiler::transpile(std::string filename) {
        using enum TokenType;
        using enum InstructionType;
        auto filenameP = std::filesystem::path(filename);

        if(filenameP.extension().string() != ".vn") { return {false, {}}; }
        LINFO("transpiling from {}", filenameP);
        auto newfilenameP = filenameP.parent_path() / "vnbuild" / "src" / filenameP.stem();
        LINFO("transpiling to {}", FORMAT("{}.cpp", newfilenameP));
        _output.open(FORMAT("{}.cpp", newfilenameP));
        filename = newfilenameP.string();
        _text += "#include <base/base.hpp>\n\n";
        try {
            for(const auto &instruction : _instructions) {
                const auto type = instruction.getLastType();
                if(_scope->isGlobalScope() && !Transpiler::checkGlobalScope(type)) {
                    throw TranspilerException("Cannot place this instruction in the global scope", instruction);
                }
                _text += std::string(_tabs, '\t');
                switch(type) {
                case MAIN:
                    transpileMain(instruction);
                    _text += "\n";
                    break;
                case DECLARATION:
                    [[fallthrough]];
                case INITIALIZATION:
                    transpileDeclaration(instruction);
                    _text += "\n";
                    break;
                case OPERATION:
                    transpileOperation(instruction);
                    _text += "\n";
                    break;
                case ASSIGNATION:
                    transpileAssignation(instruction);
                    _text += "\n";
                    break;
                case STRUCTURE:
                    transpileStructure(instruction);
                    _text += "\n";
                    break;
                case ELSE:
                    transpileElse(instruction);
                    _text += "\n";
                    break;
                case FOR_CONDITION:
                    [[fallthrough]];
                case FOR_STEP:
                    transpileFor(instruction);
                    _text += "\n";
                    break;
                case BREAK:
                    transpileBreak(instruction);
                    _text += "\n";
                    break;
                case OPEN_SCOPE:
                    _text += "{";
                    openScope(ScopeType::SCOPE);
                    checkTrailingBracket(instruction);
                    _text += "\n";
                    break;
                case CLOSE_SCOPE:
                    _text.pop_back();
                    if(_scope->isGlobalScope()) { throw TranspilerException("Unexpected '}'", instruction); }
                    checkTrailingBracket(instruction);
                    _text += "\n";
                    break;
                case RETURN_DEFINITION:
                    transpileDefinition(instruction);
                    _text += "\n";
                    break;
                case RETURN_EXPRESSION:
                    transpileReturn(instruction);
                    _text += "\n";
                    break;
                default:
                    break;
                }
            }
            if(!_scope->isGlobalScope()) { throw TranspilerException("Expected '}'", Instruction::create("")); }
            _output << _text;
            _output.close();
            LINFO("Transpiling successfully");
        } catch(const TranspilerException &e) {
            LERROR("{}", e.what());
            _output.close();
            return {false, {}};
        }
        return {true, filename};
    }

    void Transpiler::checkTrailingBracket(const Instruction &instruction) {
        if(instruction.getTokens().back().isType(TokenType::CLOSE_CUR_PARENTESIS)) {
            if(_scope->getType() == ScopeType::MAIN_SCOPE) {
                if(instruction.getLastType() == InstructionType::MAIN) { _text += "\n"; }
                _text += FORMAT("{:\t^{}}return 0;\n{:\t^{}}", "", _tabs, "", _tabs - 1);
            }
            _text += "}";
            closeScope();
        }
    }

    void Transpiler::transpileMain(const Instruction &instruction) {
        if(!_scope->isGlobalScope()) { throw TranspilerException("Cannot declare main here", instruction); }
        if(_main) { throw TranspilerException("Main already declared", instruction); }
        if(_scope->getParent() != nullptr) { throw TranspilerException("Cannot declare main here", instruction); }
        _text += "int main(int argc, char **argv) {\n";
        _main = true;
        openScope(ScopeType::MAIN_SCOPE);
        auto value = FORMAT("{:\t^{}}const vnd::vector<string> _args = vnd::createArgs(argc, argv);", "", _tabs);
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
        if(isConst || isVal) {
            reformatConstansInGlobalScope();
            _text += "const ";
        }
        auto [type, typeValue] = transpileType(iterator, tokens.end(), {TokenType::EQUAL_OPERATOR}, instruction);
        _text += FORMAT("{} ", typeValue);
        if(isConst && !Scope::isPrimitive(type)) {
            throw TRANSPILER_EXCEPTIONF(instruction, "Cannot declare const variables of {} type", type);
        }
        // Handle initialization
        handleInitialization(instruction, iterator, endToken, factory);
        if((isConst || isVal) && variables.size() > factory.size()) {
            throw TRANSPILER_EXCEPTIONF(instruction, "Uninitialized constant: {} values for {} constants", factory.size(),
                                        variables.size());
        }
        for(const auto &jvar : variables) {
            std::string value;
            formatVariable(jvar);
            if(!factory.empty()) {
                auto expression = factory.getExpression();
                auto [first, second] = _scope->canAssign(type, expression.getType());
                if(!first) { throw TRANSPILER_EXCEPTIONF(instruction, "Cannot assign {} to {}", expression.getType(), type); }
                printPrecisionLossWarning(instruction, second, expression.getType(), type);
                if(isConst) {
                    transpileConstDeclaration(expression, instruction, jvar);
                } else {
                    _text += FORMAT(" = {}", expression.getText());
                }
                value = expression.getValue();
            } else {
                _text += "{}";
            }
            auto [check, shadowing] = _scope->checkVariable(jvar);
            if(check) {
                if(!shadowing) { throw TranspilerException(FORMAT("{} already defined", jvar), instruction); }
                LWARN("{} already defined", jvar);
            }
            addConstansOrVariable(isConst, isVal, type, jvar, value);
            emplaceCommaColon(jvar == variables.back());
            if(isConst) { _text += "\n"; }
        }
    }
    void Transpiler::handleInitialization(const Instruction &instruction, TokenVecIter &iterator, const TokenVecIter &endToken,
                                          ExpressionFactory &factory) const {
        if(iterator != endToken && iterator->isType(TokenType::EQUAL_OPERATOR)) {
            iterator = std::next(iterator);
            while(iterator != endToken) {
                if(auto error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
                if(iterator != endToken) { iterator = std::next(iterator); }
            }
        }
    }

    void Transpiler::reformatConstansInGlobalScope() {
        if(_text.ends_with(R"(;

)")) {
            _text = FORMAT("{}", _text.substr(0, _text.size() - 1));
        }
    }

    void Transpiler::addConstansOrVariable(const bool isConst, const bool isVal, const std::string &type, const std::string_view &jvar,
                                           const std::string &value) {
        if(isConst) {
            _scope->addConstant(jvar, type, value);
        } else {
            _scope->addVariable(jvar, type, isVal);
        }
    }

    void Transpiler::formatVariable(const std::string_view &jvar) {
        if(!jvar.empty() && jvar.starts_with('_')) {
            _text += FORMAT("v{}", jvar);
        } else {
            _text += FORMAT("_{}", jvar);
        }
    }

    void Transpiler::transpileConstDeclaration(const Expression &expression, const Instruction &instruction, const std::string_view view) {
        if(expression.isConst()) [[likely]] {
            _text += FORMAT(" = {}", expression.getValue());
        } else [[unlikely]] {
            throw TRANSPILER_EXCEPTIONF(instruction, "Cannot evaluate {} at compile time", view);
        }
    }

    bool Transpiler::iteratorIs(const TokenVecIter &iterator, const std::string &value) const noexcept {
        return iterator->getValue() == value;
    }

    void Transpiler::transpileOperation(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        std::vector<Expression> expressions;
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        while(iterator != endToken) {
            if(const auto &error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
            if(iterator != endToken) { iterator = std::next(iterator); }
        }
        for(const auto &expression : factory.getExpressions()) {
            std::string text = expression.getText();
            if(expression.getType() != "void" && !(text.ends_with("++") || text.ends_with("--"))) {
                throw TRANSPILER_EXCEPTIONF(instruction, "Invalid operation {}", text);
            }
            _text += FORMAT("{};\n{:\t^{}}", text, "", _tabs);
        }
        _text.erase(_text.size() - _tabs - 1, _tabs + 1);
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    void Transpiler::transpileAssignation(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        Token equalToken;
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        auto variables = extractVariables(iterator, endToken, instruction);
        equalToken = *iterator;
        iterator = std::next(iterator);
        while(iterator != endToken) {
            if(auto error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
            if(iterator != endToken) { iterator = std::next(iterator); }
        }
        if(factory.isMultiplefun()) {
            if(equalToken.isType(TokenType::OPERATION_EQUAL)) {
                throw TRANSPILER_EXCEPTIONF(instruction, "incompatible operator {}", equalToken.getValue());
            }
            auto [first, second] = transpileMultipleFun(variables, factory.getExpression());
            if(!first.empty()) { throw TranspilerException(first, instruction); }
            if(!second.empty()) { LWARN("{}\n{}", instruction.toString(), second); }
            return;
        }
        auto expressions = factory.getExpressions();
        if(variables.size() != expressions.size()) {
            throw TRANSPILER_EXCEPTIONF(instruction, "inconsistent assignation: {} values for {} variables", expressions.size(),
                                        variables.size());
        }
        if(equalToken.getValue() == "=" && transpileSwap(variables, expressions)) { return; }
        auto exprIterator = expressions.begin();
        for(const auto &[first, second] : variables) {
            auto [firstt, secondd] = transpileAssigment(first, second, equalToken, *exprIterator);
            if(!firstt.empty()) { throw TranspilerException(firstt, instruction); }
            printPrecisionLossWarning(instruction, secondd, second, exprIterator->getType());
            exprIterator = std::ranges::next(exprIterator);
        }
        _text.erase(_text.size() - _tabs - 1, _tabs + 1);
    }

    void Transpiler::transpileStructure(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        if(iteratorIs(iterator, "if")) {
            openScope(ScopeType::IF_SCOPE);
        } else {
            openScope(ScopeType::LOOP_SCOPE);
        }
        _text += iterator->getValue();
        iterator += 2;
        if(auto error = transpileCondition(iterator, tokens.end()); !error.empty()) { throw TranspilerException(error, instruction); }
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileElse(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        if(_scope->getType() != ScopeType::IF_SCOPE) { throw TranspilerException("Unexpected instruction", instruction); }
        if(_text.ends_with('\t')) { _text.pop_back(); }
        closeScope();
        _text += "} else ";
        iterator += 2;
        if(iterator->getType() != TokenType::K_IF) {
            _text += "{";
            openScope(ScopeType::ELSE_SCOPE);
            checkTrailingBracket(instruction);
            return;
        }
        iterator += 2;
        _text += "if";
        if(auto error = transpileCondition(iterator, tokens.end()); !error.empty()) { throw TranspilerException(error, instruction); }
        openScope(ScopeType::IF_SCOPE);
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileFor(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto tmp = std::vector<Token>{tokens.back()};
        tokens.pop_back();
        if(tokens.back().isType(TokenType::OPEN_CUR_PARENTESIS)) { tokens.pop_back(); }
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        _text += "FOR_LOOP(";
        openScope(ScopeType::LOOP_SCOPE);
        iterator = std::next(iterator);
        auto [identifier, type] = transpileForInitialization(iterator, endToken, instruction);
        iterator = std::next(iterator);
        if(auto error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
        auto condition = factory.getExpression();
        if(!Scope::isNumber(condition.getType())) { throw TranspilerException("For final value must be of numeric type", instruction); }
        if(iterator == endToken) {
            _text += FORMAT("{}, 1) {{", condition.getText());
            if(!identifier.empty()) { _scope->addVariable(identifier, type, false); }
            if(tmp.begin()->isType(TokenType::CLOSE_CUR_PARENTESIS)) {
                _text += "}";
                closeScope();
            }
            return;
        }
        iterator = std::next(iterator);
        if(auto error = factory.parse({}); !error.empty()) { throw TranspilerException(error, instruction); }
        auto step = factory.getExpression();
        if(!Scope::isNumber(step.getType())) { throw TranspilerException("For step value must be of numeric type", instruction); }
        _text += FORMAT("{}, {}) {{", condition.getText(), step.getText());
        if(!identifier.empty()) { _scope->addVariable(identifier, type, false); }
        if(tmp.begin()->isType(TokenType::CLOSE_CUR_PARENTESIS)) {
            _text += "}";
            closeScope();
        }
    }

    void Transpiler::transpileBreak(const Instruction &instruction) {
        auto identifier = instruction.getTokens().begin()->getValue();
        auto scope = _scope;
        while(scope->getType() != ScopeType::LOOP_SCOPE) {
            if(scope->isGlobalScope()) { throw TRANSPILER_EXCEPTIONF(instruction, "Cannot use {} outside a loop", identifier); }
            scope = scope->getParent();
        }
        _text += FORMAT("{};", identifier);
    }

    void Transpiler::transpileDefinition(const Instruction &instruction) {
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        auto parentScope = _scope;
        StringPairVec params;
        std::vector<std::string> returnTypevalues;
        FunType fun = FunType::createEmpty();
        iterator = std::next(iterator);
        const auto identifier = iterator->getValue();
        iterator = std::next(iterator);
        openScope(ScopeType::FUNCTION_SCOPE);
        if(auto next = std::next(iterator); next->getType() == TokenType::CLOSE_PARENTESIS) { iterator = next; }
        while(iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            iterator = std::next(iterator);
            auto param = iterator->getValue();
            iterator = std::next(iterator);
            auto [type, typevalue] = transpileType(iterator, endToken, {TokenType::COMMA, TokenType::CLOSE_PARENTESIS}, instruction);
            _scope->addVariable(param, std::string_view{type}, false);
            fun.addParam(type);
            params.emplace_back(std::make_pair(param, typevalue));
        }
        iterator = std::next(iterator);
        while(iterator->getType() != TokenType::OPEN_CUR_PARENTESIS) {
            auto [type, typevalue] = transpileType(iterator, endToken, {TokenType::COMMA, TokenType::OPEN_CUR_PARENTESIS}, instruction);
            _returnTypes.emplace_back(type);
            returnTypevalues.emplace_back(typevalue);
        }
        if(returnTypevalues.empty()) {
            _text += "void ";
            fun.addReturn("void");
        } else if(returnTypevalues.size() == 1) {
            _text += FORMAT("{} ", returnTypevalues.at(0));
            fun.addReturn(_returnTypes.at(0));
        } else {
            _text += "std::tuple<";
            auto returnValue = _returnTypes.begin();
            for(const auto &typevalue : returnTypevalues) {
                _text += FORMAT("{}, ", typevalue);
                fun.addReturn(*returnValue);
                returnValue = std::next(returnValue);
            }
            _text.erase(_text.end() - 2, _text.end());
            _text += "> ";
        }
        formatVariable(identifier);
        _text += "(";
        for(const auto &param : params) {
            _text += FORMAT("{} ", param.second);
            formatVariable(param.first);
            _text += ", ";
        }
        if(!params.empty()) { _text.erase(_text.end() - 2, _text.end()); }
        _text += ") {";
        parentScope->addFun(identifier, fun);
        checkTrailingBracket(instruction);
    }

    void Transpiler::transpileReturn(const Instruction &instruction) {
        auto scope = _scope;
        while(scope->getType() != ScopeType::FUNCTION_SCOPE) {
            if(scope->getType() == ScopeType::GLOBAL_SCOPE) {
                throw TranspilerException("Cannot use return outside a function", instruction);
            }
            scope = scope->getParent();
        }
        auto tokens = instruction.getTokens();
        auto iterator = tokens.begin();
        const auto endToken = tokens.end();
        auto factory = ExpressionFactory::create(iterator, endToken, _scope, false);
        _text += "return";
        if(_returnTypes.size() > 1) { _text += " {"; }
        iterator = std::next(iterator);
        while(iterator != endToken) {
            if(const auto &error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
            if(iterator != endToken) { iterator = std::next(iterator); }
        }
        if(_returnTypes.size() != factory.size()) {
            throw TRANSPILER_EXCEPTIONF(instruction, "Inconsistent return: {} values where {} expected", factory.size(),
                                        _returnTypes.size());
        }
        for(const auto &type : _returnTypes) {
            auto expression = factory.getExpression();
            auto [first, second] = _scope->canAssign(type, expression.getType());
            if(!first) { throw TRANSPILER_EXCEPTIONF(instruction, "Cannot return {} as {}", expression.getType(), type); }
            printPrecisionLossWarning(instruction, second, type, expression.getType());
            _text += FORMAT(" {},", expression.getText());
        }
        if(!_returnTypes.empty()) { _text.pop_back(); }
        if(_returnTypes.size() > 1) { _text += "}"; }
        _text += ";";
    }

    std::vector<std::string_view> Transpiler::extractIdentifiers(TokenVecIter &iterator, const Instruction &instruction) const {
        using enum TokenType;
        std::vector<std::string_view> result;
        while(!iterator->isType(COLON)) {
            if(iterator->isType(IDENTIFIER)) {
                if(_scope->checkType(iterator->getValue())) {
                    throw TRANSPILER_EXCEPTIONF(instruction, "Identifier {} not allowed", iterator->getValue());
                }
                if(iterator->getValue() == "_") {
                    throw TranspilerException("Cannot declare variables using blank identifier", instruction);
                }
                result.emplace_back(iterator->getValue());
            }
            iterator = std::next(iterator);
        }
        return result;
    }

    StringPairVec Transpiler::extractVariables(TokenVecIter &iterator, const TokenVecIter &end, const Instruction &instruction) const {
        using enum TokenType;
        StringPairVec result;
        std::string currentVariable;
        std::string type;
        bool assignable = true;
        while(iterator != end && iterator->getType() != EQUAL_OPERATOR && iterator->getType() != OPERATION_EQUAL) {
            const auto next = std::ranges::next(iterator);
            if(iterator->isType(IDENTIFIER)) {
                if(next != end && next->isType(OPEN_PARENTESIS)) {
                    if(const auto &error = extractFun(iterator, end, currentVariable, type); !error.empty()) {
                        throw TranspilerException(error, instruction);
                    }
                } else if(const auto &error = extractToken(iterator, end, next, currentVariable, type, assignable); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
            } else if(iterator->isType(OPEN_SQ_PARENTESIS)) {
                if(const auto &error = extractSquareExpression(iterator, end, currentVariable, type, assignable); !error.empty()) {
                    throw TranspilerException(error, instruction);
                }
            } else if(iterator->isType(UNARY_OPERATOR)) {
                throw TRANSPILER_EXCEPTIONF(instruction, "Cannot use {} at the left side of an assignation", iterator->getValue());
            } else if(iterator->isType(COMMA)) {
                if(currentVariable.ends_with("->")) { currentVariable.erase(currentVariable.end() - 2, currentVariable.end()); }
                result.emplace_back(currentVariable, type);
                currentVariable.clear();
                type.clear();
            }
            iterator = std::next(iterator);
        }
        if(currentVariable.ends_with("->")) { currentVariable.erase(currentVariable.end() - 2, currentVariable.end()); }
        result.emplace_back(currentVariable, type);
        return result;
    }

    std::string Transpiler::extractToken(const TokenVecIter &iterator, const TokenVecIter &end, const TokenVecIter &next,
                                         std::string &currentVariable, std::string &type, bool &assignable) const noexcept {
        using enum TokenType;
        auto value = iterator->getValue();
        std::string_view newType;
        if(value == "_") {
            if(currentVariable.empty() && (next == end || next->isTypeAny_of({COMMA, EQUAL_OPERATOR, OPERATION_EQUAL}))) {
                currentVariable = "_";
                type = "";
                return {};
            } else {
                return "Cannot use blank identifier here";
            }
        }
        std::tie(newType, assignable) = std::make_pair(_scope->getVariableType(type, value), _scope->getConstValue(type, value).empty());
        if(next != end && (next->isTypeAny_of({COMMA, EQUAL_OPERATOR, OPERATION_EQUAL, OPEN_SQ_PARENTESIS}))) {
            assignable = !_scope->isConstant(type, value);
        }
        if(newType.empty()) { return FORMAT("Cannot find identifier {}.{}", type, value); }
        if(!assignable && !next->isType(OPEN_SQ_PARENTESIS)) { return FORMAT("Cannot assign {}.{}", type, value); }
        type = newType;
        if(currentVariable.empty()) {
            currentVariable += FORMAT("{}->", value);
            if(currentVariable.starts_with('_')) {
                currentVariable = FORMAT("v{}", currentVariable);
            } else {
                currentVariable = FORMAT("_{}", currentVariable);
            }
        } else if(next != end && next->isType(DOT_OPERATOR)) {
            currentVariable += FORMAT("get{}{}()->", char(std::toupper(C_UC(value[0]))), value.substr(1));
        } else {
            currentVariable += FORMAT("set{}{}(", char(std::toupper(C_UC(value[0]))), value.substr(1));
        }
        return {};
    }

    std::string Transpiler::extractFun(TokenVecIter &iterator, const TokenVecIter &end, std::string &currentVariable,
                                       std::string &type) const noexcept {
        using enum vnd::TokenType;
        auto identifier = iterator->getValue();
        auto factory = ExpressionFactory::create(iterator, end, _scope, false);
        std::vector<Expression> expressions;
        iterator = std::next(iterator);
        while(iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            iterator = std::next(iterator);
            if(iterator->getType() != CLOSE_PARENTESIS) {
                if(const auto &error = factory.parse({COMMA, CLOSE_PARENTESIS}); !error.empty()) { return error; }
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
            if(value.starts_with(".")) { value.erase(0, 1); }
            return FORMAT("Function {} not found", value);
        }
        type = newType;
        auto params = ExpressionFactory::transpileFun(expressions, variadic);
        if(constructor) {
            currentVariable += FORMAT("std::make_shared<{}>({})->", newType, params);
        } else {
            const bool empty = currentVariable.empty();
            currentVariable += FORMAT("{}({})->", identifier, params);
            if(empty) {
                if(currentVariable.starts_with('_')) {
                    currentVariable = FORMAT("v{} ", currentVariable);
                } else {
                    currentVariable = FORMAT("_{}", currentVariable);
                }
            }
        }
        return {};
    }

    std::string Transpiler::extractSquareExpression(TokenVecIter &iterator, const TokenVecIter &end, std::string &currentVariable,
                                                    std::string &type, const bool assignable) const noexcept {
        using enum TokenType;
        if(currentVariable.ends_with("->")) { currentVariable.erase(currentVariable.end() - 2, currentVariable.end()); }
        if(!Scope::checkVector(type)) { return FORMAT("Indexing not allowed for {} type", type); }
        auto factory = ExpressionFactory::create(iterator, end, _scope, false, true);
        iterator = std::next(iterator);
        if(const auto &error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        auto next = std::ranges::next(iterator);
        if(auto newType = expression.getType(); !Scope::isInteger(newType)) { return FORMAT("{} index not allowed", newType); }
        if(type == "char") { return "Strings are immutable"; }
        if(!assignable && next->isTypeAny_of({COMMA, EQUAL_OPERATOR, OPERATION_EQUAL})) { return "Cannot assign constant vectors"; }
        currentVariable += FORMAT(".at({})->", expression.getText());
        return {};
    }

    StringPair Transpiler::transpileMultipleFun(const StringPairVec &variables, const Expression &expression) noexcept {
        auto types = Transpiler::tokenize(expression.getType());
        std::string values;
        std::string warnings;
        StringVec tmp;
        std::size_t typeIndex = 0;
        if(variables.size() != types.size()) {
            return {FORMAT("Inconsistent assignation: {} return values for {} variables", types.size(), variables.size()), {}};
        }
        for(size_t i = 0; i != types.size(); i++) {
            values += FORMAT("vnd::tmp[\"{}\"], ", i);
            tmp.emplace_back(FORMAT("vnd::tmp.at(\"{}\")", i));
        }
        values.erase(values.end() - 2, values.end());
        _text += FORMAT("std::tie({}) = {};\n{:\t^{}}", values, expression.getText(), "", _tabs);
        for(const auto &[first, second] : variables) {
            if(first != "_") {
                auto type = types.at(typeIndex);
                auto typeValue = Scope::getTypeValue(type);
                auto [firsttt, seconddd] = _scope->canAssign(second, type);
                if(!firsttt) { return {FORMAT("Cannot assign {}.{}", type, first), {}}; }
                if(seconddd) { warnings = FORMAT("Possible loss of precision from {} to {}\n", second, type); }
                typeIndex++;
                if(first.ends_with('(')) {
                    _text += FORMAT("{}std::any_cast<{}>({}));\n{:\t^{}}", first, typeValue, tmp.front(), "", _tabs);
                } else if(first != "_") {
                    _text += FORMAT("{} = std::any_cast<{}>({});\n{:\t^{}}", first, typeValue, tmp.front(), "", _tabs);
                }
            }
            tmp.erase(tmp.begin());
        }
        if(!warnings.empty()) { warnings.pop_back(); }
        _text += "vnd::tmp.clear();";
        return {{}, warnings};
    }

    StringPair Transpiler::transpileType(TokenVecIter &iterator, const TokenVecIter &end, const std::vector<TokenType> &endTokens,
                                         const Instruction &instruction) {
        using enum TokenType;
        auto type = std::string{(++iterator)->getValue()};
        auto typeValue = type;
        std::string prefix;
        std::string suffix;
        if(typeValue == "void" || typeValue == "any") {
            throw TranspilerException(FORMAT("Cannot declare {} variables", typeValue), instruction);
        }
        if(!_scope->checkType(type)) { throw TranspilerException(FORMAT("Type {} not valid", type), instruction); }
        if(!vnd::Scope::isPrimitive(type)) { typeValue = FORMAT("std::shared_ptr<{}>", type); }
        while(iterator != end && std::ranges::find(endTokens, iterator->getType()) == endTokens.end()) {
            if(iterator->isType(OPEN_SQ_PARENTESIS)) {
                if(std::ranges::next(iterator)->isType(CLOSE_SQ_PARENTESIS)) {
                    prefix += "vnd::vector<";
                    suffix = FORMAT(">{}", suffix);
                    type += "[]";
                } else {
                    std::string size;
                    iterator = std::next(iterator);
                    auto factory = ExpressionFactory::create(iterator, end, _scope, true, true);
                    if(const auto &error = factory.parse({CLOSE_SQ_PARENTESIS, EQUAL_OPERATOR}); !error.empty()) {
                        throw TranspilerException(error, instruction);
                    }
                    auto expression = factory.getExpression();
                    if(!expression.isConst()) { throw TranspilerException("Cannot evaluate array dimension at compile time", instruction); }
                    size = expression.getValue();
                    if(std::stoi(size) < 0) { throw TranspilerException("Array cannot have negative size", instruction); }
                    prefix += "vnd::array<";
                    suffix = FORMAT(", {}>{}", size, suffix);
                    type += FORMAT("[{}]", size);
                }
            }
            iterator = std::next(iterator);
        }
        return {type, FORMAT("{}{}{}", prefix, typeValue, suffix)};
    }

    bool Transpiler::transpileSwap(const StringPairVec &variables, const std::vector<Expression> &expressions) noexcept {
        if(variables.size() != 2 || expressions.size() != 2) { return false; }
        StringVec swapVariables = {variables.at(0).first, variables.at(1).first};
        StringVec swapExpressions = {expressions.at(0).getText(), expressions.at(1).getText()};
        if(std::vector<std::pair<bool, bool>> results = {_scope->canAssign(expressions.at(0).getType(), expressions.at(1).getType()),
                                                         _scope->canAssign(expressions.at(1).getType(), expressions.at(0).getType())};
           !results.at(0).first || !results.at(1).first) {
            return false;
        }
        for(auto &iter : swapVariables) {
            if(iter.ends_with('(')) { return false; }
            std::erase_if(iter, isspace);
        }
        for(auto &iter : swapExpressions) { std::erase_if(iter, isspace); }
        if(swapVariables.at(0) != swapExpressions.at(1) || swapVariables.at(1) != swapExpressions.at(0)) { return false; }
        _text += FORMAT("std::swap({}, {});", swapVariables.at(0), swapVariables.at(1));
        return true;
    }

    std::pair<std::string, bool> Transpiler::transpileAssigment(const std::string &variable, const std::string &type,
                                                                const Token &equalToken, const Expression &expression) noexcept {
        std::string_view equalValue = equalToken.getValue();
#ifdef __llvm__
        const bool exprContainsSpace = expression.getType().find(' ') != std::string::npos;
#else
        const bool exprContainsSpace = expression.getType().contains(' ');
#endif
        if(exprContainsSpace) { return {"Multiple return value functions must be used alone", false}; }
        if(equalToken.isType(TokenType::OPERATION_EQUAL) && !Scope::isNumber(type)) {
            if(variable == "_") { return {FORMAT("incompatible operator {} for blank identifier", equalToken.getValue()), false}; }
            return {FORMAT("incompatible operator {} for {} type", equalToken.getValue(), type), false};
        }
        if(variable == "_") {
            if(expression.getType() == "void") {
                return {"Cannot assign void expression", false};
            } else {
                _text += FORMAT("{};\n{:\t^{}}", expression.getText(), "", _tabs);
                return {};
            }
        }
        auto [first, second] = _scope->canAssign(type, expression.getType());
        if(!first) { return {FORMAT("Cannot assign {} to {}", expression.getType(), type), false}; }
        auto text = variable;
        auto getter = variable;
        if(!variable.ends_with('(')) { text += " = "; }
        if(equalToken.isType(TokenType::OPERATION_EQUAL)) {
            if(variable.ends_with('(')) {
                getter.replace(getter.find_last_of("->") + 1, 1, "g");
                getter = FORMAT("{})", getter);
            }
            if(equalValue == "^=") {
                text += FORMAT("vnd::pow({}, ", getter);
            } else if(equalValue == "%=") {
                text += FORMAT("vnd::mod({}, ", getter);
            } else {
                text += FORMAT("{} {} ", getter, equalToken.getValue().at(0));
            }
        }
        text += expression.getText();
        if(equalValue == "^=" || equalValue == "%=") { text += ")"; }
        if(variable.ends_with('(')) { text += ")"; }
        _text += FORMAT("{};\n{:\t^{}}", text, "", _tabs);
        return {{}, second};
    }

    std::string Transpiler::transpileCondition(TokenVecIter &iterator, const TokenVecIter &end) noexcept {
        auto factory = ExpressionFactory::create(iterator, end, _scope, false);
        if(const auto &error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        if(expression.getType() != "bool") { return "Invalid condition type"; }
        _text += FORMAT("({}) {{", expression.getText());
        return {};
    }

    StringPair Transpiler::transpileForInitialization(TokenVecIter &iterator, const TokenVecIter &end, const Instruction &instruction) {
        auto factory = ExpressionFactory::create(iterator, end, _scope, false);
        std::string identifier;
        std::string type;
        std::string typeValue;
        std::string declaration;
        if(iterator->isType(TokenType::K_VAR)) {
            if(iterator->getValue() != "var") { throw TranspilerException("For variables must be declared using var", instruction); }
            iterator = std::next(iterator);
            identifier = iterator->getValue();
            iterator = std::next(iterator);
            std::tie(type, typeValue) = transpileType(iterator, end, {TokenType::EQUAL_OPERATOR}, instruction);
            if(!Scope::isNumber(type)) { throw TranspilerException("For variables must be of numeric type", instruction); }
            auto [check, shadowing] = _scope->checkVariable(identifier);
            if(check) {
                if(!shadowing) { throw TRANSPILER_EXCEPTIONF(instruction, "{} already defined", identifier); }
                LWARN("{} already defined", identifier);
            }
            declaration = identifier;
        } else {
            identifier = iterator->getValue();
            iterator = std::next(iterator);
        }
        if(identifier.starts_with("_")) {
            identifier = FORMAT("v{}", identifier);
        } else {
            identifier = FORMAT("_{}", identifier);
        }
        _text += FORMAT("{}, {}, ", typeValue, identifier);
        iterator = std::next(iterator);
        if(const auto &error = factory.parse({TokenType::COMMA}); !error.empty()) { throw TranspilerException(error, instruction); }
        auto expression = factory.getExpression();
        if(!Scope::isNumber(expression.getType())) { throw TranspilerException("For variables must be of numeric type", instruction); }
        _text += FORMAT("{}, ", expression.getText());
        return {declaration, type};
    }

    void Transpiler::openScope(const ScopeType &type) noexcept {
        auto newScope = Scope::create(_scope, type);
        _scope = newScope;
        _tabs++;
    }

    void Transpiler::closeScope() noexcept {
        auto oldScope = _scope;
        _scope = _scope->getParent();
        oldScope->removeParent();
        if(oldScope->getType() == ScopeType::FUNCTION_SCOPE) { _returnTypes.clear(); }
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
