#include "Vandior/ExpressionFactory.hpp"
#include <algorithm>

namespace vnd {

    // NOLINTNEXTLINE
    std::vector<std::string> ExpressionFactory::_numberTypes = {"int", "float"};

    // NOLINTBEGIN
    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator,
                                         std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope) noexcept
      : _iterator(iterator), _end(end), _scope(std::move(scope)), _text({}), _expressions({}), _lastOperator('\0') {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope) noexcept {
        return {iterator, end, std::move(scope)};
    }
    // NOLINTEND

    bool ExpressionFactory::isNumber(const std::string &type) noexcept {
        return std::ranges::find(ExpressionFactory::_numberTypes, type) != ExpressionFactory::_numberTypes.end();
    }

    std::string ExpressionFactory::checkType(TupType &oldType, const std::string_view newType) noexcept {
        if(std::get<2>(oldType).empty()) {
            std::get<2>(oldType) = newType;
            return "";
        }
        if(std::get<2>(oldType) == newType) { return ""; }
        if(ExpressionFactory::isNumber(std::get<2>(oldType)) &&
           (ExpressionFactory::isNumber(std::string{newType}) || newType == "operator")) {
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
            if(_iterator->getType() == TokenType::OPEN_PARENTESIS) {
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
            _text.emplace_back("'" + std::string{_iterator->getValue()} + "'");
            return;
        }
        if(_iterator->getType() == STRING) {
            _text.emplace_back("\"" + std::string{_iterator->getValue()} + "\"");
            return;
        }
        if(value == "^") {
            _text.emplace(_text.end() - 2, "std::pow(");
            _text.emplace_back(",");
            _lastOperator = '^';
            return;
        }
        _text.emplace_back(writeToken());
        if(_lastOperator != '\0') {
            _text.emplace_back(")");
            _lastOperator = '\0';
            return;
        }
        if(value == "/") {
            _text.emplace_back(" double(");
            _lastOperator = '/';
        }
    }

    std::string ExpressionFactory::writeToken() noexcept {
        std::string value = std::string{_iterator->getValue()};

        if(_lastOperator == '/') { _text.pop_back(); }
        if(_iterator->getType() == TokenType::IDENTIFIER) { return "_" + value; }
        if(_iterator->getType() == TokenType::INTEGER && value[0] == '#') {
            value.erase(0, 1);
            if(!value.empty() && value[0] == 'o') {
                value.erase(0, 1);
                return "0" + value;
            }
            return "0x" + value;
        }
        return value;
    }

    std::string ExpressionFactory::handleInnerExpression(TupType &type) noexcept {  // NOLINT(*-no-recursion)
        std::string error;
        std::string newType;
        ExpressionFactory factory = ExpressionFactory::create(_iterator, _end, _scope);
        _iterator++;
        if(error = factory.parse({TokenType::CLOSE_PARENTESIS}); !error.empty()) { return error; }
        _iterator++;
        Expression expression = factory.getExpression();
        newType = expression.getType();
        if(newType.empty()) { return FORMAT("Identifier {} not found", expression.getType()); }
        if(error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        _text.emplace_back(" ");
        _text.emplace_back("(" + expression.getText().substr(1) + ")");
        if(_lastOperator != '\0') {
            _text.emplace_back(")");
            _lastOperator = '\0';
        }
        return "";
    }

    std::string ExpressionFactory::handleToken(TupType &type) noexcept {
        std::string_view newType = ExpressionFactory::getTokenType(*_iterator);
        if(newType.empty()) { return FORMAT("Identifier {} not found", _iterator->getValue()); }
        if(std::string error = ExpressionFactory::checkType(type, newType); !error.empty()) { return error; }
        _text.emplace_back(" ");
        emplaceToken();
        _iterator++;
        return "";
    }

}  // namespace vnd
