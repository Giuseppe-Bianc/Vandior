#include "Vandior/ExpressionFactory.hpp"

namespace vnd {

    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope) noexcept
      : _iterator(iterator), _end(end), _scope(scope), _expressions({}) {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope) noexcept {
        return {iterator, end, scope};
    }

    std::string ExpressionFactory::parse(const TokenType &endToken) noexcept {
        std::vector<std::string> text;
        std::string type = "";
        while(_iterator != _end && _iterator->getType() != endToken) {
            text.emplace_back(" ");
            std::string error = checkType(type, getTokenType(*_iterator));
            if(error != "") { return error; }
            text.emplace_back(_iterator->getValue());
            _iterator++;
        }
        _expressions.emplace_back(Expression::create(text));
        return "";
    }

    std::size_t ExpressionFactory::size() const noexcept { return _expressions.size(); }

    bool ExpressionFactory::empty() const noexcept { return _expressions.empty(); }

    std::string ExpressionFactory::getExpression() noexcept {
        std::string result = _expressions.begin()->getText();
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
        case IDENTIFIER:
            return _scope->getVariableType(token.getValue());
        case OPERATOR:
        case MINUS_OPERATOR:
            return "operator";
        case NOT_OPERATOR:
            return "not";
        case LOGICAL_OPERATOR:
            return "logical";
        }
        return "";
    }

    std::string ExpressionFactory::checkType(std::string &oldType, const std::string_view newType) noexcept {
        if(oldType == "") {
            oldType = newType;
            return "";
        }
        if(oldType == newType) { return ""; }
        if((oldType == "int" || oldType == "float" || oldType == "operator") &&
           (newType == "int" || newType == "float" || newType == "operator")) {
            return "";
        }
        if(oldType == "logical" || newType == "logical") {
            oldType = "";
            return "";
        }
        return FORMAT("Incompatible types: {}, {}", oldType, newType);
    }

}  // namespace vnd