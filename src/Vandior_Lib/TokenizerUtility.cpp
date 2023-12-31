#include "Vandior/TokenizerUtility.hpp"

namespace vnd {
    bool TokenizerUtility::isOperator(const char &aChar) {
        static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-', '^'};
        return operators.contains(aChar);
    }
    bool TokenizerUtility::isPlusOrMinus(const char &cara) { return cara == '+' || cara == '-'; }
}  // namespace vnd