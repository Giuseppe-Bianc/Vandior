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
        } else if(isOperator(currentChar)) [[likely]] {
            tokens.emplace_back(handleOperators());
        } else [[unlikely]] {
            handleError(std::string(1, currentChar), "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }
    tokens.emplace_back(TokenType::EOFT, "", line, column);
    return tokens;
}
bool Tokenizer::positionIsInText() const noexcept { return position < _inputSize; }
Token Tokenizer::handleAlpha() {
    const auto start = position;
    while(positionIsInText() && (std::isalnum(_input[position]) || _input[position] == '_')) { incPosAndColumn(); }
    std::string_view value = _input.substr(start, position - start);
    return {TokenType::IDENTIFIER, value, line, column - value.size()};
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
    if(positionIsInText() && (isPlusOrMinus(_input[position]))) { incPosAndColumn(); }
    extractDigits();
}
bool Tokenizer::isPlusOrMinus(const char &cara) const { return cara == '+' || cara == '-'; }
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
// NOLINTBEGIN
bool Tokenizer::isOperator(const char &aChar) {
    static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-', '^'};
    return operators.contains(aChar);
}
Token Tokenizer::handleOperators() {
    const auto start = position;
    incPosAndColumn();
    std::string_view value = _input.substr(start, position - start);
    return {TokenType::OPERATOR, value, line, column - value.size()};
}
// NOLINTEND

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
