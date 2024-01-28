#include <ranges>
#include "Vandior/Expression.hpp"
#include "Vandior/Log.hpp"

namespace vnd {

    Expression::Expression(std::string text) noexcept : _text(text) {}

    Expression Expression::create(std::vector<Token>::iterator &iterator, TokenType end,
                                  std::vector<Token>::iterator tokensEnd) noexcept {
        std::string text = "";
        while(iterator != tokensEnd && iterator->getType() != end) {
            text += " ";
            text += iterator->getValue();
            iterator++;
        }
        return Expression(text);
    }

    std::string Expression::getText() const noexcept { return _text; }

}  // namespace vnd