#include "Vandior/ExpressionFactory.hpp"

namespace vnd {

    ExpressionFactory::ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end) noexcept
      : _iterator(iterator), _end(end), _expressions({}) {}

    ExpressionFactory ExpressionFactory::create(std::vector<Token>::iterator &iterator,
                                                std::vector<Token>::iterator end) noexcept {
        return {iterator, end};
    }

    void ExpressionFactory::parse(const TokenType &endToken) noexcept {
        std::string text = "";
        while(_iterator != _end && _iterator->getType() != endToken) {
            text += " ";
            text += _iterator->getValue();
            _iterator++;
        }
        _expressions.emplace_back(Expression::create(text));
    }

    bool ExpressionFactory::empty() const noexcept { return _expressions.empty(); }

    std::string ExpressionFactory::getExpression() noexcept {
        std::string result = _expressions.begin()->getText();
        _expressions.erase(_expressions.begin());
        return result;
    }

}  // namespace vnd