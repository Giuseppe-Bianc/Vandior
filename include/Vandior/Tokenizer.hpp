#pragma once

#include "Vandior/Log.hpp"
#include "Vandior/Timer.hpp"
#include "Vandior/Token.hpp"
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(const std::string_view &input) noexcept : _input(input), _inputSize(input.size()) {}
    [[nodiscard]] std::vector<Token> tokenize();

private:
    std::string_view _input;
    std::size_t _inputSize;
    std::size_t position = 0;
    std::size_t line = 1;
    std::size_t column = 1;

    [[nodiscard]] bool positionIsInText() const noexcept;
    [[nodiscard]] Token handleAlpha();
    [[nodiscard]] Token handleDigits();
    void handleWhiteSpace();
    void handleError(const std::string &value, const std::string_view &errorMsg);
    [[nodiscard]] std::size_t findLineStart() const;
    [[nodiscard]] std::size_t findLineEnd() const;
    [[nodiscard]] std::string getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) const;
    [[nodiscard]] std::string getHighlighting(const std::size_t &start, const std::size_t &length) const;
    std::string getErrorMessage(const std::string &value, const std::string_view &errMsg, const std::string & contextLine,
                                const std::string &highlighting);
    void extractDigits();
    void incPosAndColumn();
    void extractExponent();
    [[nodiscard]] bool isOperator(const char &aChar);
    [[nodiscard]] bool isPlusOrMinus(const char &cara) const;
    [[nodiscard]] Token handleOperators();
};
