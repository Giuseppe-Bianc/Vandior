// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"

namespace vnd {

    /**
     * @brief Utility class for tokenizing operations.
     */
    class TokenizerUtility {
    public:
        /**
         * @brief Check if the character is an operator.
         * @param aChar The character to check.
         * @return True if the character is an operator, false otherwise.
         */
        [[nodiscard]] static bool isOperator(const char aChar);

        /**
         * @brief Check if the character is either plus or minus.
         * @param cara The character to check.
         * @return True if the character is plus or minus, false otherwise.
         */
        [[nodiscard]] static bool isPlusOrMinus(const char cara) noexcept;

        /**
         * @brief Check if the character is a dot.
         * @param cha The character to check.
         * @return True if the character is a dot, false otherwise.
         */
        [[nodiscard]] static bool isDot(const char cha) noexcept;

        /**
         * @brief Check if the character is a bracket.
         * @param cha The character to check.
         * @return True if the character is a bracket, false otherwise.
         */
        [[nodiscard]] static bool isBrackets(const char cha);

        /**
         * @brief Check if the character is an apostrophe.
         * @param cha The character to check.
         * @return True if the character is an apostrophe, false otherwise.
         */
        [[nodiscard]] static bool isApostrophe(const char cha) noexcept;

        /**
         * @brief Check if the character is a quotation mark.
         * @param cha The character to check.
         * @return True if the character is a quotation mark, false otherwise.
         */
        [[nodiscard]] static bool isQuotation(const char cha) noexcept;

        /**
         * @brief Check if the character is a comma.
         * @param cha The character to check.
         * @return True if the character is a comma, false otherwise.
         */
        [[nodiscard]] static bool isComma(const char cha) noexcept;

        /**
         * @brief Check if the character is a colon.
         * @param cha The character to check.
         * @return True if the character is a colon, false otherwise.
         */
        [[nodiscard]] static bool isColon(const char cha) noexcept;

        /**
         * @brief Check if the span contains a comment starting at the given position.
         * @param inputSpan The input string span.
         * @param position The starting position for checking the comment.
         * @return True if a comment is found, false otherwise.
         */
        [[nodiscard]] static bool isComment(const std::string_view &inputSpan, const size_t position) noexcept;

        /**
         * @brief Check if the character is alphanumeric or underscore.
         * @param cha The input character.
         * @return True if the character is alphanumeric or underscore, false otherwise.
         */
        [[nodiscard]] static bool isalnumUnderscore(const char cha) noexcept;

        /**
         * @brief Check if the character is a octal digit.
         * @param cha The input character.
         * @return True if the character is a octal digit, false otherwise.
         */
        [[nodiscard]] static bool isOctalDigit(const char cha) noexcept;

        /**
         * @brief Check if the character is a Hasterisc.
         * @param cha The input character.
         * @return True if the character is a Hasterisc, false otherwise.
         */
        [[nodiscard]] static bool isHasterisc(const char cha) noexcept;

        /**
         * @brief Check if the character is a Underscore.
         * @param cha The input character.
         * @return True if the character is a  Underscore, false otherwise.
         */
        [[nodiscard]] static bool isUnderscore(const char cha) noexcept;
    };

}  // namespace vnd

// NOLINTEND(*-include-cleaner)