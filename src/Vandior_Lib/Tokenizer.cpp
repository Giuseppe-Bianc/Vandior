#include "Vandior/Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize() {  // NOLINT(*-include-cleaner)
    std::vector<Token> tokens;
    while(positionIsInText()) {
        const char &currentChar = _input[position];
        if(std::isalpha(currentChar)) [[likely]] {
            tokens.emplace_back(handleAlpha());
        } else if(std::isdigit(currentChar)) [[likely]] {
            tokens.emplace_back(handleDigits());
        } else if(std::isspace(currentChar)) [[likely]] {
            handleWhiteSpace();
            continue;  // Continue the loop to get the next token
        } else if(vnd::TokenizerUtility::isOperator(currentChar)) [[likely]] {
            tokens.emplace_back(handleOperators());
        } else if(vnd::TokenizerUtility::isBrackets(currentChar)) [[likely]] {
            tokens.emplace_back(handleBrackets());
        } else [[unlikely]] {
            handleError(std::string(1, currentChar), "Unknown Character");
        }
    }
    tokens.emplace_back(TokenType::EOFT, "", line, column);
    return tokens;
}
bool Tokenizer::positionIsInText() const noexcept { return position < _inputSize; }
Token Tokenizer::handleAlpha() {
    const auto start = position;
    TokenType type = TokenType::IDENTIFIER;
    while(positionIsInText() && (std::isalnum(_input[position]) || _input[position] == '_')) { incPosAndColumn(); }
    std::string_view value = _input.substr(start, position - start);
    kewordType(value, type);
    return {type, value, line, column - value.size()};
}
void Tokenizer::kewordType(const std::string_view &value, TokenType &type) {
    using enum TokenType;
    if(value== "main") {type = K_MAIN;}
    if(value== "var" || value =="const") {type=K_VAR;}
    if(value== "if" || value == "while") {type=K_STRUCTURE;}
    if(value== "for") {type=K_FOR;}
    if(value== "fun") {type=K_FUN;}
    if(value== "return") {type=K_RETURN;}
    if(value== "true"||value=="false") {type=TokenType::BOOLEAN;}
}
Token Tokenizer::handleDigits() {
    TokenType tokenType = TokenType::INTEGER;
    const auto start = position;
    extractDigits();
    if(positionIsInText() && _input[position] == PNT) {
        incPosAndColumn();
        extractDigits();
        if(positionIsInText() && std::toupper(_input[position]) == ECR) {
            incPosAndColumn();
            extractExponent();
        }
        tokenType = TokenType::DOUBLE;
    }
    if(positionIsInText() && std::toupper(_input[position]) == ECR) {
        incPosAndColumn();
        extractExponent();
        tokenType = TokenType::DOUBLE;
    }
    std::string_view value = _input.substr(start, position - start);
    return {tokenType, value, line, column - value.size()};
}
void Tokenizer::extractExponent() {
    if(positionIsInText() && (vnd::TokenizerUtility::isPlusOrMinus(_input[position]))) { incPosAndColumn(); }
    extractDigits();
}
void Tokenizer::extractDigits() {
    while(positionIsInText() && isdigit(_input[position])) { incPosAndColumn(); }
}
void Tokenizer::incPosAndColumn() {
    position++;
    column++;
}
void Tokenizer::handleWhiteSpace() {
    if(_input[position] == '\n') {
        ++line;
        column = 1;
    } else {
        ++column;
    }
    ++position;
}
Token Tokenizer::handleBrackets() {
    const auto start = position;
    incPosAndColumn();
    std::string_view value = _input.substr(start, position - start);
    TokenType type{};
    switch(value[0]) {
    case '(':
        [[fallthrough]];
    case ')':
        type= TokenType::PARENTESIS;
        break;
    case '[':
        [[fallthrough]];
    case ']':
        type= TokenType::SQ_PARENTESIS;
        break;
    case '{':
        [[fallthrough]];
    case '}':
        type= TokenType::CUR_PARENTESIS;
        break;
    default:
        type= TokenType::UNKNOWN;
        break;
    }
    return {type, value, line, column - value.size()};
}
void Tokenizer::extractVarLenOperator() {
    while(positionIsInText() && vnd::TokenizerUtility::isOperator(_input[position])) { incPosAndColumn(); }
}
TokenType Tokenizer::singoleCharOp(const char &view) {
    if(vnd::TokenizerUtility::isOperator(view)) {
        return TokenType::OPERATOR;
    }
    return TokenType::UNKNOWN;
}

TokenType Tokenizer::multyCharOp(const std::string_view &view) const {
    using enum TokenType;
    if(vnd::TokenizerUtility::isOperationEqual(view)) { return OPERATION_EQUAL; }
    if(vnd::TokenizerUtility::isBooleanOperator(view)) { return BOOLEAN_OPERATOR; }
    if(vnd::TokenizerUtility::isLogicalOperator(view)) { return LOGICAL_OPERATOR; }
    if(vnd::TokenizerUtility::isUnaryOperator(view)) { return UNARY_OPERATOR; }
    return UNKNOWN;
}

Token Tokenizer::handleOperators() {
    const auto start = position;
    extractVarLenOperator();
    std::string_view value = _input.substr(start, position - start);
    TokenType type = TokenType::UNKNOWN;
    if(!value.empty()) {
        if(value.size() == 1) {
            type = singoleCharOp(value[0]);
        } else if(value.size() > 1) {
            type = multyCharOp(value);
        }
    }
    return {type, value, line, column - value.size()};
}

void Tokenizer::handleError(const std::string &value, const std::string_view &errorMsg) {
    const auto &lineStart = findLineStart();
    const auto &lineEnd = findLineEnd();

    std::string contextLine = getContextLine(lineStart, lineEnd);
    std::string highlighting = getHighlighting(lineStart, value.length());
    std::string errorMessage = getErrorMessage(value, errorMsg, contextLine, highlighting);

    throw std::runtime_error(errorMessage);
}
std::size_t Tokenizer::findLineStart() const {  // NOLINT(*-include-cleaner)
    std::size_t lineStart = position;
    while(lineStart > 0 && _input[lineStart - 1] != CNL) { --lineStart; }  // NOLINT(*-include-cleaner)
    return lineStart;
}
std::size_t Tokenizer::findLineEnd() const {
    std::size_t lineEnd = position;
    while(lineEnd < _inputSize && _input[lineEnd] != CNL) { ++lineEnd; }
    return lineEnd;
}
std::string Tokenizer::getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) const {
    return std::string(_input.begin() + C_L(lineStart), _input.begin() + C_L(lineEnd)).append(&CNL);
}
std::string Tokenizer::getHighlighting(const std::size_t &start, const std::size_t &length) const {
    return FORMAT("{: ^{}}{:^{}}{}", "", position - start, "^", length, CNL);
}
std::string Tokenizer::getErrorMessage(const std::string &value, const std::string_view &errMsg, const std::string &contextLine,
                                       const std::string &highlighting) {
    std::ostringstream errorMessageStream;
    errorMessageStream << FORMAT("{} '{}' (line {}, column {}):{}", errMsg, value, line, column, CNL);
    errorMessageStream << FORMAT("Context: {}", CNL);
    errorMessageStream << contextLine;
    errorMessageStream << highlighting;
    return errorMessageStream.str();
}
