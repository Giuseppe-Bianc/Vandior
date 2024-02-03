#pragma once
#include <vector>
#include "Token.hpp"

namespace vnd {

    class Expression {
    public:
        [[nodiscard]] static Expression create(const std::vector<std::string> &text, const std::string &type) noexcept;
        std::string getText() const noexcept;
        std::string getType() const noexcept;
    private:
        Expression(const std::vector<std::string> &text, const std::string type) noexcept;
        std::vector<std::string> _text;
        std::string _type;
    };

}  // namespace vnd