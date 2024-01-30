#pragma once
#include <vector>
#include "Token.hpp"

namespace vnd {

    class Expression {
    public:
        [[nodiscard]] static Expression create(const std::vector<std::string> text) noexcept;
        std::string getText() const noexcept;
    private:
        Expression(const std::vector<std::string> text) noexcept;
        std::vector<std::string> _text;
        std::string _type;
    };

}  // namespace vnd