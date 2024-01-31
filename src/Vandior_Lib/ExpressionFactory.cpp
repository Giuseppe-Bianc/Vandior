#include "Vandior/ExpressionFactory.hpp"

namespace vnd {

    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                         std::shared_ptr<Scope> scope) noexcept
      : _iterator(iterator), _end(end), _scope(scope), _expressions({}) {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                std::shared_ptr<Scope> scope) noexcept {
        return {iterator, end, scope};
    }

    std::string ExpressionFactory::checkType(std::tuple<bool, bool, std::string> &oldType, const std::string_view newType) noexcept {
        if(std::get<2>(oldType) == "") {
            std::get<2>(oldType) = newType;
            return "";
        }
        if(std::get<2>(oldType) == newType) { return ""; }
        if((std::get<2>(oldType) == "int" || std::get<2>(oldType) == "float" || std::get<2>(oldType) == "operator") &&
           (newType == "int" || newType == "float" || newType == "operator")) {
            return "";
        }
        if(std::get<2>(oldType) == "logical" || newType == "logical") {
            std::get<2>(oldType) = "";
            return "";
        }
        return FORMAT("Incompatible types: {}, {}", std::get<2>(oldType), newType);
    }

    std::string ExpressionFactory::parse(const TokenType &endToken) noexcept {
        std::vector<std::string> text;
        std::tuple<bool, bool, std::string> type = std::make_tuple(false, false, "");
        while(_iterator != _end && _iterator->getType() != endToken) {
            std::string_view newType = ExpressionFactory::getTokenType(*_iterator);
            std::string error;
            if(newType == "") { return FORMAT("Identifier {} not found", _iterator->getValue()); }
            error = ExpressionFactory::checkType(type, newType);
            text.emplace_back(" ");
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
        case BOOLEAN:
            return "bool";
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

}  // namespace vnd