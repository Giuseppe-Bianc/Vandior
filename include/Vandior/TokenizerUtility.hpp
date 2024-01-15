#pragma once
#include "headers.hpp"

namespace vnd {

    class TokenizerUtility {
    public:
        [[nodiscard]] static bool isOperator(const char &aChar);
        [[nodiscard]] static bool isPlusOrMinus(const char &cara) noexcept;
        [[nodiscard]] static bool isOperationEqual(const std::string_view &value) noexcept;
        [[nodiscard]] static bool isBooleanOperator(const std::string_view &value) noexcept;
        [[nodiscard]] static bool isLogicalOperator(const std::string_view &value) noexcept;
        [[nodiscard]] static bool isUnaryOperator(const std::string_view &value) noexcept;
        [[nodiscard]] static bool isDot(const char &cha) noexcept;
        [[nodiscard]] static bool isBrackets(const char &cha);
        [[nodiscard]] static bool isApostrophe(const char &cha) noexcept;
        [[nodiscard]] static bool isQuotation(const char &cha) noexcept;
        [[nodiscard]] static bool isComma(const char &cha) noexcept;
        [[nodiscard]] static bool isColon(const char &cha) noexcept;
        [[nodiscard]] static bool isComment(const std::string_view &inputSpan, size_t position) noexcept;
    };

}  // namespace vnd
