#include "Vandior/Tokenizer.hpp"
using namespace std::literals::string_view_literals;
namespace vnd {
    // NOLINTBEGIN(*-include-cleaner)
    std::vector<Token> Tokenizer::tokenize() {
        std::vector<Token> tokens;
        while(positionIsInText()) {
            const char &currentChar = _input.at(position);
            if(std::isalpha(currentChar)) [[likely]] {
                tokens.emplace_back(handleAlpha());
            } else if(std::isdigit(currentChar)) [[likely]] {
                tokens.emplace_back(handleDigits());
            } else if(currentChar == '_') [[likely]] {
                tokens.emplace_back(handleUnderscoreAlpha());
            } else if(vnd::TokenizerUtility::isHasterisc(currentChar)) [[likely]] {
                tokens.emplace_back(handleHexadecimalOrOctal());
            } else if(std::isspace(currentChar)) [[likely]] {
                handleWhiteSpace();
                continue;  // Continue the loop to get the next token
            } else if(vnd::TokenizerUtility::isComment(_input, position)) {
                tokens.emplace_back(handleComment());
            } else if(vnd::TokenizerUtility::isOperator(currentChar)) [[likely]] {
                auto opTokens = handleOperators();
                tokens.insert(tokens.end(), begin(opTokens), end(opTokens));
            } else if(vnd::TokenizerUtility::isDot(currentChar)) {
                tokens.emplace_back(handleDot());
            } else if(vnd::TokenizerUtility::isBrackets(currentChar)) [[likely]] {
                tokens.emplace_back(handleBrackets());
            } else if(vnd::TokenizerUtility::isApostrophe(currentChar)) [[likely]] {
                tokens.emplace_back(handleChar());
            } else if(vnd::TokenizerUtility::isQuotation(currentChar)) [[likely]] {
                tokens.emplace_back(handleString());
            } else if(vnd::TokenizerUtility::isComma(currentChar)) {
                tokens.emplace_back(TokenType::COMMA, ","sv, CodeSourceLocation{_filename, line, column - 1});
                incPosAndColumn();
            } else if(vnd::TokenizerUtility::isColon(currentChar)) {
                tokens.emplace_back(TokenType::COLON, ":"sv, CodeSourceLocation{_filename, line, column - 1});
                incPosAndColumn();
            } else [[unlikely]] {
                handleError(std::string(1, currentChar), "Unknown Character");
            }
        }
        tokens.emplace_back(TokenType::EOFT, "", CodeSourceLocation{_filename, line, column});
        return tokens;
    }

    bool Tokenizer::positionIsInText() const noexcept { return position < _inputSize; }

    Token Tokenizer::handleAlpha() {
        const auto start = position;
        TokenType type = TokenType::IDENTIFIER;
        while(positionIsInText() && (TokenizerUtility::isalnumUnderscore(_input.at(position)))) { incPosAndColumn(); }
        const auto value = _input.substr(start, position - start);
        kewordType(value, type);
        return {type, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleUnderscoreAlpha() {
        const auto start = position;
        incPosAndColumn();
        while(positionIsInText() && (TokenizerUtility::isalnumUnderscore(_input.at(position)))) { incPosAndColumn(); }
        const auto value = _input.substr(start, position - start);
        return {TokenType::IDENTIFIER, value, {_filename, line, column - value.size()}};
    }

    void Tokenizer::kewordType(const std::string_view &value, TokenType &type) noexcept {
        using enum TokenType;
        if(value == "main"sv) { type = K_MAIN; }
        if(value == "var"sv || value == "val"sv || value == "const"sv) { type = K_VAR; }
        if(value == "if"sv) { type = K_IF; }
        if(value == "while"sv) { type = K_WHILE; }
        if(value == "else"sv) { type = K_ELSE; }
        if(value == "for"sv) { type = K_FOR; }
        if(value == "break"sv || value == "continue"sv) { type = K_BREAK; }
        if(value == "fun"sv) { type = K_FUN; }
        if(value == "return"sv) { type = K_RETURN; }
        if(value == "nullptr"sv) { type = K_NULLPTR; }
        if(value == "true"sv || value == "false"sv) { type = BOOLEAN; }
    }

    bool Tokenizer::inTextAndE() const noexcept { return positionIsInText() && std::toupper(_input[position]) == ECR; }

    Token Tokenizer::handleDigits() {
        using enum vnd::TokenType;
        TokenType tokenType = INTEGER;
        const auto start = position;
        extractDigits();
        if(positionIsInText() && _input[position] == PNT) {
            incPosAndColumn();
            extractDigits();
            if(inTextAndE()) {
                incPosAndColumn();
                extractExponent();
            }
            tokenType = DOUBLE;
        }
        if(inTextAndE()) {
            incPosAndColumn();
            extractExponent();
            tokenType = DOUBLE;
        }
        if(positionIsInText() && _input[position] == 'i') {
            incPosAndColumn();
            tokenType = DOUBLE;
        }
        if(positionIsInText() && _input[position] == 'f') {
            incPosAndColumn();
            tokenType = DOUBLE;
        }
        const auto value = _input.substr(start, position - start);
        return {tokenType, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleComment() {
        if(_input[position + 1] == '/') { return handleSingleLineComment(); }
        if(_input[position + 1] == '*') { return handleMultiLineComment(); }
        return {TokenType::UNKNOWN, ""sv, {_filename, line, column}};
    }

    Token Tokenizer::handleSingleLineComment() {
        const auto start = position;
        while(positionIsInText() && _input[position] != CNL) { incPosAndColumn(); }
        const auto value = _input.substr(start, position - start);
        return {TokenType::COMMENT, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleMultiLineComment() {
        const auto start = position;
        const auto startColumn = column;
        while(_input[position] != '*' || _input[position + 1] != '/') {
            if(position + 2 == _inputSize) {
                const auto value = _input.substr(start, position - start + 1);
                return {TokenType::UNKNOWN, value, {_filename, line, startColumn}};
            }
            handleWhiteSpace();
        }
        incPosAndColumn();
        incPosAndColumn();
        const auto value = _input.substr(start, position - start);
        return {TokenType::COMMENT, value, {_filename, line, startColumn}};
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
            if(positionIsInText() && _input[position] == 'i') { incPosAndColumn(); }
            if(positionIsInText() && _input[position] == 'f') { incPosAndColumn(); }
        }
        const auto value = _input.substr(start, position - start);
        return {type, value, {_filename, line, column - value.size()}};
    }

    void Tokenizer::extractExponent() noexcept {
        if(positionIsInText() && vnd::TokenizerUtility::isPlusOrMinus(_input[position])) { incPosAndColumn(); }
        extractDigits();
    }

    void Tokenizer::extractDigits() noexcept {
        while(positionIsInText() && isdigit(_input.at(position))) { incPosAndColumn(); }
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
        return {type, value, {_filename, line, column - value.size()}};
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
                return {TokenType::UNKNOWN, value, {_filename, line, column - value.size()}};
            }
            incPosAndColumn();
        }
        value = _input.substr(start, position - start);
        const auto colum = column - value.size();
        incPosAndColumn();
        return {TokenType::CHAR, value, {_filename, line, colum}};
    }

    Token Tokenizer::handleString() {
        const auto startColumn = column;
        incPosAndColumn();
        const auto start = position;
        std::string_view value{};
        while(!vnd::TokenizerUtility::isQuotation(_input.at(position))) {
            if(position + 1 == _inputSize) {
                incPosAndColumn();
                value = _input.substr(start, position - start);
                return {TokenType::UNKNOWN, value, {_filename, line, startColumn}};
            }
            handleWhiteSpace();
        }
        value = _input.substr(start, position - start);
        incPosAndColumn();
        return {TokenType::STRING, value, {_filename, line, startColumn}};
    }

    void Tokenizer::extractVarLenOperator() {
        while(positionIsInText() && vnd::TokenizerUtility::isOperator(_input.at(position))) { incPosAndColumn(); }
    }

    TokenType Tokenizer::singoleCharOp(const char view) noexcept {
        switch(view) {
            using enum TokenType;
        case '-':
            return MINUS_OPERATOR;
        case '=':
            return EQUAL_OPERATOR;
        case '<':
            [[fallthrough]];
        case '>':
            return BOOLEAN_OPERATOR;
        case '!':
            return NOT_OPERATOR;
        case '+':
        case '*':
        case '/':
        case '^':
        case '%':
        case '|':
            [[fallthrough]];
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

    std::vector<Token> Tokenizer::handleOperators() {
        std::vector<Token> tokens;
        const auto start = position;
        extractVarLenOperator();
        auto value = _input.substr(start, position - start);
        while(!value.empty()) {
            Token token;
            if(value.size() > 1) { token = {multyCharOp(value.substr(0, 2)), value.substr(0, 2), {_filename, line, column - 2}}; }
            if(token.isType(TokenType::UNKNOWN) || value.size() == 1) {
                token = Token{singoleCharOp(value[0]), value.substr(0, 1), {_filename, line, column - 1}};
            }
            tokens.emplace_back(token);
            value = value.substr(token.getValue().size(), value.size() - token.getValue().size());
        }
        return tokens;
    }

    void Tokenizer::handleError(const std::string &value, const std::string_view &errorMsg) {
        const auto &lineStart = findLineStart();
        const auto &lineEnd = findLineEnd();

        std::string contextLine = getContextLine(lineStart, lineEnd);
        std::string highlighting = getHighlighting(lineStart, value.length());
        std::string errorMessage = getErrorMessage(value, errorMsg, contextLine, highlighting);

        throw std::runtime_error(errorMessage);
    }

    std::size_t Tokenizer::findLineStart() const noexcept {
        std::size_t lineStart = position;
        while(lineStart > 0 && _input[lineStart - 1] != CNL) { --lineStart; }
        return lineStart;
    }

    std::size_t Tokenizer::findLineEnd() const noexcept {
        std::size_t lineEnd = position;
        while(lineEnd < _inputSize && _input[lineEnd] != CNL) { ++lineEnd; }
        return lineEnd;
    }

    std::string Tokenizer::getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) const {
        return std::string(_input.begin() + C_L(lineStart), _input.begin() + C_L(lineEnd)).append(NEWL);
    }

    std::string Tokenizer::getHighlighting(const std::size_t &start, const std::size_t &length) const {
        return FORMAT("{: ^{}}{:^{}}{}", "", position - start, "^", length, CNL);
    }

    std::string Tokenizer::getErrorMessage(const std::string &value, const std::string_view &errMsg, const std::string &contextLine,
                                           const std::string &highlighting) {
        std::ostringstream errorMessageStream;
        errorMessageStream << FORMAT("{} '{}' (line {}, column {}):{}", errMsg, value, line, column, NEWL);
        errorMessageStream << FORMAT("Context: {}", NEWL);
        errorMessageStream << contextLine;
        errorMessageStream << highlighting;
        return errorMessageStream.str();
    }

    Token Tokenizer::handleHexadecimalOrOctal() {
        const auto start = position;
        incPosAndColumn();
        if(std::tolower(_input[position]) != 'o') {
            while(positionIsInText() && std::isxdigit(C_UC(_input[position]))) { incPosAndColumn(); }
        } else {
            incPosAndColumn();
            while(positionIsInText() && TokenizerUtility::isOctalDigit(_input[position])) { incPosAndColumn(); }
        }

        const auto value = _input.substr(start, position - start);
        return {TokenType::INTEGER, value, {_filename, line, column - value.size() - 1}};
    }
    // NOLINTEND(*-include-cleaner)
}  // namespace vnd
