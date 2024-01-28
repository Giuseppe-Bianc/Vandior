#include "Vandior/Expression.hpp"
#include "Vandior/Log.hpp"

namespace vnd {

    Expression::Expression(const std::string text) noexcept : _text(text) {}

    Expression Expression::create(const std::string text) noexcept { return {text}; }

    std::string Expression::getText() const noexcept { return _text; }

}  // namespace vnd