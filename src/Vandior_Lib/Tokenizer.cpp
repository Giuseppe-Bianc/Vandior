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
    while(positionIsInText() && (std::isalnum(_input[position]) || _input[position] == '_')) {
        position++;
        column++;
    }
    const auto end = position;
    std::string_view value = _input.substr(start, end - start);
    return {TokenType::IDENTIFIER, value, line, column - value.size()};
}
Token Tokenizer::handleDigits() {
    const auto start = position;
    while(positionIsInText() && std::isdigit(_input[position])) {
        position++;
        column++;
    }
    const auto end = position;
    std::string_view value = _input.substr(start, end - start);
    return {TokenType::INTEGER, value, line, column - value.size()};
}
void Tokenizer::handleWhiteSpace() {
    if(std::isspace(_input[position])) { position++; }
    if(_input[position] == '\n') {
        ++line;
        column = 1;
    } else {
        ++column;
    }
    ++position;
}
void Tokenizer::handleError(const std::string &value, const std::string_view &errorMsg) {
    Timer timerError{std::string{errorMsg}};

    const auto &lineStart = findLineStart();
    const auto &lineEnd = findLineEnd();

    std::string contextLine = getContextLine(lineStart, lineEnd);
    std::string highlighting = getHighlighting(lineStart, value.length());
    std::string errorMessage = getErrorMessage(value, errorMsg, contextLine, highlighting);

    LERROR("{}{}", errorMessage, timerError);
}
std::size_t Tokenizer::findLineStart() {  // NOLINT(*-include-cleaner)
    std::size_t lineStart = position;
    while(lineStart > 0 && _input[lineStart - 1] != CNL) { --lineStart; }  // NOLINT(*-include-cleaner)
    return lineStart;
}
std::size_t Tokenizer::findLineEnd() {
    std::size_t lineEnd = position;
    while(lineEnd < _inputSize && _input[lineEnd] != CNL) { ++lineEnd; }
    return lineEnd;
}
std::string Tokenizer::getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) {
    return std::string(_input.begin() + C_L(lineStart), _input.begin() + C_L(lineEnd)).append(NEWL);
}
std::string Tokenizer::getHighlighting(const std::size_t &start, const std::size_t &length) const { return FORMAT("{: ^{}}{:^{}}{}", "", position - start,"^",length,NEWL); }
std::string Tokenizer::getErrorMessage(const std::string &value, const std::string_view &errMsg, std::string contextLine,
                                       std::string highlighting) {
    std::ostringstream errorMessageStream;
    errorMessageStream << FORMAT("{} '{}' (line {}, column {}):{}", errMsg, value, line, column, NEWL);
    errorMessageStream << "Context:" << NEWL;
    errorMessageStream << contextLine;
    errorMessageStream << highlighting;
    return errorMessageStream.str();
}
