#include "Vandior/Expression.hpp"
#include "Vandior/Log.hpp"

namespace vnd {

    Expression::Expression(const std::vector<std::string> text) noexcept : _text(text) {}

    Expression Expression::create(const std::vector<std::string> text) noexcept { return {text}; }

    std::string Expression::getText() const noexcept { return std::accumulate(_text.begin(), _text.end(), std::string()); }

}  // namespace vnd