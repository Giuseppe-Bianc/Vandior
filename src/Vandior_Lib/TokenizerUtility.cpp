#include "Vandior/TokenizerUtility.hpp"

namespace vnd {
    bool TokenizerUtility::isOperator(const char &aChar) {
        static const std::unordered_set<char> operators = {'*', '/', '=', '<', '>', '!', '|', '&', '+', '-', '^', '%'};
        return operators.contains(aChar);
    }
    bool TokenizerUtility::isPlusOrMinus(const char &cara) noexcept { return cara == '+' || cara == '-'; }
    bool TokenizerUtility::isOperationEqual(const std::string_view &value) noexcept {
        return value == "+=" || value == "-=" || value == "*=" || value == "/=" || value == "^=" || value == "%=";
    }
    bool TokenizerUtility::isBooleanOperator(const std::string_view &value) noexcept {
        return value == "==" || value == ">=" || value == "<=" || value == "!=";
    }
    bool TokenizerUtility::isLogicalOperator(const std::string_view &value) noexcept { return value == "&&" || value == "||"; }
    bool TokenizerUtility::isBrackets(const char &cha) {
        static const std::unordered_set<char> operators = {'(', ')', '[', ']', '{', '}'};
        return operators.contains(cha);
    }
    bool TokenizerUtility::isUnaryOperator(const std::string_view &value) noexcept { return value == "++" || value == "--"; }
    bool TokenizerUtility::isDot(const char &cha) noexcept { return cha == '.'; }
    bool TokenizerUtility::isApostrophe(const char &cha) noexcept { return cha == '\''; }
    bool TokenizerUtility::isQuotation(const char &cha) noexcept { return cha == '\"'; }
    bool TokenizerUtility::isComma(const char &cha) noexcept { return cha == ','; }
    bool TokenizerUtility::isColon(const char &cha) noexcept { return cha == ':'; }
    bool TokenizerUtility::isComment(const std::string_view &inputSpan, size_t position) noexcept {
        return position != inputSpan.size() && inputSpan[position] == '/' &&
               (inputSpan[position + 1] == '/' || inputSpan[position + 1] == '*');
    }
    bool TokenizerUtility::isalnumUnderscore(const char &cha) noexcept { return std::isalnum(C_UC(cha)) || cha == '_'; }
    bool TokenizerUtility::isOctalDigit(const char &cha) noexcept { return std::isdigit(C_UC(cha)) && cha >= '0' && cha <= '7'; }
}  // namespace vnd
