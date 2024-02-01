#include <algorithm>
#include "Vandior/ExpressionFactory.hpp"

namespace vnd {
    
    std::vector<std::string> ExpressionFactory::_numberTypes = {"int", "float"};

    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope) noexcept
      : _iterator(iterator), _end(end), _scope(scope), _text({}), _expressions({}), _lastOperator('\0') {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope) noexcept {
        return {iterator, end, scope};
    }

    bool ExpressionFactory::isNumber(const std::string &type) noexcept {
        return std::ranges::find(ExpressionFactory::_numberTypes, type) != ExpressionFactory::_numberTypes.end();
    }

    std::string ExpressionFactory::checkType(std::tuple<bool, bool, std::string> &oldType, const std::string_view newType) noexcept {
        if(std::get<2>(oldType) == "") {
            std::get<2>(oldType) = newType;
            return "";
        }
        if(std::get<2>(oldType) == newType) { return ""; }
        if(ExpressionFactory::isNumber(std::get<2>(oldType)) && (ExpressionFactory::isNumber(std::string{newType}) || newType == "operator")) {
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

    std::string ExpressionFactory::parse(const TokenType &endToken) noexcept {
        _text = {};
        std::tuple<bool, bool, std::string> type = std::make_tuple(false, false, "");
        while(_iterator != _end && _iterator->getType() != endToken) {
            std::string_view newType = ExpressionFactory::getTokenType(*_iterator);
            std::string error;
            if(newType == "") { return FORMAT("Identifier {} not found", _iterator->getValue()); }
            if(error = ExpressionFactory::checkType(type, newType); error != "") { return error; }
            _text.emplace_back(" ");
            emplaceToken(*_iterator);
            _iterator++;
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

    std::string_view ExpressionFactory::getTokenType(const Token& token) const noexcept {
        using enum TokenType;
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
        case MINUS_OPERATOR:
            return "operator";
        case BOOLEAN_OPERATOR:
            return "boolean";
        case NOT_OPERATOR:
            return "not";
        case LOGICAL_OPERATOR:
            return "logical";
        }
        return "";
    }

    void ExpressionFactory::emplaceToken(const Token& token) noexcept {
        using enum TokenType;
        if(token.getType() == CHAR) {
            _text.emplace_back("'" + std::string{token.getValue()} + "'");
            return;
        }
        if(token.getType() == STRING) {
            _text.emplace_back("\"" + std::string{token.getValue()} + "\"");
            return;
        }
        if(token.getValue() == "^") {
            _text.emplace(_text.end() - 2, "std::pow(");
            _text.emplace_back(",");
            _lastOperator = '^';
            return;
        }
        _text.emplace_back(token.getValue());
        if(_lastOperator == '^') {
            _text.emplace_back(")");
            _lastOperator = '\0';
        }
    }

}  // namespace vnd