#pragma once

#include "Vandior/lexer/Token.hpp"
#include "Vandior/lexer/TokenizerUtility.hpp"
#include <string>

namespace vnd {
    /**
     * @brief Tokenizer class for parsing input strings.
     */
    class Tokenizer {
    public:
        /**
         * @brief Constructor for Tokenizer.
         * @param input The input string to tokenize.
         * @param fileName The name of the file being tokenized (default: "unknown.vn").
         */
        explicit Tokenizer(const std::string_view &input, const std::string_view &fileName = "unknown.vn") noexcept
          : _input(input), _filename(fileName), _inputSize(input.size()) {}  // NOLINT(*-include-cleaner)

        /**
         * @brief Tokenize the input string.
         * @return A vector of Token objects.
         */
        [[nodiscard]] TokenVec tokenize();  // NOLINT(*-include-cleaner)

    private:
        std::string_view _input;     ///< The input string to tokenize.
        std::string_view _filename;  ///< The name of the file being tokenized.
        std::size_t _inputSize;      ///< The size of the input string
        std::size_t position = 0;    ///< Current position in the input string.
        std::size_t line = 1;        ///< Current line number.
        std::size_t column = 1;      ///< Current column number.

        /**
         * @brief Check if the current position is within the text.
         * @return True if the position is within the text, false otherwise.
         */
        [[nodiscard]] bool positionIsInText() const noexcept;

        /**
         * @brief Handles alphabetic characters.
         * @return Token representing alphabetic characters.
         */
        [[nodiscard]] Token handleAlpha();

        /**
         * @brief Handles underscore followed by alphabetic characters.
         * @return Token representing underscore followed by alphabetic characters.
         */
        [[nodiscard]] Token handleUnderscoreAlpha();

        /**
         * @brief Handles digits.
         * @return Token representing digits.
         */
        [[nodiscard]] Token handleDigits();

        /**
         * @brief Handles comments.
         * @return Token representing comments.
         */
        [[nodiscard]] Token handleComment();

        /**
         * @brief Handles single line comments.
         * @return Token representing single line comments.
         */
        [[nodiscard]] Token handleSingleLineComment();

        /**
         * @brief Handles multi-line comments.
         * @return Token representing multi-line comments.
         */
        [[nodiscard]] Token handleMultiLineComment();

        /**
         * @brief Handles dot character.
         * @return Token representing dot character.
         */
        [[nodiscard]] Token handleDot();

        static void getType(const std::string_view &value, TokenType &type) noexcept;
        /**
         * @brief Sets TokenType based on the keyword.
         * @param value The keyword.
         * @param type TokenType to set.
         */
        static void kewordType(const std::string_view &value, TokenType &type) noexcept;

        /**
         * @brief Handles white space characters.
         */
        void handleWhiteSpace() noexcept;
        /**
         * @brief Finds the start of the current line.
         * @return Position of the start of the current line.
         */
        [[nodiscard]] std::size_t findLineStart() const noexcept;

        /**
         * @brief Finds the end of the current line.
         * @return Position of the end of the current line.
         */
        [[nodiscard]] std::size_t findLineEnd() const noexcept;

        /**
         * @brief Handles errors.
         * @param value The error value.
         * @param errorMsg Error message.
         */
        template <StringOrStringView T> [[noreturn]] void handleError(const T &value, const std::string_view &errorMsg);

        /**
         * @brief Retrieves the context.
         * @param lineStart Position of the start of the context.
         * @param lineEnd Position of the end of the context.
         * @return Contex.
         */
        [[nodiscard]] std::string extract_context(const std::size_t &lineStart, const std::size_t &lineEnd) const;

        /**
         * @brief Retrieves the context line.
         * @param lineStart Position of the start of the line.
         * @param lineEnd Position of the end of the line.
         * @return Context line.
         */
        [[nodiscard]] std::string getContextLine(const std::size_t &lineStart, const std::size_t &lineEnd) const;

        /**
         * @brief Retrieves the highlighting for error messages.
         * @param lineStart Starting position of the highlight.
         * @param lineEnd End position of the highlight.
         * @param value the value to highlight.
         * @return Highlighted string.
         */
        [[nodiscard]] std::string getHighlighting(const std::size_t &lineStart, const std::size_t &lineEnd,
                                                  const std::string_view value) const;

        /**
         * @brief Generates an error message.
         * @param value The error value.
         * @param errMsg Error message.
         * @param contextLine Context line.
         * @param highlighting Highlighted string.
         * @return Error message.
         */
        template <StringOrStringView T>
        [[nodiscard]] std::string getErrorMessage(const T &value, const std::string_view &errMsg, const std::string &contextLine,
                                                  const std::string &highlighting);
        /**
         * @brief Extracts digits from the input.
         */
        void extractDigits() noexcept;

        /**
         * @brief Increases position and column.
         */
        void incPosAndColumn() noexcept;

        /**
         * @brief Extracts exponent from the input.
         */
        void extractExponent() noexcept;

        /**
         * @brief Handles operators.
         * @return Tokens representing operators.
         */
        [[nodiscard]] TokenVec handleOperators();

        /**
         * @brief Extracts variable length operator.
         */
        void extractVarLenOperator();

        /**
         * @brief Determines the token type of single character operators.
         * @param view The character.
         * @return TokenType of the character.
         */
        [[nodiscard]] static TokenType singoleCharOp(const char view) noexcept;

        /**
         * @brief Determines the token type of multi-character operators.
         * @param view The operator.
         * @return TokenType of the operator.
         */
        [[nodiscard]] static TokenType multyCharOp(const std::string_view &view) noexcept;

        /**
         * @brief Handles brackets.
         * @return Token representing brackets.
         */
        [[nodiscard]] Token handleBrackets();

        /**
         * @brief Handles characters.
         * @return Token representing characters.
         */
        [[nodiscard]] Token handleChar();

        /**
         * @brief Checks if the current position is within the text and the next character is 'e'.
         * @return True if within text and next character is 'e', false otherwise.
         */
        [[nodiscard]] bool inTextAndE() const noexcept;
        /**
         * @brief Checks if the current position is within the text and the next character.
         * @param chr the caracter to check.
         * @return True if within text and next character is chr, false otherwise.
         */
        [[nodiscard]] bool inTextAnd(char chr) const noexcept;

        /**
         * @brief Handles strings.
         * @return Token representing strings.
         */
        Token handleString();

        /**
         * @brief Handles hexadecimal or octal values.
         * @return Token representing hexadecimal or octal values.
         */
        Token handleHexadecimalOrOctal();
    };

}  // namespace vnd
