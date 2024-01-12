#pragma once
#include "headers.hpp"

namespace vnd {

    class TokenizerUtility {
    public:
        [[nodiscard]] static bool isOperator(const char &aChar);
        [[nodiscard]] static bool isPlusOrMinus(const char &cara);
        [[nodiscard]] static bool isOperationEqual(const std::string_view &value);
        [[nodiscard]] static bool isBooleanOperator(const std::string_view &value);
        [[nodiscard]] static bool isLogicalOperator(const std::string_view &value);
        [[nodiscard]] static bool isUnaryOperator(const std::string_view &value);
        [[nodiscard]] static bool isBrackets(const char &cha);
    };

}  // namespace vnd
