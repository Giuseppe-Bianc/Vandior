#include "Vandior/ExpressionFactory.hpp"
#include <algorithm>

// NOLINTBEGIN(*-include-cleaner, *-env33-c)
#ifdef _MSC_VER
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

#define EXPRTK

namespace vnd {

    // NOLINTBEGIN
    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope, const bool isConst, const bool sq) noexcept
      : _iterator(iterator), _end(end), _scope(std::move(scope)), _text({}), _expressions({}), _power(), _divide(false),
        _dot(false), _sq(sq), _const(isConst), _expressionText(""), _type(""), _temp("") {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope, const bool isConst, bool sq) noexcept {
        return {iterator, end, std::move(scope), isConst, sq};
    }
    // NOLINTEND

    std::string ExpressionFactory::transpileFun(const std::vector<Expression> &expressions,
                                                std::optional<size_t> variadic) noexcept {
        std::string params;
        size_t pos = 0;
        for(const Expression &expression : expressions) {
            if(variadic.has_value() && pos == variadic.value()) { params += "{"; }
            params += expression.getText() + ",";
            pos++;
        }
        if(!params.empty()) {
            if(params.at(0) == ' ') { params.erase(0, 1); }
            params.pop_back();
        }
        if(variadic.has_value()) { params += "}"; }
        return params;
    }

    bool ExpressionFactory::isSquareType(const std::string_view &type) noexcept { return type == "int" || type == "operator"; }

    std::string ExpressionFactory::evaluate(const std::string &expression) noexcept {
        std::string command = FORMAT("python -c \"print({})\"", expression);

        double result = 0.0;
        std::unique_ptr<FILE, decltype(&PCLOSE)> pipe(POPEN(command.c_str(), "r"), PCLOSE);
        if(!pipe) { return "0"; }

        // NOLINTNEXTLINE(*-err34-c, *-pro-type-vararg, hicpp-vararg)
        if(std::fscanf(pipe.get(), "%lf", &result) != 1) { result = 0; }

        return std::to_string(result);
    }

    void ExpressionFactory::resetVariables() noexcept {
        _text = {};
        _expressionText = "";
        _power.reset();
        _divide = false;
        _dot = false;
        _type = "";
        _temp = "";
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::string ExpressionFactory::parse(const std::vector<TokenType> &endToken) noexcept {  // NOLINT(*-no-recursion)
        using enum vnd::TokenType;
        resetVariables();
        exprtk::parser<double> parser;
        auto type = std::make_tuple(false, false, std::string{});
        while(_iterator != _end && std::ranges::find(endToken, _iterator->getType()) == endToken.end()) {
            auto iterType = _iterator->getType();
            if(iterType == UNARY_OPERATOR) {
                _iterator++;
                continue;
            }
            if(iterType == IDENTIFIER && std::next(_iterator) != _end && std::next(_iterator)->getType() == OPEN_PARENTESIS) {
                if(auto error = handleFun(type); !error.empty()) { return error; };
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
        if(std::get<0>(type)) { std::get<2>(type) = "bool"; }
        handleFinalExpression(type);
        return {};
    }

    void ExpressionFactory::handleFinalExpression(const std::tuple<bool, bool, std::string> &type) noexcept {
#ifdef EXPRTK
        exprtk::parser<double> parser;
        exprtk::expression<double> expression;
        if(Scope::isNumber(std::get<2>(type))) {
            std::string value;
            if(_const) { parser.compile(_expressionText, expression); }
            value = std::to_string(expression.value());
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const, value));
        } else {
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const));
        }
#else
        if(Scope::isNumber(std::get<2>(type))) {
            std::string value;
            if(_const) { value = ExpressionFactory::evaluate(_expressionText); }
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const, value));
        } else {
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const));
        }
#endif
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
        return _expressions.front().getType().contains(' ');
    }

    std::string_view ExpressionFactory::getTokenType() noexcept {
        using enum TokenType;
        // NOLINTBEGIN
        switch(_iterator->getType()) {
        case INTEGER:
            return "int";
        case DOUBLE:
            return "float";
        case BOOLEAN:
            return "bool";
        case CHAR:
            return "char";
        case STRING:
            return "string";
        case IDENTIFIER:
            return _scope->getVariableType(_type, _iterator->getValue());
        case OPERATOR:
            [[fallthrough]];
        case MINUS_OPERATOR:
        case UNARY_OPERATOR:
            return "operator";
        case DOT_OPERATOR:
            return "dot";
        case BOOLEAN_OPERATOR:
            return "boolean";
        case NOT_OPERATOR:
            return "not";
        case LOGICAL_OPERATOR:
            return "logical";
        default:
            break;
        }
        // NOLINTEND
        return {};
    }

    void ExpressionFactory::emplaceToken(const std::string_view &type) noexcept {
        using enum TokenType;
        clearData();
        if(_iterator->getType() == TokenType::DOT_OPERATOR) {
            _iterator++;
            return;
        }
        if(checkNextToken(std::string{type}, writeToken())) { return; }
        _dot = false;
        const auto value = _iterator->getValue();
        if(_iterator->getType() == CHAR) {
            _text.emplace_back(FORMAT("'{}'", std::string{value}));
            _expressionText += FORMAT("'{}'", std::string{value});
            _iterator++;
            return;
        }
        _text.emplace_back(" ");
        if(value == "^") {
            if(!_power.has_value()) { _power = _text.size() - 2; }
            auto textIndex = _text.begin() + C_LL(_power.value());
            if(_sq) {
                _text.emplace(textIndex, "int(std::pow(");
            } else {
                _text.emplace(textIndex, "std::pow(");
            }
            _text.emplace_back(",");
#ifdef EXPRTK
            _expressionText += value;
#else
            _expressionText += "**";
#endif
            _iterator++;
            return;
        }
        std::string text = _scope->getTmp(_temp + writeToken());
        if(_const) {
            if(_iterator->getType() == TokenType::IDENTIFIER) {
                std::string constValue = _scope->getConstValue(_type, _iterator->getValue());
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
        _text.emplace_back(text);
        if(value == "/" && !_sq) { _divide = true; }
        _iterator++;
    }

    std::string ExpressionFactory::writeToken() noexcept {
        auto value = std::string{_iterator->getValue()};
        if(_iterator->getType() == TokenType::STRING) { value = FORMAT(R"(string("{}"))", std::string{_iterator->getValue()}); }
        if(_iterator->getType() == TokenType::IDENTIFIER) {
            if(_temp.empty()) {
                if(!value.empty() && value.at(0) == '_') {
                    value = FORMAT("v{}", value);
                } else {
                    value = FORMAT(" _{}", value);
                }
            } else {
                value = FORMAT("get{}{}()", char(std::toupper(value[0])), value.substr(1));
            }
            if((_iterator + 1) != _end && (_iterator + 1)->getType() == TokenType::UNARY_OPERATOR) {
                value += (_iterator + 1)->getValue();
            }
        }
        if(_iterator->getType() == TokenType::INTEGER && value.at(0) == '#') {
            value.erase(0, 1);
            if(!value.empty() && value.at(0) == 'o') {
                value.erase(0, 1);
                return FORMAT("0{}", value);
            }
            return FORMAT("0x{}", value);
        }
        return value;
    }

    std::string ExpressionFactory::handleFun(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string_view identifier = _iterator->getValue();
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope, false);
        std::vector<Expression> expressions;
        std::string params;
        if(_const && !_temp.empty()) { return "Cannot call methods on const value"; }
        _iterator++;
        while(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            _iterator++;
            if(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
                if(std::string error = factory.parse({TokenType::COMMA, TokenType::CLOSE_PARENTESIS}); !error.empty()) {
                    return error;
                }
            }
        }
        if((_iterator + 1) == _end || (_iterator + 1)->getType() != TokenType::DOT_OPERATOR) { _const = false; }
        expressions = factory.getExpressions();
        auto [newType, constructor, variadic] = _scope->getFunType(_type, identifier, expressions);
        if(newType.empty()) {
            std::string value;
            std::string paramTypes;
            for(const Expression &expression : expressions) { paramTypes += expression.getType() + ","; }
            if(!paramTypes.empty()) { paramTypes.pop_back(); }
            value = FORMAT("{}.{}({})", _type, identifier, paramTypes);
            if(value.starts_with(".")) { value.erase(0, 1); }
            return FORMAT("Function {} not found", value);
        }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        params = ExpressionFactory::transpileFun(expressions, variadic);
        std::string value = FORMAT(" {}({})", std::string{identifier}, params);
        if(_temp.empty()) {
            value.erase(0, 1);
            if(constructor) {
                value = FORMAT("std::make_shared<{}>({})", newType, params);
            } else if(!value.empty() && value.at(0) == '_') {
                value = FORMAT("v{}", value);
            } else {
                value = FORMAT(" _{}", value);
            }
        }
        write(value, newType);
        return {};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleSquareExpression(TupType &type) noexcept {
        std::string newType;
        if(!Scope::checkVector(_type)) { return FORMAT("Indexing not allowed for {} type", _type); }
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope, _const, true);
        _iterator++;
        if(std::string error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS}); !error.empty()) { return error; }
        Expression expression = factory.getExpression();
        newType = expression.getType();
        if(newType != "int") { return FORMAT("{} index not allowed", newType); }
        if(auto error = ExpressionFactory::checkType(type, _type); !error.empty()) { return error; }
        _const = false;
        write(FORMAT("at({})", expression.getText().substr(1)), _type);
        return {};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleVectorInitialization(TupType &type) noexcept {
        std::string oldType;
        std::string vectorType;
        std::string value;
        std::string constValue;
        std::vector<Expression> expressions;
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope, _const);
        while(_iterator->getType() != TokenType::CLOSE_CUR_PARENTESIS) {
            using enum vnd::TokenType;
            _iterator++;
            if(_iterator->getType() != CLOSE_CUR_PARENTESIS) {
                if(std::string error = factory.parse({COMMA, CLOSE_CUR_PARENTESIS}); !error.empty()) { return error; }
            }
        }
        for(const Expression &expression : factory.getExpressions()) {
            if(vectorType.empty()) {
                vectorType = expression.getType();
            } else if(!_scope->canAssign(vectorType, expression.getType())) {
                return FORMAT("Incompatible types in vector {}, {}", vectorType, expression.getType());
            }
            if(expression.isConst()) {
                constValue += FORMAT("{},", expression.getValue());
            } else {
                _const = false;
            }
            value += FORMAT("{},", expression.getText());
        }
        if(!value.empty()) {
            value.pop_back();
            if(value.at(0) == ' ') { value.erase(0, 1); }
        }
        oldType = vectorType;
        vectorType += "[]";
        if(std::string error = checkType(type, vectorType); !error.empty()) { return error; }
        if(_const) {
            if(!constValue.empty()) { constValue.pop_back(); }
            _expressionText += FORMAT("{{{}}}", value);
        }
        write(FORMAT("{{{}}}", value), vectorType);
        return {};
    }

    std::string ExpressionFactory::handleInnerExpression(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string newType;
        auto factory = ExpressionFactory::create(_iterator, _end, _scope, _const);
        _iterator++;
        if(std::string error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        auto expression = factory.getExpression();
        newType = expression.getType();
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, expression.getType()); }
        if(auto error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        if(expression.isConst()) {
            _expressionText += expression.getValue();
        } else {
            _const = false;
        }
        write(FORMAT(" ({})", expression.getText().substr(1)), newType);
        return {};
    }

    std::string ExpressionFactory::handleToken(TupType &type) noexcept {
        const auto newType = ExpressionFactory::getTokenType();
        if(!checkUnaryOperator(newType)) { return FORMAT("Cannot apply unary operator for {}", _iterator->getValue()); }
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, _iterator->getValue()); }
        if(auto error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        emplaceToken(newType);
        return {};
    }

    // NOLINTNEXTLINE(*-function-cognitive-complexity)
    std::string ExpressionFactory::checkType(TupType &oldType, const std::string_view newType) noexcept {
        if(newType == "dot" || (std::next(_iterator) != _end && std::next(_iterator)->getType() == TokenType::DOT_OPERATOR)) {
            return {};
        }
        if(std::get<2>(oldType).contains(' ')) { return "Multiple return value functions must be used alone"; }
        if(newType.contains(' ')) {
            if(!std::get<2>(oldType).empty()) { return "Multiple return value functions must be used alone"; }
            std::get<2>(oldType) = newType;
            return {};
        }
        if(std::next(_iterator) != _end && std::next(_iterator)->getType() == TokenType::OPEN_SQ_PARENTESIS) { return ""; }
        if(_sq && !isSquareType(newType)) { return FORMAT("Type not allowed {}", newType); }
        if(std::get<2>(oldType).empty()) {
            if(newType == "operator") {
                std::get<2>(oldType) = "int";
                return {};
            }
            std::get<2>(oldType) = newType;
            return {};
        }
        if(std::get<2>(oldType) == "not" && newType == "bool") {
            std::get<2>(oldType) = newType;
            return {};
        }
        if(std::get<2>(oldType) == "bool" && newType == "not") { return ""; }
        if(std::get<2>(oldType) == newType) { return ""; }
        if(Scope::isNumber(std::get<2>(oldType)) && (Scope::isNumber(std::string{newType}) || newType == "operator")) {
            return {};
        }
        if(newType == "boolean") {
            std::get<0>(oldType) = true;
            return {};
        }
        if(newType == "logical") {
            if(std::get<2>(oldType) == "bool" || std::get<0>(oldType) == true) {
                std::get<0>(oldType) = false;
                std::get<1>(oldType) = true;
                std::get<2>(oldType) = "";
                return {};
            }
            return FORMAT("Cannot apply operator for {} type", std::get<2>(oldType));
        }
        return FORMAT("Incompatible types: {}, {}", std::get<2>(oldType), newType);
    }

    // NOLINTNEXTLINE(*-easily-swappable-parameters)
    bool ExpressionFactory::checkNextToken(const std::string &type, const std::string &value) noexcept {
        if(std::next(_iterator) != _end && (std::next(_iterator)->getType() == TokenType::DOT_OPERATOR ||
                                            std::next(_iterator)->getType() == TokenType::OPEN_SQ_PARENTESIS)) {
            _type = type;
            if(type == "string" || type.back() == ']') {
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
        return _iterator->getType() != TokenType::IDENTIFIER || (_iterator + 1) == _end ||
               (_iterator + 1)->getType() != TokenType::UNARY_OPERATOR ||
               (_temp.empty() && type == "int" && !_scope->isConstant(_type, _iterator->getValue()));
    }

    void ExpressionFactory::checkOperators(std::string &value) noexcept {
        if(_iterator->getType() == TokenType::MINUS_OPERATOR) { return; }
        if(_divide) {
            value = FORMAT("double({})", value);
            _divide = false;
        }
        if(_power.has_value()) {
            value = FORMAT("{})", value);
            if(_sq) { value = FORMAT("{})", value); }
        }
        if(((_iterator + 1) == _end || (_iterator + 1)->getValue() != "^")) { _power.reset(); }
    }

    void ExpressionFactory::write(const std::string &value, const std::string_view &type) noexcept {
        clearData();
        if(checkNextToken(std::string{type}, value)) { return; }
        std::string text = _scope->getTmp(_temp + value);
        checkOperators(text);
        _text.emplace_back(text);
        _iterator++;
        _dot = false;
    }
    void ExpressionFactory::clearData() noexcept {
        if(!_dot) {
            _type.clear();
            _temp.clear();
        }
    }
}  // namespace vnd
// NOLINTEND(*-include-cleaner, *-env33-c)
