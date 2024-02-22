#include "Vandior/Expression.hpp"

namespace vnd {

    // clang-format off
    // NOLINTNEXTLINE
    Expression::Expression(const std::string &text, const std::string &type, const bool isConst, const std::string &value) noexcept
      : _text(text), _type(type), _const(isConst), _value(value) {
    }

    // NOLINTNEXTLINE
    Expression Expression::create(const std::vector<std::string> &text, const std::string &type, const bool isConst, const std::string &value) noexcept {
        return {std::accumulate(text.begin(), text.end(), std::string()), type, isConst, value};
    }
    
    // NOLINTNEXTLINE
    Expression Expression::create(const std::vector<std::string> &text, const std::string &type, const bool isConst) noexcept {
        std::string textString = std::accumulate(text.begin(), text.end(), std::string());
        return {textString, type, isConst, textString};
    }
    // clang-format on

    // NOLINTNEXTLINE(*-include-cleaner)
    std::string Expression::getText() const noexcept { return std::accumulate(_text.begin(), _text.end(), std::string()); }

    // NOLINTNEXTLINE(*-include-cleaner)
    std::string Expression::getType() const noexcept { return _type; }

    bool Expression::isConst() const noexcept { return _const; }

    // NOLINTNEXTLINE(*-include-cleaner)
    std::string Expression::getValue() const noexcept { return _value; }

}  // namespace vnd
