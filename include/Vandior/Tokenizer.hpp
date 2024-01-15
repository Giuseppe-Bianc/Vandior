#pragma once

#include "Vandior/Log.hpp"    // NOLINT(*-include-cleaner)
#include "Vandior/Timer.hpp"  // NOLINT(*-include-cleaner)
#include "Vandior/Token.hpp"
#include "Vandior/TokenizerUtility.hpp"
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(const std::string_view &input) noexcept
      : _input(input), _inputSize(input.size()) {}  // NOLINT(*-include-cleaner)
    [[nodiscard]] std::vector<Token> tokenize();    // NOLINT(*-include-cleaner)

private:
    std::string_view _input;
    std::size_t _inputSize;
    std::size_t position = 0;
    std::size_t line = 1;
    std::size_t column = 1;

    [[nodiscard]] bool positionIsInText() const noexcept;
    [[nodiscard]] Token handleAlpha();
    [[nodiscard]] Token handleDigits();
    [[nodiscard]] Token handleComment();
    [[nodiscard]] Token handleSingleLineComment();
    [[nodiscard]] Token handleMultiLineComment();
    [[nodiscard]] Token handleDot();
    static void kewordType(const std::string_view &value, TokenType &type) noexcept;
    void handleWhiteSpace() noexcept;
    [[noreturn]] void handleError(const std::string &value, const std::string_view &errorMsg);
    [[nodiscard]] std::size_t findLineStart() const noexcept;
    [[nodiscard]] std::size_t findLineEnd() const noexcept;
    [[nodiscard]] std::string getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) const;
    [[nodiscard]] std::string getHighlighting(const std::size_t &start, const std::size_t &length) const;
    std::string getErrorMessage(const std::string &value, const std::string_view &errMsg, const std::string &contextLine,
                                const std::string &highlighting);
    void extractDigits() noexcept;
    void incPosAndColumn() noexcept;
    void extractExponent() noexcept;
    void handleOperators(std::vector<Token> &tokens);
    void extractVarLenOperator();
    [[nodiscard]] static TokenType singoleCharOp(const char &view) noexcept;
    [[nodiscard]] static TokenType multyCharOp(const std::string_view &view) noexcept;
    [[nodiscard]] Token handleBrackets();
    [[nodiscard]] TokenType getType(const std::string_view &value) const noexcept;
    [[nodiscard]] Token handleChar();
    [[nodiscard]] bool isalnumUnderscore(const char &cha) const noexcept;
    [[nodiscard]] bool inTextAndE() const noexcept;
    Token handleString();
};
