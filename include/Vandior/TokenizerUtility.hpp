#pragma once
#include "headers.hpp"

namespace vnd {

    class TokenizerUtility {
    public:
        [[nodiscard]] static bool isOperator(const char &aChar);
        [[nodiscard]] static bool isPlusOrMinus(const char &cara);
    };

}  // namespace vnd
