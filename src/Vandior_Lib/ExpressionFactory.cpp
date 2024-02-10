#include "Vandior/ExpressionFactory.hpp"
#include <algorithm>

namespace vnd {

    // NOLINTBEGIN
    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope, bool sq) noexcept
      : _iterator(iterator), _end(end), _scope(std::move(scope)), _text({}), _expressions({}), _power(), _divide(false),
        _dot(false), _sq(sq), _const(true), _expressionText(""), _type(""), _temp("") {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope, bool sq) noexcept {
        return {iterator, end, std::move(scope), sq};
    }
    // NOLINTEND

    bool ExpressionFactory::isSquareType(const std::string_view &type) noexcept { return type == "int" || type == "operator"; }

    std::string ExpressionFactory::parse(const std::vector<TokenType> &endToken) noexcept {  // NOLINT(*-no-recursion)
        _text = {};
        _expressionText = "";
        std::tuple<bool, bool, std::string> type = std::make_tuple(false, false, "");
        exprtk::expression<double> expression;
        exprtk::parser<double> parser;
        while(_iterator != _end && std::ranges::find(endToken, _iterator->getType()) == endToken.end()) {
            if(_iterator->getType() == TokenType::IDENTIFIER && _iterator + 1 != _end &&
               (_iterator + 1)->getType() == TokenType::OPEN_PARENTESIS) {
                if(std::string error = handleFun(type); !error.empty()) { return error; }
            } else if(_iterator->getType() == TokenType::OPEN_PARENTESIS) {
                if(std::string error = handleInnerExpression(type); !error.empty()) { return error; }
            } else if(_iterator->getType() == TokenType::OPEN_SQ_PARENTESIS) {
                if(std::string error = handleSquareExpression(type); !error.empty()) { return error; }
            } else if(_iterator->getType() == TokenType::OPEN_CUR_PARENTESIS) {
                if(std::string error = handleVectorInitialization(type); !error.empty()) { return error; }
            } else if(std::string error = handleToken(type); !error.empty()) {
                return error;
            }
        }
        if(Scope::isNumber(std::get<2>(type))) {
            bool ok = parser.compile(_expressionText, expression);
            std::string value = std::to_string(expression.value());
            _expressions.emplace_back(
                Expression::create(_text, std::get<2>(type), _const && ok, value));
        } else {
            _expressions.emplace_back(Expression::create(_text, std::get<2>(type), _const, _expressionText));
        }
        return "";
    }

    std::size_t ExpressionFactory::size() const noexcept { return _expressions.size(); }

    bool ExpressionFactory::empty() const noexcept { return _expressions.empty(); }

    Expression ExpressionFactory::getExpression() noexcept {
        Expression result = *_expressions.begin();
        _expressions.erase(_expressions.begin());
        return result;
    }

    std::vector<Expression> ExpressionFactory::getExpressions() noexcept {
        std::vector<Expression> result = _expressions;
        _expressions = {};
        return result;
    }

    std::string_view ExpressionFactory::getTokenType(const Token &token) noexcept {
        using enum TokenType;
        // NOLINTBEGIN
        switch(token.getType()) {
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
            _const = false;
            return _scope->getVariableType(_type, token.getValue());
        case OPERATOR:
            [[fallthrough]];
        case MINUS_OPERATOR:
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
        return "";
    }

    void ExpressionFactory::emplaceToken(const std::string_view &type) noexcept {
        using enum TokenType;
        if(!_dot) {
            _type.clear();
            _temp.clear();
        }
        if(_iterator->getType() == TokenType::DOT_OPERATOR) {
            _iterator++;
            return;
        }
        if(checkNextToken(std::string{type}, writeToken())) { return; }
        _dot = false;
        const auto value = _iterator->getValue();
        if(_iterator->getType() == CHAR) {
            _text.emplace_back(FORMAT("'{}'", std::string{_iterator->getValue()}));
            _expressionText += FORMAT("'{}'", std::string{_iterator->getValue()});
            _iterator++;
            return;
        }
        _text.emplace_back(" ");
        if(value == "^") {
            if(!_power.has_value()) { _power = _text.size() - 2; }
            if(_sq) {
                _text.emplace(_text.begin() + _power.value(), "int(std::pow(");
            } else {
                _text.emplace(_text.begin() + _power.value(), "std::pow(");
            }
            _text.emplace_back(",");
            _iterator++;
            return;
        }
        std::string text = _temp + writeToken();
        _expressionText += text;
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
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        std::vector<Expression> expressions;
        std::string newType;
        std::string text;
        _iterator++;
        _const = false;
        while(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            _iterator++;
            if(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
                if(std::string error = factory.parse({TokenType::COMMA, TokenType::CLOSE_PARENTESIS}); !error.empty()) {
                    return error;
                }
            }
        }
        expressions = factory.getExpressions();
        newType = _scope->getFunType(_type, identifier, expressions);
        if(newType.empty()) { return FORMAT("Function {}.{} not found", _type, identifier); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        for(const Expression &expression : expressions) { text += expression.getText() + ","; }
        if(!expressions.empty() && !text.empty()) {
            if(text.at(0) == ' ') { text.erase(0, 1); }
            text.pop_back();
        }
        std::string value = FORMAT(" {}({})", std::string{identifier}, text);
        if(_temp.empty()) {
            value.erase(0, 1);
            if(!value.empty() && value.at(0) == '_') {
                value = FORMAT("v{}", value);
            } else {
                value = FORMAT(" _{}", value);
            }
        }
        write(value, newType);
        return "";
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleSquareExpression(TupType &type) noexcept {
        std::string newType;
        if(!checkVector()) { return FORMAT("Indexing not allowed for {} type", _type); }
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope, true);
        _iterator++;
        if(std::string error = factory.parse({TokenType::CLOSE_SQ_PARENTESIS}); !error.empty()) { return error; }
        Expression expression = factory.getExpression();
        newType = expression.getType();
        if(!expression.isConst()) { _const = false; }
        if(newType != "int") { return FORMAT("{} index not allowed", newType); }
        if(std::string error = ExpressionFactory::checkType(type, _type); !error.empty()) { return error; }
        write(FORMAT("at({})", expression.getText().substr(1)), _type);
        return "";
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string ExpressionFactory::handleVectorInitialization(TupType& type) noexcept {
        std::string oldType = std::get<2>(type);
        std::string vectorType;
        std::string value;
        std::vector<Expression> expressions;
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        while(_iterator->getType() != TokenType::CLOSE_CUR_PARENTESIS) {
            _iterator++;
            if(_iterator->getType() != TokenType::CLOSE_CUR_PARENTESIS) {
                if(std::string error = factory.parse({TokenType::COMMA, TokenType::CLOSE_CUR_PARENTESIS}); !error.empty()) {
                    return error;
                }
            }
        }
        for(Expression& expression : factory.getExpressions()) {
            if(vectorType == "") {
                vectorType = expression.getType();
            } else if(!Scope::canAssign(vectorType, expression.getType())) {
                return FORMAT("Incompatible types in vector {}, {}", vectorType, expression.getType());
            }
            if(!expression.isConst()) { _const = false; }
            value += FORMAT("{},", expression.getText());
        }
        if(!value.empty()) {
            value.pop_back();
            if(value.at(0) == ' ') { value.erase(0, 1); }
        }
        vectorType = FORMAT("std::vector<{}>", vectorType);
        if(std::string error = checkType(type, vectorType); !error.empty()) { return error; }
        write(FORMAT("{{{}}}", value), vectorType);
        return "";
    }

    std::string ExpressionFactory::handleInnerExpression(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string newType;
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        _iterator++;
        if(std::string error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        Expression expression = factory.getExpression();
        newType = expression.getType();
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, expression.getType()); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        write(FORMAT(" ({})", expression.getText().substr(1)), newType);
        return "";
    }

    std::string ExpressionFactory::handleToken(TupType &type) noexcept {
        const auto newType = ExpressionFactory::getTokenType(*_iterator);
        if(newType.empty()) { return FORMAT("Identifier {}.{} not found", _type, _iterator->getValue()); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        emplaceToken(newType);
        return "";
    }

    bool ExpressionFactory::checkVector() noexcept {
        if(_type.starts_with("std::vector<")) {
            _type.erase(0, std::string_view("std::vector<").size());
            _type.pop_back();
            if(_type.back() == '>') { _type.pop_back(); }
            return true;
        }
        if(_type.starts_with("std::array<")) {
            int par = 0;
            _type.erase(0, std::string_view("std::array<").size());
            while(_type.back() != ',' || par != 0) {
                if(_type.back() == '(') {
                    par--;
                } else if(_type.back() == ')') {
                    par++;
                }
                _type.pop_back();
            }
            _type.pop_back();
            return true;
        }
        if(_type == "string.") {
            _type = "char";
            return true;
        }
        return false;
    }

    std::string ExpressionFactory::checkType(TupType &oldType, const std::string_view newType) noexcept {
        if(newType == "dot" || ((_iterator + 1) != _end && (_iterator + 1)->getType() == TokenType::DOT_OPERATOR)) { return ""; }
        if(((_iterator + 1) != _end && (_iterator + 1)->getType() == TokenType::OPEN_SQ_PARENTESIS)) { return ""; }
        if(_sq && !isSquareType(newType)) { return FORMAT("Type not allowed {}", newType); }
        if(std::get<2>(oldType).empty()) {
            if(newType == "operator") {
                std::get<2>(oldType) = "int";
                return "";
            }
            std::get<2>(oldType) = newType;
            return "";
        }
        if(std::get<2>(oldType) == "not" && newType == "bool") {
            std::get<2>(oldType) = newType;
            return "";
        }
        if(std::get<2>(oldType) == "bool" && newType == "not") { return ""; }
        if(std::get<2>(oldType) == newType) { return ""; }
        if(Scope::isNumber(std::get<2>(oldType)) && (Scope::isNumber(std::string{newType}) || newType == "operator")) {
            return "";
        }
        if(newType == "boolean") {
            std::get<0>(oldType) = true;
            return "";
        }
        if(newType == "logical") {
            if(std::get<2>(oldType) == "bool" || std::get<0>(oldType) == true) {
                std::get<0>(oldType) = false;
                std::get<1>(oldType) = true;
                std::get<2>(oldType) = "";
                return "";
            }
            return FORMAT("Cannot apply operator for {} type", std::get<2>(oldType));
        }
        return FORMAT("Incompatible types: {}, {}", std::get<2>(oldType), newType);
    }

    bool ExpressionFactory::checkNextToken(const std::string &type, const std::string &value) noexcept {
        if((_iterator + 1) != _end && ((_iterator + 1)->getType() == TokenType::DOT_OPERATOR ||
                                       (_iterator + 1)->getType() == TokenType::OPEN_SQ_PARENTESIS)) {
            _type = type + ".";
            _temp += value + ".";
            _dot = true;
            _iterator++;
            return true;
        }
        return false;
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
        _expressionText += _temp + value;
        if(!_dot) {
            _type.clear();
            _temp.clear();
        }
        if(checkNextToken(std::string{type}, value)) { return; }
        std::string text = _temp + value;
        checkOperators(text);
        _text.emplace_back(text);
        _iterator++;
        _dot = false;
    }

}  // namespace vnd
