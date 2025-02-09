// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/lexer/TokenizerUtility.hpp"
using namespace std::literals::string_view_literals;

DISABLE_WARNINGS_PUSH(26446 26497)
namespace vnd {
    static inline constexpr std::array<char, 12> operators = {starcr, slashcr, '=', '<', '>', '!', '|', '&', '+', '-', '^', '%'};
    static inline constexpr std::array<char, 6> brackets = {'(', ')', '[', ']', '{', '}'};

    bool TokenizerUtility::isOperator(const char aChar) { return std::ranges::find(operators, aChar) != operators.end(); }
    bool TokenizerUtility::isPlusOrMinus(const char cara) noexcept { return cara == plusscr || cara == minuscs; }
    bool TokenizerUtility::isBrackets(const char cha) { return std::ranges::find(brackets, cha) != brackets.end(); }
    bool TokenizerUtility::isDot(const char cha) noexcept { return cha == '.'; }
    bool TokenizerUtility::isApostrophe(const char cha) noexcept { return cha == '\''; }
    bool TokenizerUtility::isQuotation(const char cha) noexcept { return cha == '\"'; }
    bool TokenizerUtility::isComment(const std::string_view &inputSpan, const size_t position) noexcept {
        const auto nextpos = position + 1;
        return position != inputSpan.size() && inputSpan[position] == slashcr &&
               (inputSpan[nextpos] == slashcr || inputSpan[nextpos] == starcr);
    }
    bool TokenizerUtility::isalnumUnderscore(const char cha) noexcept { return C_BOOL(std::isalnum(C_UC(cha))) || cha == underscore; }
    bool TokenizerUtility::isOctalDigit(const char cha) noexcept {
        return C_BOOL(std::isdigit(C_UC(cha))) && cha >= zerocr && cha <= sevencr;
    }
    bool TokenizerUtility::isHasterisc(const char cha) noexcept { return cha == '#'; }
    bool TokenizerUtility::isUnderscore(const char cha) noexcept { return cha == underscore; }
    bool TokenizerUtility::isCommaColon(const char cha) noexcept { return cha == commacr || cha == coloncr; }
    TokenType TokenizerUtility::CommaOrColonType(const char cha) noexcept {
        using enum TokenType;
        if(cha == commacr) {
            return COMMA;
        } else if(cha == coloncr) {
            return COLON;
        }
        return UNKNOWN;
    }
    std::string_view TokenizerUtility::CommaOrColonValue(const char cha) noexcept {
        if(cha == commacr) {
            return comma;
        } else if(cha == coloncr) {
            return colon;
        }
        return "unknown"sv;
    }
}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
