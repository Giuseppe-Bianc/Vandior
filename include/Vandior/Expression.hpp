#pragma once
#include <vector>
#include "Token.hpp"

namespace vnd {

    class Expression {
    public:
        [[nodiscard]] static Expression create(std::vector<Token>::iterator &iterator, TokenType end,
                                               std::vector<Token>::iterator tokensEnd) noexcept;
        std::string getText() const noexcept;
    private:
        Expression(std::string text) noexcept;
        std::string _text;
        std::string _type;
    };

}  // namespace vnd