// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/lexer/TokenizerUtility.hpp"
using namespace std::literals::string_view_literals;

DISABLE_WARNINGS_PUSH(26446 26497)
namespace vnd {
    bool TokenizerUtility::isOperator(const char aChar) {
        static const std::unordered_set<char> operators = {starcr, slashcr, '=', '<', '>', '!', '|', '&', '+', '-', '^', '%'};
        return operators.contains(aChar);
    }
    bool TokenizerUtility::isPlusOrMinus(const char cara) noexcept { return cara == plusscr || cara == minuscs; }
    /*bool TokenizerUtility::isOperationEqual(const std::string_view &value) noexcept {
        return value == "+="sv || value == "-="sv || value == "*="sv || value == "/="sv || value == "^="sv || value == "%="sv;
    }
    bool TokenizerUtility::isBooleanOperator(const std::string_view &value) noexcept {
        return value == "=="sv || value == ">="sv || value == "<="sv || value == "!="sv;
    }
    bool TokenizerUtility::isLogicalOperator(const std::string_view &value) noexcept { return value == "&&"sv || value == "||"sv; }*/
    bool TokenizerUtility::isBrackets(const char cha) {
        static const std::unordered_set<char> operators = {'(', ')', '[', ']', '{', '}'};
        return operators.contains(cha);
    }
    // bool TokenizerUtility::isUnaryOperator(const std::string_view &value) noexcept { return value == "++"sv || value == "--"sv; }
    bool TokenizerUtility::isDot(const char cha) noexcept { return cha == '.'; }
    bool TokenizerUtility::isApostrophe(const char cha) noexcept { return cha == '\''; }
    bool TokenizerUtility::isQuotation(const char cha) noexcept { return cha == '\"'; }
    bool TokenizerUtility::isComma(const char cha) noexcept { return cha == commacr; }
    bool TokenizerUtility::isColon(const char cha) noexcept { return cha == coloncr; }
    bool TokenizerUtility::isComment(const std::string_view &inputSpan, const size_t position) noexcept {
        const auto nextpos = position + 1;
        return position != inputSpan.size() && inputSpan[position] == slashcr &&
               (inputSpan[nextpos] == slashcr || inputSpan[nextpos] == starcr);
    }
    bool TokenizerUtility::isalnumUnderscore(const char cha) noexcept { return C_BOOL(std::isalnum(C_UC(cha))) || cha == underore; }
    bool TokenizerUtility::isOctalDigit(const char cha) noexcept {
        return C_BOOL(std::isdigit(C_UC(cha))) && cha >= zerocr && cha <= sevencr;
    }
    bool TokenizerUtility::isHasterisc(const char cha) noexcept { return cha == '#'; }
    bool TokenizerUtility::isUnderscore(const char cha) noexcept { return cha == underore; }
}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
