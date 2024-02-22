#include "Vandior/Expression.hpp"

namespace vnd {
    // NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters , *-pass-by-value)
    Expression::Expression(const std::string &text, const std::string &type, const bool isConst,
                           const std::string &value) noexcept
      : _text(text), _type(type), _const(isConst), _value(value) {}

    Expression Expression::create(const std::vector<std::string> &text, const std::string &type, const bool isConst,
                                  const std::string &value) noexcept {
        return {std::accumulate(text.begin(), text.end(), std::string()), type, isConst, value};
    }

    Expression Expression::create(const std::vector<std::string> &text, const std::string &type, const bool isConst) noexcept {
        std::string textString = std::accumulate(text.begin(), text.end(), std::string());
        return {textString, type, isConst, textString};
    }

    std::string Expression::getText() const noexcept { return std::accumulate(_text.begin(), _text.end(), std::string()); }

    std::string Expression::getType() const noexcept { return _type; }

    bool Expression::isConst() const noexcept { return _const; }

    std::string Expression::getValue() const noexcept { return _value; }
    // NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-pass-by-value)
}  // namespace vnd
