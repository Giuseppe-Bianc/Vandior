#include "Vandior/lexer/Tokenizer.hpp"

using namespace std::literals::string_view_literals;
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-avoid-magic-numbers, *-magic-numbers)
DISABLE_WARNINGS_PUSH(26446)
namespace vnd {

    std::vector<TokenVec> Tokenizer::tokenize() {
        tokens.reserve(_inputSize);
        while(positionIsInText()) {
            const char &currentChar = _input.at(position);
            if(C_BOOL(std::isalpha(currentChar))) [[likely]] {
                tokens.emplace_back(handleAlpha());
            } else if(C_BOOL(std::isdigit(currentChar))) [[likely]] {
                tokens.emplace_back(handleDigits());
            } else if(TokenizerUtility::isUnderscore(currentChar)) [[likely]] {
                tokens.emplace_back(handleUnderscoreAlpha());
            } else if(TokenizerUtility::isHasterisc(currentChar)) [[likely]] {
                tokens.emplace_back(handleHexadecimalOrOctal());
            } else if(C_BOOL(std::isspace(currentChar))) [[likely]] {
                handleWhiteSpace();
            } else if(TokenizerUtility::isComment(_input, position)) {
                tokens.emplace_back(handleComment());
            } else if(TokenizerUtility::isOperator(currentChar)) [[likely]] {
                auto opTokens = handleOperators();
                tokens.insert(tokens.end(), std::begin(opTokens), std::end(opTokens));
            } else if(TokenizerUtility::isDot(currentChar)) {
                tokens.emplace_back(handleDot());
            } else if(TokenizerUtility::isBrackets(currentChar)) [[likely]] {
                tokens.emplace_back(handleBrackets());
            } else if(TokenizerUtility::isApostrophe(currentChar)) [[likely]] {
                tokens.emplace_back(handleChar());
            } else if(TokenizerUtility::isQuotation(currentChar)) [[likely]] {
                tokens.emplace_back(handleString());
            } else if(TokenizerUtility::isCommaColon(currentChar)) {
                incPosAndColumn();
                tokens.emplace_back(TokenizerUtility::CommaOrColonType(currentChar), TokenizerUtility::CommaOrColonValue(currentChar),
                                    CodeSourceLocation{_filename, line, column - 1});
            } else [[unlikely]] {
                error(std::string(1, currentChar), "Unknown Character");
            }
        }
        tokens.emplace_back(TokenType::EOFT, CodeSourceLocation{_filename, line, column});
        if(!brackets.empty()) { throw std::runtime_error("Mismatch bracket"); }
        result.emplace_back(tokens);
        return result;
    }

    bool Tokenizer::positionIsInText() const noexcept { return position < _inputSize; }

    unsigned char Tokenizer::getUnsignedCharAt(const size_t pos) const {
        if(pos >= _inputSize) { throw std::out_of_range(FORMAT("Position {} out of range 0-{}", pos, _inputSize)); }
        return C_UC(_input[pos]);
    }

    Token Tokenizer::handleAlpha() {
        const auto start = position;
        auto type = TokenType::IDENTIFIER;
        // clang-format off
        while(positionIsInText() && (TokenizerUtility::isalnumUnderscore(_input[position]))) {
            incPosAndColumn();
        }
        // clang-format on
        const auto value = _input.substr(start, position - start);
        kewordType(value, type);
        return {type, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleUnderscoreAlpha() {
        const auto start = position;
        incPosAndColumn();
        while(positionIsInText() && (TokenizerUtility::isalnumUnderscore(_input[position]))) { incPosAndColumn(); }
        const auto value = _input.substr(start, position - start);
        return {TokenType::IDENTIFIER, value, {_filename, line, column - value.size()}};
    }

    DISABLE_WARNINGS_PUSH(26446 26447)
    // NOLINTBEGIN(*-identifier-length, *-qualified-auto)
    void Tokenizer::getType(const std::string_view &value, TokenType &type) noexcept {
        const auto it = std::ranges::find_if(typeArray, [&value](const auto &pair) { return pair.first == value; });

        if(it != typeArray.end()) {
            type = it->second;  // Assign the found type
        }
    }

    void Tokenizer::kewordType(const std::string_view &value, TokenType &type) noexcept {
        // Use std::find_if to search for the keyword type
        const auto it = std::ranges::find_if(keywordArray, [&value](const auto &pair) { return pair.first == value; });

        if(it != keywordArray.end()) {
            type = it->second;  // Assign the found type
        } else [[unlikely]] {
            getType(value, type);  // Call getType for other types
        }
    }
    // NOLINTEND(*-identifier-length, *-qualified-auto)
    DISABLE_WARNINGS_POP()

    bool Tokenizer::inTextAndE() const noexcept { return positionIsInText() && std::toupper(_input[position]) == ECR; }
    bool Tokenizer::inTextAnd(char chr) const noexcept { return positionIsInText() && _input[position] == chr; }

    Token Tokenizer::handleDigits() {
        using enum TokenType;
        auto tokenType = INTEGER;
        const auto start = position;
        extractDigits();
        if(inTextAnd(PNT)) {
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
        if(inTextAnd(icr)) {
            incPosAndColumn();
            tokenType = DOUBLE;
        }
        if(inTextAnd(fcr)) {
            incPosAndColumn();
            tokenType = DOUBLE;
        }
        const auto value = _input.substr(start, position - start);
        return {tokenType, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleComment() {
        const auto nextposition = position + 1;
        if(_input[nextposition] == slashcr) { return handleSingleLineComment(); }
        if(_input[nextposition] == starcr) { return handleMultiLineComment(); }
        return {TokenType::UNKNOWN, {_filename, line, column}};
    }

    Token Tokenizer::handleSingleLineComment() {
        const auto start = position;
        while(positionIsInText() && _input[position] != NL) { incPosAndColumn(); }
        const auto value = _input.substr(start, position - start);
        return {TokenType::COMMENT, value, {_filename, line, column - value.size()}};
    }

    Token Tokenizer::handleMultiLineComment() {
        const auto start = position;
        const auto startColumn = column;
        while(_input[position] != starcr || _input[position + 1] != slashcr) {
            if(!positionIsInText()) {
                const auto value = _input.substr(start, position - start);
                return {TokenType::UNKNOWN, value, {_filename, line, startColumn}};
            }
            // Advance to the next character
            incPosAndColumn();
            handleWhiteSpaceSingle();
        }
        incPosAndColumn();  // Skip '*'
        incPosAndColumn();  // Skip '/'
        const auto value = _input.substr(start, position - start);
        return {TokenType::COMMENT, value, {_filename, line, startColumn}};
    }

    Token Tokenizer::handleDot() {
        const auto start = position;
        auto type = TokenType::DOT;
        incPosAndColumn();
        if(positionIsInText() && C_BOOL(std::isdigit(getUnsignedCharAt(position)))) {
            type = TokenType::DOUBLE;
            extractDigits();
            if(inTextAndE()) {
                incPosAndColumn();
                extractExponent();
            }
            if(inTextAnd(icr)) { incPosAndColumn(); }
            if(inTextAnd(fcr)) { incPosAndColumn(); }
        }
        const auto value = _input.substr(start, position - start);
        return {type, value, {_filename, line, column - value.size()}};
    }

    void Tokenizer::extractExponent() noexcept {
        if(positionIsInText() && TokenizerUtility::isPlusOrMinus(_input[position])) { incPosAndColumn(); }
        extractDigits();
    }

    void Tokenizer::extractDigits() noexcept {
        while(positionIsInText() && C_BOOL(std::isdigit(_input[position]))) { incPosAndColumn(); }
    }

    void Tokenizer::incPosAndColumn() noexcept {
        position++;
        column++;
    }

    void Tokenizer::incrementLine() noexcept {
        if(bracketNum == 0) {
            result.push_back(tokens);
            tokens = {};
        }
        ++line;
        column = 0;
    }

    void Tokenizer::handleWhiteSpaceSingle() noexcept {
        // Check if we're at the start of a line with '\n' or '\r\n'
        char const charnsl = _input[position];
        if(charnsl == NL) [[unlikely]] {
            incrementLine();
        } else if(charnsl == '\r') [[unlikely]] {
            // Handle '\r' (carriage return), which might be followed by '\n'
            if(const std::size_t nextPosition = position + 1; nextPosition < _inputSize && _input[nextPosition] == NL) {
                incrementLine();
                position++;  // Skip the '\n' that follows '\r'
            } else [[unlikely]] {
                incrementLine();
            }
        }

        // Increment position and column to move to the next character
        incPosAndColumn();
    }

    void Tokenizer::handleWhiteSpace() noexcept {
        while(positionIsInText() && C_BOOL(std::isspace(_input[position]))) { handleWhiteSpaceSingle(); }
    }

    Token Tokenizer::handleBrackets() {
        using enum TokenType;
        const auto start = position;
        incPosAndColumn();
        const auto value = _input.substr(start, position - start);
        const auto type = getBracketsType(value);
        switch(type) {
        case OPEN_PARENTESIS:
        case OPEN_SQ_PARENTESIS:
            if(bracketNum == 0) { bracketNum = brackets.size() + 1; }
            [[fallthrough]];
        case OPEN_CUR_PARENTESIS:
            brackets.push_back(type);
            break;
        case CLOSE_PARENTESIS:
            removeBrackets(OPEN_PARENTESIS);
            break;
        case CLOSE_SQ_PARENTESIS:
            removeBrackets(OPEN_SQ_PARENTESIS);
            break;
        case CLOSE_CUR_PARENTESIS:
            removeBrackets(OPEN_CUR_PARENTESIS);
            break;
        default:
            break;
        }
        return {type, value, {_filename, line, column - value.size()}};
    }

    void Tokenizer::removeBrackets(const TokenType &type) {
        if(type != TokenType::OPEN_CUR_PARENTESIS && brackets.size() == bracketNum) { bracketNum = 0; }
        if(brackets.empty() || brackets.back() != type) { error("", "Mismatch bracket"); }
        brackets.pop_back();
    }

    Token Tokenizer::handleChar() {
        incPosAndColumn();
        const auto start = position;
        std::string_view value{};
        while(!TokenizerUtility::isApostrophe(_input[position])) {
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
        while(!TokenizerUtility::isQuotation(_input[position])) {
            if(position + 1 == _inputSize) {
                incPosAndColumn();
                value = _input.substr(start, position - start);
                return {TokenType::UNKNOWN, value, {_filename, line, startColumn}};
            }
            handleWhiteSpaceSingle();
        }
        value = _input.substr(start, position - start);
        incPosAndColumn();
        return {TokenType::STRING, value, {_filename, line, startColumn}};
    }

    void Tokenizer::extractVarLenOperator() {
        while(positionIsInText() && TokenizerUtility::isOperator(_input[position])) { incPosAndColumn(); }
    }

    TokenType Tokenizer::multyCharOp(const std::string_view &view) noexcept {
        // NOLINTNEXTLINE(*-identifier-length, *-qualified-auto)
        if(const auto it = std::ranges::find_if(multiCharOperators, [&](const auto &pair) { return pair.first == view; });
           it != multiCharOperators.end()) {
            return it->second;
        }
        return TokenType::UNKNOWN;
    }

    std::vector<Token> Tokenizer::handleOperators() {
        std::vector<Token> code;
        const auto start = position;
        extractVarLenOperator();
        auto value = _input.substr(start, position - start);
        while(!value.empty()) {
            Token token;
            if(value.size() > 1) {
                const auto twoCharOp = value.substr(0, 2);
                token = {multyCharOp(twoCharOp), twoCharOp, {_filename, line, column - twoCharOp.size()}};
            }
            if(token.isType(TokenType::UNKNOWN) || value.size() == 1) {
                const auto oneCharOp = value.substr(0, 1);
                token = Token{singoleCharOp(oneCharOp[0]), oneCharOp, {_filename, line, column - oneCharOp.size()}};
            }

            code.emplace_back(token);
            value.remove_prefix(token.getValueSize());
        }

        return code;
    }

    template <StringOrStringView T> void Tokenizer::error(const T &value, const std::string_view &errorMsg) {
        ErrorHandler errorHandler(_input, _filename, line, column);
        errorHandler.setCurrentPosition(position);
        errorHandler.handleError(value, errorMsg);
    }
    Token Tokenizer::handleHexadecimalOrOctal() {
        const auto start = position;
        incPosAndColumn();
        if(positionIsInText() && std::tolower(getUnsignedCharAt(position)) == 'o') {
            // Gestione dei numeri ottali
            incPosAndColumn();
            while(positionIsInText() && TokenizerUtility::isOctalDigit(_input[position])) { incPosAndColumn(); }
        } else if(positionIsInText() && C_BOOL(std::isxdigit(getUnsignedCharAt(position)))) {
            // Gestione dei numeri esadecimali
            while(positionIsInText() && C_BOOL(std::isxdigit(getUnsignedCharAt(position)))) { incPosAndColumn(); }
        } else [[unlikely]] {
            const auto error_value = _input.substr(start, position);
            error(error_value, "malformed exadecimal number or octal number");
        }

        const auto value = _input.substr(start, position - start);
        return {TokenType::INTEGER, value, {_filename, line, column - value.size() - 1}};
    }
}  // namespace vnd
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-avoid-magic-numbers, *-magic-numbers)
