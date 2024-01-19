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
        } else if(vnd::TokenizerUtility::isComment(_input, position)) {
            tokens.emplace_back(handleComment());
        } else if(vnd::TokenizerUtility::isOperator(currentChar)) [[likely]] {
            handleOperators(tokens);
        } else if(vnd::TokenizerUtility::isDot(currentChar)) {
            tokens.emplace_back(handleDot());
        } else if(vnd::TokenizerUtility::isBrackets(currentChar)) [[likely]] {
            tokens.emplace_back(handleBrackets());
        } else if(vnd::TokenizerUtility::isApostrophe(currentChar)) [[likely]] {
            tokens.emplace_back(handleChar());
        } else if(vnd::TokenizerUtility::isQuotation(currentChar)) [[likely]] {
            tokens.emplace_back(handleString());
        } else if(vnd::TokenizerUtility::isComma(currentChar)) {
            tokens.emplace_back(TokenType::COMMA, ",", line, column - 1);
            incPosAndColumn();
        } else if(vnd::TokenizerUtility::isColon(currentChar)) {
            tokens.emplace_back(TokenType::COLON, ":", line, column - 1);
            incPosAndColumn();
        } else [[unlikely]] {
            handleError(std::string(1, currentChar), "Unknown Character");
        }
    }
    tokens.emplace_back(TokenType::EOFT, "", line, column);
    return tokens;
}
bool Tokenizer::positionIsInText() const noexcept { return position < _inputSize; }

bool Tokenizer::isalnumUnderscore(const char &cha) const noexcept { return std::isalnum(C_UC(cha)) || cha == '_'; }

Token Tokenizer::handleAlpha() {
    const auto start = position;
    TokenType type = TokenType::IDENTIFIER;
    while(positionIsInText() && isalnumUnderscore(_input[position])) { incPosAndColumn(); }
    const auto value = _input.substr(start, position - start);
    kewordType(value, type);
    return {type, value, line, column - value.size()};
}
void Tokenizer::kewordType(const std::string_view &value, TokenType &type) noexcept {
    using enum TokenType;
    if(value == "main") { type = K_MAIN; }
    if(value == "var" || value == "const") { type = K_VAR; }
    if(value == "if" || value == "while") { type = K_STRUCTURE; }
    if(value == "for") { type = K_FOR; }
    if(value == "fun") { type = K_FUN; }
    if(value == "return") { type = K_RETURN; }
    if(value == "true" || value == "false") { type = BOOLEAN; }
}

bool Tokenizer::inTextAndE() const noexcept { return positionIsInText() && std::toupper(_input[position]) == ECR; }

Token Tokenizer::handleDigits() {
    TokenType tokenType = TokenType::INTEGER;
    const auto start = position;
    extractDigits();
    if(positionIsInText() && _input[position] == PNT) {
        incPosAndColumn();
        extractDigits();
        if(inTextAndE()) {
            incPosAndColumn();
            extractExponent();
        }
        tokenType = TokenType::DOUBLE;
    }
    if(inTextAndE()) {
        incPosAndColumn();
        extractExponent();
        tokenType = TokenType::DOUBLE;
    }
    const auto value = _input.substr(start, position - start);
    return {tokenType, value, line, column - value.size()};
}
Token Tokenizer::handleComment() {
    if(_input[position + 1] == '/') { return handleSingleLineComment(); }
    if(_input[position + 1] == '*') { return handleMultiLineComment(); }
    return {TokenType::UNKNOWN, "", line, column};
}
Token Tokenizer::handleSingleLineComment() {
    const auto start = position;
    while(positionIsInText() && _input[position] != CNL) { incPosAndColumn(); }
    const auto value = _input.substr(start, position - start);
    return {TokenType::COMMENT, value, line, column - value.size()};
}
Token Tokenizer::handleMultiLineComment() {
    const auto start = position;
    const auto startColumn = column;
    while(_input[position] != '*' || _input[position + 1] != '/') {
        if(position + 2 == _inputSize) {
            const auto value = _input.substr(start, position - start + 1);
            return {TokenType::UNKNOWN, value, line, startColumn};
        }
        handleWhiteSpace();
    }
    incPosAndColumn();
    incPosAndColumn();
    const auto value = _input.substr(start, position - start);
    return {TokenType::COMMENT, value, line, startColumn};
}
Token Tokenizer::handleDot() {
    const auto start = position;
    auto type = TokenType::DOT_OPERATOR;
    incPosAndColumn();
    if(positionIsInText() && std::isdigit(_input[position])) {
        type = TokenType::DOUBLE;
        extractDigits();
        if(inTextAndE()) {
            incPosAndColumn();
            extractExponent();
        }
    }
    const auto value = _input.substr(start, position - start);
    return {type, value, line, column - value.size()};
}
void Tokenizer::extractExponent() noexcept {
    if(positionIsInText() && vnd::TokenizerUtility::isPlusOrMinus(_input[position])) { incPosAndColumn(); }
    extractDigits();
}
void Tokenizer::extractDigits() noexcept {
    while(positionIsInText() && isdigit(_input[position])) { incPosAndColumn(); }
}
void Tokenizer::incPosAndColumn() noexcept {
    position++;
    column++;
}
void Tokenizer::handleWhiteSpace() noexcept {
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
    const auto value = _input.substr(start, position - start);
    const auto type = getType(value);
    return {type, value, line, column - value.size()};
}

TokenType Tokenizer::getType(const std::string_view &value) const noexcept {
    switch(value[0]) {
        using enum TokenType;
    case '(':
        return OPEN_PARENTESIS;
    case ')':
        return CLOSE_PARENTESIS;
    case '[':
        return OPEN_SQ_PARENTESIS;
    case ']':
        return CLOSE_SQ_PARENTESIS;
    case '{':
        return OPEN_CUR_PARENTESIS;
    case '}':
        return CLOSE_CUR_PARENTESIS;
    default:
        return UNKNOWN;
    }
}
Token Tokenizer::handleChar() {
    incPosAndColumn();
    const auto start = position;
    std::string_view value{};
    while(!vnd::TokenizerUtility::isApostrophe(_input[position])) {
        if(position + 1 == _inputSize) {
            incPosAndColumn();
            value = _input.substr(start, position - start);
            return {TokenType::UNKNOWN, value, line, column - value.size()};
        }
        incPosAndColumn();
    }
    value = _input.substr(start, position - start);
    const auto colum = column - value.size();
    incPosAndColumn();
    return {TokenType::CHAR, value, line, colum};
}
Token Tokenizer::handleString() {
    const auto startColumn = column;
    incPosAndColumn();
    const auto start = position;
    std::string_view value{};
    while(!vnd::TokenizerUtility::isQuotation(_input[position])) {
        if(position + 1 == _inputSize) {
            incPosAndColumn();
            value = _input.substr(start, position - start);
            return {TokenType::UNKNOWN, value, line, startColumn};
        }
        incPosAndColumn();
    }
    value = _input.substr(start, position - start);
    incPosAndColumn();
    return {TokenType::STRING, value, line, startColumn};
}
void Tokenizer::extractVarLenOperator() {
    while(positionIsInText() && vnd::TokenizerUtility::isOperator(_input[position])) { incPosAndColumn(); }
}
TokenType Tokenizer::singoleCharOp(const char &view) noexcept {
    switch(view) {
        using enum TokenType;
    case '-':
        return MINUS_OPERATOR;
    case '=':
        return EQUAL_OPERATOR;
    case '<':
    case '>':
        return BOOLEAN_OPERATOR;
    case '!':
        return NOT_OPERATOR;
    case '+':
    case '*':
    case '/':
    case '^':
    case '|':
    case '&':
        return OPERATOR;
    default:
        return UNKNOWN;
    }
}

TokenType Tokenizer::multyCharOp(const std::string_view &view) noexcept {
    using enum TokenType;
    if(vnd::TokenizerUtility::isOperationEqual(view)) { return OPERATION_EQUAL; }
    if(vnd::TokenizerUtility::isBooleanOperator(view)) { return BOOLEAN_OPERATOR; }
    if(vnd::TokenizerUtility::isLogicalOperator(view)) { return LOGICAL_OPERATOR; }
    if(vnd::TokenizerUtility::isUnaryOperator(view)) { return UNARY_OPERATOR; }
    return UNKNOWN;
}

void Tokenizer::handleOperators(std::vector<Token> &tokens) {
    const auto start = position;
    extractVarLenOperator();
    auto value = _input.substr(start, position - start);
    while(!value.empty()) {
        Token token;
        if(value.size() > 1) { token = {multyCharOp(value.substr(0, 2)), value.substr(0, 2), line, column - 2}; }
        if(token.getType() == TokenType::UNKNOWN || value.size() == 1) {
            token = Token{singoleCharOp(value[0]), value.substr(0, 1), line, column - 1};
        }
        tokens.emplace_back(token);
        value = value.substr(token.getValue().size(), value.size() - token.getValue().size());
    }
}

void Tokenizer::handleError(const std::string &value, const std::string_view &errorMsg) {
    const auto &lineStart = findLineStart();
    const auto &lineEnd = findLineEnd();

    std::string contextLine = getContextLine(lineStart, lineEnd);
    std::string highlighting = getHighlighting(lineStart, value.length());
    std::string errorMessage = getErrorMessage(value, errorMsg, contextLine, highlighting);

    throw std::runtime_error(errorMessage);
}
std::size_t Tokenizer::findLineStart() const noexcept {  // NOLINT(*-include-cleaner)
    std::size_t lineStart = position;
    while(lineStart > 0 && _input[lineStart - 1] != CNL) { --lineStart; }  // NOLINT(*-include-cleaner)
    return lineStart;
}
std::size_t Tokenizer::findLineEnd() const noexcept {
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
