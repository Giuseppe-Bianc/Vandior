#pragma once
#include <vector>
#include "Token.hpp"

namespace vnd {

    class Expression {
    public:
        [[nodiscard]] static Expression create(const std::string text) noexcept;
        std::string getText() const noexcept;
    private:
        Expression(const std::string text) noexcept;
        std::string _text;
        std::string _type;
    };

}  // namespace vnd