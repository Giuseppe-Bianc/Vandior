#include "Vandior/ExpressionFactory.hpp"
#include <algorithm>

namespace vnd {

    // NOLINTBEGIN
    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope) noexcept
      : _iterator(iterator), _end(end), _scope(std::move(scope)), _text({}), _expressions({}), _power(0), _divide(false) {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope) noexcept {
        return {iterator, end, std::move(scope)};
    }
    // NOLINTEND

    std::string ExpressionFactory::checkType(TupType &oldType, const std::string_view newType) noexcept {
        if(std::get<2>(oldType).empty()) {
            std::get<2>(oldType) = newType;
            return "";
        }
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

    std::string ExpressionFactory::parse(const std::vector<TokenType> &endToken) noexcept {  // NOLINT(*-no-recursion)
        _text = {};
        std::tuple<bool, bool, std::string> type = std::make_tuple(false, false, "");
        while(_iterator != _end && std::ranges::find(endToken, _iterator->getType()) == endToken.end()) {
            if(_iterator->getType() == TokenType::IDENTIFIER && _iterator + 1 != _end &&
               (_iterator + 1)->getType() == TokenType::OPEN_PARENTESIS) {
                if(std::string error = handleFun(type); !error.empty()) { return error; }
            } else if(_iterator->getType() == TokenType::OPEN_PARENTESIS) {
                if(std::string error = handleInnerExpression(type); !error.empty()) { return error; }
            } else {
                if(std::string error = handleToken(type); !error.empty()) { return error; }
            }
        }
        _expressions.emplace_back(Expression::create(_text, std::get<2>(type)));
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

    std::string_view ExpressionFactory::getTokenType(const Token &token) const noexcept {
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
            return _scope->getVariableType(token.getValue());
        case OPERATOR:
            [[fallthrough]];
        case MINUS_OPERATOR:
            return "operator";
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

    void ExpressionFactory::emplaceToken() noexcept {
        using enum TokenType;
        std::string_view value = _iterator->getValue();
        if(_iterator->getType() == CHAR) {
            _text.emplace_back(FORMAT("'{}'", std::string{_iterator->getValue()}));
            //_text.emplace_back("'" + std::string{_iterator->getValue()} + "'");
            return;
        }
        if(_iterator->getType() == STRING) {
            _text.emplace_back(FORMAT(R"("{}")", std::string{_iterator->getValue()}));
            //_text.emplace_back("\"" + std::string{_iterator->getValue()} + "\"");
            return;
        }
        if(value == "^") {
            _text.emplace(_text.end() - 2, "std::pow(");
            _text.emplace_back(",");
            _power++;
            return;
        }
        _text.emplace_back(writeToken());
        if(value == "/") { _divide = true; }
    }

    std::string ExpressionFactory::writeToken() noexcept {
        auto value = std::string{_iterator->getValue()};

        if(_iterator->getType() == TokenType::IDENTIFIER) { return FORMAT("_{}", value); }
        if(_iterator->getType() == TokenType::INTEGER && value[0] == '#') {
            value.erase(0, 1);
            if(!value.empty() && value[0] == 'o') {
                value.erase(0, 1);
                return FORMAT("0{}", value);
            }
            return FORMAT("0x{}", value);
        }
        checkOperators(value);
        return value;
    }

    std::string ExpressionFactory::handleFun(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string_view identifier = _iterator->getValue();
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        std::vector<Expression> expressions;
        std::string newType;
        std::string text;
        _iterator++;
        while(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
            _iterator++;
            if(_iterator->getType() != TokenType::CLOSE_PARENTESIS) {
                if(std::string error = factory.parse({TokenType::COMMA, TokenType::CLOSE_PARENTESIS}); !error.empty()) {
                    return error;
                }
            }
        }
        expressions = factory.getExpressions();
        newType = _scope->getFunType(identifier, expressions);
        if(newType.empty()) { return FORMAT("Function {} not found", identifier); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        for(const Expression &expression : expressions) { text += expression.getText() + ","; }
        if(!expressions.empty() && !text.empty()) {
            if(text[0] == ' ') { text.erase(0, 1); }
            text.pop_back();
        }
        text = FORMAT(" _{}({})", std::string{identifier}, text);
        checkOperators(text);
        _text.emplace_back(text);
        _iterator++;
        return "";
    }

    std::string ExpressionFactory::handleInnerExpression(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string newType;
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        _iterator++;
        if(std::string error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        Expression expression = factory.getExpression();
        newType = expression.getType();
        if(newType.empty()) { return FORMAT("Identifier {} not found", expression.getType()); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        std::string text = FORMAT(" ({})", expression.getText().substr(1));
        checkOperators(text);
        _text.emplace_back(text);
        _iterator++;
        return "";
    }

    std::string ExpressionFactory::handleToken(TupType &type) noexcept {
        const auto newType = ExpressionFactory::getTokenType(*_iterator);
        if(newType.empty()) { return FORMAT("Identifier {} not found", _iterator->getValue()); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        _text.emplace_back(" ");
        emplaceToken();
        _iterator++;
        return "";
    }

    void ExpressionFactory::checkOperators(std::string &value) noexcept {
        if(_divide) {
            value = FORMAT("double({})", value);  // value = "double(" + value + ")";
            _divide = false;
        }
        if(_power != 0 && ((_iterator + 1) == _end || (_iterator + 1)->getValue() != "^")) {
            value = FORMAT("{}{:)^{}}", value, "", _power);  // value = value + std::string(_power, ')');
            _power = 0;
        }
    }

}  // namespace vnd
