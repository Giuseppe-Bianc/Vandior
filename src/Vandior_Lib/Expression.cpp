#include "Vandior/Expression.hpp"
#include "Vandior/Log.hpp"

namespace vnd {

    Expression::Expression(const std::vector<std::string> text, const std::string type) noexcept : _text(text), _type(type) {}

    Expression Expression::create(const std::vector<std::string> text, const std::string type) noexcept { return {text, type}; }

    std::string Expression::getText() const noexcept { return std::accumulate(_text.begin(), _text.end(), std::string()); }

    std::string Expression::getType() const noexcept { return _type; }

}  // namespace vnd