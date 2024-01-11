#include "Vandior/TokenizerUtility.hpp"

namespace vnd {
    bool TokenizerUtility::isOperator(const char &aChar) {
        static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-', '^'};
        return operators.contains(aChar);
    }
    bool TokenizerUtility::isPlusOrMinus(const char &cara) { return cara == '+' || cara == '-'; }
    bool TokenizerUtility::isOperationEqual(const std::string_view &value) {
        return value == "+=" || value == "-=" || value == "*=" || value == "/=";
    }
    bool TokenizerUtility::isBooleanOperator(const std::string_view &value) {
        return value == "==" || value == ">=" || value == "<=" || value == "!=";
    }
    bool TokenizerUtility::isLogicalOperator(const std::string_view &value) { return value == "&&" || value == "||"; }
    bool TokenizerUtility::isBrackets(const char &cha) {
        static const std::unordered_set<char> operators = {'(',')','[',']','{','}'};
        return operators.contains(cha);
    }
    bool TokenizerUtility::isUnaryOperator(const std::string_view &value) { return value == "++" || value == "--"; }
//    bool TokenizerUtility::isVarLenOperator(const std::string_view &value) {
//        if(value.size() == 1) { return isOperator(value[0]) || isBrackets(value[0]); }
//        return isOperationEqual(value) || isUnaryOperator(value) || isBooleanOperator(value) || isLogicalOperator(value);
//    }

}  // namespace vnd