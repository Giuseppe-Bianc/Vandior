// NOLINTBEGIN(*-include-cleaner, *-env33-c)
#include "Vandior/ExpressionFactory.hpp"
#include "Vandior/disableWarn.hpp"
#include <algorithm>

/**
 * This macro disable some msvc warnigs.
 * \cond
 */
DISABLE_WARNINGS_PUSH(26447)
/** \endcond */

#ifdef _MSC_VER
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

namespace vnd {
    static inline constexpr std::string_view sps = " ";      // space
    static inline constexpr std::string_view nots = "not";   // not string
    static inline constexpr std::string_view bols = "bool";  // bool string
    static inline constexpr std::string_view oprt = "operator";
    static inline constexpr std::string_view ints = "int";
    static inline constexpr std::size_t buffer_size = 128;
    // NOLINTBEGIN(*-pass-by-value, *-identifier-length)
    ExpressionFactory::ExpressionFactory(TokenVecIter &iterator, const TokenVecIter &end, std::shared_ptr<Scope> scope, const bool isConst,
                                         const bool sq) noexcept
      : _iterator(iterator), _end(end), _scope(std::move(scope)), _const(isConst), _sq(sq) {}

    ExpressionFactory ExpressionFactory::create(TokenVecIter &iterator, const TokenVecIter &end, std::shared_ptr<Scope> scope,
                                                const bool isConst, bool sq) noexcept {
        return {iterator, end, std::move(scope), isConst, sq};
    }
    // NOLINTEND(*-pass-by-value, *-identifier-length)

    std::string ExpressionFactory::transpileFun(const std::vector<Expression> &expressions, std::optional<size_t> variadic) noexcept {
        std::string params;
        size_t pos = 0;
        std::ranges::for_each(expressions, [&](const Expression &expression) {
            if(variadic.has_value() && pos == variadic.value()) { params += "{"; }
            params += FORMAT("{}, ", expression.getText());
            pos++;
        });
        if(!params.empty()) { params.erase(params.size() - 2, 2); }
        if(variadic.has_value()) {
            if(pos <= variadic.value()) { params += ", {"; }
            params += "}";
        }
        return params;
    }

    bool ExpressionFactory::isSquareType(const std::string_view &type) noexcept { return type == ints || type == oprt; }

    std::string ExpressionFactory::evaluate(const std::string &expression) const noexcept {
#ifdef _WIN32
        std::string command = FORMAT("python -c \"print({})\"", expression);
#else
        std::string command = FORMAT("python3 -c \"print({})\"", expression);
#endif

        std::unique_ptr<FILE, decltype(&PCLOSE)> pipe(POPEN(command.c_str(), "r"), PCLOSE);
        if(!pipe) { return "0"; }

        std::stringstream sss;
        std::array<char, buffer_size> buffer{};
        while(fgets(buffer.data(), buffer_size, pipe.get()) != nullptr) { sss << buffer.data(); }
        std::string result = sss.str();
        if(result.ends_with('\n')) { result.pop_back(); }
        return result;
    }

    void ExpressionFactory::resetVariables() noexcept {
        _text = {};
        _expressionText = "";
        _operators.reset();
        _divide = false;
        _dot = false;
        _type = "";
        _temp = "";
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::string ExpressionFactory::parse(const std::vector<TokenType> &endToken) noexcept {  // NOLINT(*-no-recursion)
        using enum vnd::TokenType;
        resetVariables();
        std::tuple<bool, bool, std::string> type;
        while(!isEnd(_iterator) && std::ranges::find(endToken, _iterator->getType()) == endToken.end()) {
            const auto iterType = _iterator->getType();
            if(iterType == UNARY_OPERATOR) {
                ++_iterator;
                continue;
            }
            if(auto nextIter = std::ranges::next(_iterator);
               iterType == IDENTIFIER && nextIter != _end && nextIter->isType(OPEN_PARENTESIS)) {
                if(auto error = handleFun(type); !error.empty()) { return error; }
            } else if(iterType == OPEN_PARENTESIS) {
                if(auto error = handleInnerExpression(type); !error.empty()) { return error; }
            } else if(iterType == OPEN_SQ_PARENTESIS) {
                if(std::string error = handleSquareExpression(type); !error.empty()) { return error; }
            } else if(iterType == OPEN_CUR_PARENTESIS) {
                if(std::string error = handleVectorInitialization(type); !error.empty()) { return error; }
            } else if(std::string error = handleToken(type); !error.empty()) {
                return error;
            }
        }
        if(std::get<0>(type)) { std::get<2>(type) = bols; }
        handleFinalExpression(type);
        return {};
    }

    void ExpressionFactory::handleFinalExpression(const TupType &type) noexcept {
        if(Scope::isNumber(std::get<2>(type))) {
            std::string value;
            if(_const) { value = ExpressionFactory::evaluate(_expressionText); }
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const, value));
        } else {
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const));
        }
    }

    std::size_t ExpressionFactory::size() const noexcept { return _expressions.size(); }

    bool ExpressionFactory::empty() const noexcept { return _expressions.empty(); }

    Expression ExpressionFactory::getExpression() noexcept {
        auto result = *_expressions.begin();
        _expressions.erase(_expressions.begin());
        return result;
    }

    std::vector<Expression> ExpressionFactory::getExpressions() noexcept {
        auto result = _expressions;
        _expressions.clear();
        return result;
    }

    bool ExpressionFactory::isMultiplefun() const noexcept {
        if(_expressions.size() != 1) { return false; }
        return _expressions.front().getType().find(' ') != std::string::npos;
    }

    std::string_view ExpressionFactory::getTokenType() const noexcept {
        using enum TokenType;
        auto iterValue = _iterator->getValue();
        switch(_iterator->getType()) {
        case INTEGER:
            return ints;
        case DOUBLE:
            return "float";
        case BOOLEAN:
            return bols;
        case CHAR:
            return "char";
        case STRING:
            return "string";
        case IDENTIFIER:
            return _scope->getVariableType(_type, iterValue);
        case OPERATOR:
        case MINUS_OPERATOR:
            [[fallthrough]];
        case UNARY_OPERATOR:
            return oprt;
        case DOT_OPERATOR:
            return "dot";
        case BOOLEAN_OPERATOR:
            if(iterValue == "==" || iterValue == "!=") { return "equal"; }
            return "boolean";
        case NOT_OPERATOR:
            return nots;
        case LOGICAL_OPERATOR:
            return "logical";
        case K_NULLPTR:
            return "nullptr";
        default:
            break;
        }
        return {};
    }

    void ExpressionFactory::emplaceToken(const std::string_view &type) noexcept {
        using enum TokenType;
        clearData();
        if(_iterator->isType(TokenType::DOT_OPERATOR)) {
            ++_iterator;
            return;
        }
        if(checkNextToken(std::string{type}, writeToken())) { return; }
        _dot = false;
        const auto value = _iterator->getValue();
        if(_iterator->isType(CHAR)) {
            auto frtVal = FORMAT("'{}'", std::string{value});
            _text.emplace_back(frtVal);
            _expressionText += frtVal;
            ++_iterator;
            return;
        }
        if(value == "^") {
            if(!_operators.has_value()) { _operators = _text.size() - 1; }
            const auto textIndex = _text.begin() + C_LL(_operators.value());
            _text.emplace(textIndex, "vnd::pow(");
            _text.emplace_back(",");
            _expressionText += "**";
            ++_iterator;
            return;
        }
        if(value == "%") {
            if(!_operators.has_value()) { _operators = _text.size() - 1; }
            const auto textIndex = _text.begin() + C_LL(_operators.value());
            _text.emplace(textIndex, "vnd::mod(");
            _text.emplace_back(",");
            _expressionText += value;
            ++_iterator;
            return;
        }
        auto text = _temp + writeToken();
        if(_const) {
            if(_iterator->isType(TokenType::IDENTIFIER)) {
                auto constValue = _scope->getConstValue(_type, _iterator->getValue());
                if(constValue.empty()) {
                    _const = false;
                } else {
                    _expressionText += constValue;
                }
            } else {
                _expressionText += text;
            }
        }
        checkOperators(text);
        if(!_text.empty()) { text = FORMAT(" {}", text); }
        _text.emplace_back(text);
        if(value == "/" && !_sq) { _divide = true; }
        ++_iterator;
    }

    std::string ExpressionFactory::writeToken() noexcept {
        auto value = std::string{_iterator->getValue()};
        if(_iterator->isType(TokenType::STRING)) { value = FORMAT(R"(string("{}"))", value); }
        if(_iterator->isType(TokenType::IDENTIFIER)) {
            if(_temp.empty()) {
                if(!value.empty() && value.starts_with('_')) {
                    value = FORMAT("v{}", value);
                } else {
                    value = FORMAT("_{}", value);
                }
            } else {
                value = FORMAT("get{}{}()", char(std::toupper(value.at(0))), value.substr(1));
            }
            auto nxtIter = std::ranges::next(_iterator);
            if(!isEnd(nxtIter) && nxtIter->isType(TokenType::UNARY_OPERATOR)) { value += nxtIter->getValue(); }
        }
        if(_iterator->isType(TokenType::INTEGER) && value.starts_with('#')) {
            value.erase(0, 1);
            if(!value.empty() && value.starts_with('o')) {
                value.erase(0, 1);
                return FORMAT("0{}", value);
            }
            return FORMAT("0x{}", value);
        }
        return value;
    }

    std::string ExpressionFactory::handleFun(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        using enum vnd::TokenType;
        auto identifier = _iterator->getValue();
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope, false);
        std::vector<Expression> expressions;
        std::string params;
        if(_const && !_temp.empty()) { return "Cannot call methods on const value"; }
        ++_iterator;
        while(!_iterator->isType(TokenType::CLOSE_PARENTESIS)) {
            ++_iterator;
            if(!_iterator->isType(CLOSE_PARENTESIS)) {
                if(std::string error = factory.parse({COMMA, CLOSE_PARENTESIS}); !error.empty()) { return error; }
            }
        }

        auto nxtIter = std::ranges::next(_iterator);
        if(isEnd(nxtIter) || !nxtIter->isType(DOT_OPERATOR)) { _const = false; }
        expressions = factory.getExpressions();
        auto [newType, constructor, variadic] = _scope->getFunType(_type, identifier, expressions);
        if(newType.empty()) {
            std::string paramTypes;
            for(const auto &expression : expressions) { paramTypes += expression.getType() + ","; }
            if(!paramTypes.empty()) { paramTypes.pop_back(); }
            auto value = FORMAT("{}.{}({})", _type, identifier, paramTypes);
            if(value.starts_with(".")) { value.erase(0, 1); }
            return FORMAT("Function {} not found", value);
        }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        params = ExpressionFactory::transpileFun(expressions, variadic);
        auto value = FORMAT("{}({})", std::string{identifier}, params);
        if(_temp.empty()) {
            if(constructor) {
                value = FORMAT("std::make_shared<{}>({})", newType, params);
            } else if(!value.empty() && value.starts_with('_')) {
                value = FORMAT("v{}", value);
            } else {
                value = FORMAT("_{}", value);
            }
        }
        write(value, newType);
        return {};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleSquareExpression(TupType &type) noexcept {
        if(!Scope::checkVector(_type)) { return FORMAT("Indexing not allowed for {} type", _type); }
        auto factory = ExpressionFactory::create(_iterator, _end, _scope, _const, true);
        ++_iterator;
        if(auto error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        if(auto newType = expression.getType(); newType != ints) { return FORMAT("{} index not allowed", newType); }
        if(auto error = ExpressionFactory::checkType(type, _type); !error.empty()) { return error; }
        _const = false;
        write(FORMAT("at({})", expression.getText()), _type);
        return {};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleVectorInitialization(TupType &type) noexcept {
        std::string vectorType;
        std::string value;
        std::string constValue;
        std::vector<Expression> expressions;
        auto factory = ExpressionFactory::create(_iterator, _end, _scope, _const);
        while(!_iterator->isType(TokenType::CLOSE_CUR_PARENTESIS)) {
            using enum vnd::TokenType;
            ++_iterator;
            if(!_iterator->isType(CLOSE_CUR_PARENTESIS)) {
                if(auto error = factory.parse({COMMA, CLOSE_CUR_PARENTESIS}); !error.empty()) { return error; }
            }
        }
        for(const auto &expression : factory.getExpressions()) {
            const auto &exprType = expression.getType();
            bool assignable = false;
            // NOLINTBEGIN(*-branch-clone)
            if(vectorType.empty()) {
                vectorType = exprType;
                assignable = true;
            } else if(_scope->canAssign(vectorType, exprType)) {
                assignable = true;
            } else if(_scope->canAssign(exprType, vectorType)) {
                vectorType = exprType;
                assignable = true;
            }
            // NOLINTEND(*-branch-clone)
            if(!assignable) { return FORMAT("Incompatible types in vector {}, {}", vectorType, exprType); }
            if(expression.isConst()) {
                constValue += FORMAT("{}, ", expression.getValue());
            } else {
                _const = false;
            }
            value += FORMAT("{}, ", expression.getText());
        }
        if(!value.empty()) { value.erase(value.size() - 2, 2); }
        vectorType += "[]";
        if(auto error = checkType(type, vectorType); !error.empty()) { return error; }
        if(_const) {
            if(!constValue.empty()) { constValue.pop_back(); }
            _expressionText += FORMAT("{{{}}}", value);
        }
        write(FORMAT("{{{}}}", value), vectorType);
        return {};
    }

    std::string ExpressionFactory::handleInnerExpression(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        auto factory = ExpressionFactory::create(_iterator, _end, _scope, _const);
        ++_iterator;
        if(auto error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        auto newType = expression.getType();
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, expression.getType()); }
        if(auto error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        if(expression.isConst()) {
            _expressionText += expression.getValue();
        } else {
            _const = false;
        }
        write(FORMAT("({})", expression.getText()), newType);
        return {};
    }

    std::string ExpressionFactory::handleToken(TupType &type) noexcept {
        const auto newType = ExpressionFactory::getTokenType();
        auto iterValue = _iterator->getValue();
        if(!checkUnaryOperator(newType)) { return FORMAT("Cannot apply unary operator for {}", iterValue); }
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, iterValue); }
        if(auto error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        emplaceToken(newType);
        return {};
    }

    bool ExpressionFactory::isTokenOfType(const TokenVecIter &iterator, TokenType type) const noexcept {
        return !isEnd(iterator) && iterator->isType(type);
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::string ExpressionFactory::checkType(TupType &oldType, const std::string_view newType) noexcept {
        auto nxtIter = std::ranges::next(_iterator);
        if(newType == "dot" || isTokenOfType(nxtIter, TokenType::DOT_OPERATOR)) { return {}; }
        if(std::get<2>(oldType).find(sps) != std::string::npos) { return "Multiple return value functions must be used alone"; }
        if(newType.find(sps) != std::string::npos) {
            if(!std::get<2>(oldType).empty()) { return "Multiple return value functions must be used alone"; }
            std::get<2>(oldType) = newType;
            return {};
        }
        if(isTokenOfType(nxtIter, TokenType::OPEN_SQ_PARENTESIS)) { return ""; }
        if(newType == "nullptr" && !Scope::isNumber(std::get<2>(oldType))) {
            std::get<2>(oldType) = newType;
            return {};
        }
        if(_sq && !isSquareType(newType)) { return FORMAT("Type not allowed {}", newType); }
        if(std::get<2>(oldType).empty()) {
            if(newType == oprt) {
                std::get<2>(oldType) = ints;
                return {};
            }
            std::get<2>(oldType) = newType;
            return {};
        }
        if(std::get<2>(oldType) == nots && newType == bols) {
            std::get<2>(oldType) = newType;
            return {};
        }
        if(std::get<2>(oldType) == bols && newType == nots) { return ""; }
        if(std::get<2>(oldType) == newType) { return ""; }
        if(Scope::isNumber(std::get<2>(oldType)) && (Scope::isNumber(std::string{newType}) || newType == oprt)) { return {}; }
        if(newType == "equal") {
            std::get<0>(oldType) = true;
            return {};
        }
        if(newType == "boolean") { return handleBooleanType(oldType); }
        if(newType == "logical") { return handleLogicalType(oldType); }
        return FORMAT("Incompatible types: {}, {}", std::get<2>(oldType), newType);
    }

    // NOLINTNEXTLINE(*-easily-swappable-parameters)
    bool ExpressionFactory::checkNextToken(const std::string_view &type, const std::string &value) noexcept {
        auto nxtIter = std::ranges::next(_iterator);
        if(!isEnd(nxtIter) && (nxtIter->isType(TokenType::DOT_OPERATOR) || nxtIter->isType(TokenType::OPEN_SQ_PARENTESIS))) {
            _type = type;
            if(type == "string" || type.ends_with(']')) {
                _temp += value + ".";
            } else {
                _temp += value + "->";
            }
            _dot = true;
            ++_iterator;
            return true;
        }
        return false;
    }
    bool ExpressionFactory::checkUnaryOperator(const std::string_view &type) const noexcept {
        auto nxtIter = std::ranges::next(_iterator);
        return !_iterator->isType(TokenType::IDENTIFIER) || isEnd(nxtIter) || !nxtIter->isType(TokenType::UNARY_OPERATOR) ||
               (_temp.empty() && type == ints && !_scope->isConstant(_type, _iterator->getValue()));
    }

    void ExpressionFactory::checkOperators(std::string &value) noexcept {
        if(_iterator->isType(TokenType::MINUS_OPERATOR)) { return; }
        if(_divide) {
            value = FORMAT("double({})", value);
            _divide = false;
        }
        if(_operators.has_value()) {
            value = FORMAT("{})", value);
            auto nxtIter = std::ranges::next(_iterator);
            if(isEnd(nxtIter) || (nxtIter->getValue() != "^" && nxtIter->getValue() != "%")) {
                _text.emplace(_text.begin() + C_LL(_operators.value()), " ");
                _operators.reset();
            }
        }
    }
    bool ExpressionFactory::isEnd(const TokenVecIter &nxtIter) const noexcept { return nxtIter == _end; }

    void ExpressionFactory::write(const std::string &value, const std::string_view &type) noexcept {
        clearData();
        if(checkNextToken(type, value)) { return; }
        auto text = FORMAT("{}{}", _temp, value);
        checkOperators(text);
        if(!_text.empty()) { text = FORMAT(" {}", text); }
        _text.emplace_back(text);
        ++_iterator;
        _dot = false;
    }

    void ExpressionFactory::clearData() noexcept {
        if(!_dot) {
            _type.clear();
            _temp.clear();
        }
    }
    std::string ExpressionFactory::handleLogicalType(ExpressionFactory::TupType &oldType) const noexcept {
        if(std::get<2>(oldType) == bols || std::get<0>(oldType) == true) {
            std::get<0>(oldType) = false;
            std::get<1>(oldType) = true;
            std::get<2>(oldType).clear();
            return {};
        }
        return FORMAT("Cannot apply operator for {} type", std::get<2>(oldType));
    }
    std::string ExpressionFactory::handleBooleanType(ExpressionFactory::TupType &oldType) const noexcept {
        if(!Scope::isPrimitive(std::get<2>(oldType))) { return FORMAT("Cannot compare {} type", std::get<2>(oldType)); }
        std::get<0>(oldType) = true;
        return {};
    }
}  // namespace vnd
// NOLINTEND(*-include-cleaner, *-env33-c)

DISABLE_WARNINGS_POP()
