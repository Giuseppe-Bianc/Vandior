#include "Vandior/Expression.hpp"

namespace vnd {

    // NOLINTNEXTLINE
    Expression::Expression(const std::vector<std::string> &text, const std::string type, const bool isConst,
                           const std::string &value) noexcept
      : _text(text), _type(type), _const(isConst), _value(value) {
    }

    Expression Expression::create(const std::vector<std::string> &text, const std::string &type, const bool isConst,
                                  const std::string &value) noexcept {
        return {text, type, isConst, value};
    }

    std::string Expression::getText() const noexcept { return std::accumulate(_text.begin(), _text.end(), std::string()); }

    std::string Expression::getType() const noexcept { return _type; }

    bool Expression::isConst() const noexcept { return _const; }

    std::string Expression::getValue() const noexcept {
        return _value;
    }

}  // namespace vnd
