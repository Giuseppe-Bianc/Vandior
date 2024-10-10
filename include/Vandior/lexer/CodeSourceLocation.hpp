//
// Created by gbian on 02/02/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../headers.hpp"

using namespace std::literals::string_view_literals;

/**
 * \cond
 */
DISABLE_WARNINGS_PUSH(26447)
/** \endcond */

namespace vnd {
    /**
     * @brief Represents the source location in code (file, line, column).
     * This class provides a representation of the source location in code, including
     * the file name, line number, and column number.
     */
    class CodeSourceLocation {
    public:
        /**
         * @brief Default constructor.
         * @details Constructs a CodeSourceLocation with default values.
         */
        constexpr CodeSourceLocation() noexcept = default;

        /**
         * @brief Constructor with all parameters.
         * @param fileName The name of the source file.
         * @param line The line number in the source file.
         * @param column The column number in the source file.
         */
        constexpr CodeSourceLocation(std::string_view fileName, std::size_t line, std::size_t column) noexcept
          : _fileName(fileName), _line(line), _column(column) {}

        /**
         * @brief Perfect forwarding constructor.
         * @param fileName The name of the source file.
         * @param line The line number in the source file.
         * @param column The column number in the source file.
         */
        template <typename StringOrStringView>
        CodeSourceLocation(StringOrStringView &&fileName, std::size_t line, std::size_t column) noexcept
          : _fileName(std::forward<std::decay_t<StringOrStringView>>(fileName)), _line(line), _column(column) {}

        /**
         * @brief Copy constructor.
         */
        constexpr CodeSourceLocation(const CodeSourceLocation &) noexcept = default;

        /**
         * @brief Move constructor.
         */
        constexpr CodeSourceLocation(CodeSourceLocation &&) noexcept = default;

        /**
         * @brief Copy assignment operator.
         */
        constexpr CodeSourceLocation &operator=(const CodeSourceLocation &) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        constexpr CodeSourceLocation &operator=(CodeSourceLocation &&) noexcept = default;

        /**
         * @brief Destructor.
         */
        ~CodeSourceLocation() noexcept = default;

        /**
         * @brief Getter method for file name.
         * @return The file name as a string view.
         */
        [[nodiscard]] constexpr std::string_view getFileName() const noexcept { return _fileName; }

        /**
         * @brief Getter method for line number.
         * @return The line number.
         */
        [[nodiscard]] constexpr std::size_t getLine() const noexcept { return _line; }

        /**
         * @brief Getter method for column number.
         * @return The column number.
         */
        [[nodiscard]] constexpr std::size_t getColumn() const noexcept { return _column; }

        /**
         * @brief Setter method for file name.
         * @param fileName The name of the source file.
         */
        constexpr void setFileName(std::string_view fileName) noexcept { _fileName = fileName; }

        /**
         * @brief Setter method for line number.
         * @param line The line number in the source file.
         */
        constexpr void setLine(std::size_t line) noexcept { _line = line; }

        /**
         * @brief Setter method for column number.
         * @param column The column number in the source file.
         */
        constexpr void setColumn(std::size_t column) noexcept { _column = column; }

        /**
         * @brief Creates a CodeSourceLocation object representing an unknown location.
         * @return A CodeSourceLocation object with "unknown" file name and (0, 0) coordinates.
         */
        static constexpr CodeSourceLocation unknown() noexcept { return CodeSourceLocation("unknown"sv, 0, 0); }

        /**
         * @brief Equality operator.
         */
        constexpr bool operator==(const CodeSourceLocation &other) const noexcept = default;

        /**
         * @brief Inequality operator.
         * @param other The CodeSourceLocation to compare with.
         * @return True if the CodeSourceLocations are not equal, false otherwise.
         */
        constexpr bool operator!=(const CodeSourceLocation &other) const noexcept = default;

        /**
         * @brief Spaceship operator for comparison.
         * @param other The CodeSourceLocation to compare with.
         * @return The result of the three-way comparison.
         */
        constexpr auto operator<=>(const CodeSourceLocation &other) const noexcept = default;
        [[nodiscard]] std::string toString() const { return FORMAT("(file:{}, line:{}, column:{})", _fileName, _line, _column); }

        /**
         * @brief A compact version of toString, for compatibility.
         * @return A compact string representation of the CodeSourceLocation.
         */
        [[nodiscard]] std::string compat_to_string() const { return fmt::format("(f:{}, l:{}, c:{})", _fileName, _line, _column); }

    private:
        std::string_view _fileName{"unknown"};  ///< The name of the source file.
        std::size_t _line{0};                   ///< The line number in the source file.
        std::size_t _column{0};                 ///< The column number in the source file.
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

/**
 * This function is a formatter for CodeSourceLocation using fmt.
 * \cond
 */
// NOLINTNEXTLINE
template <> struct fmt::formatter<vnd::CodeSourceLocation> : fmt::formatter<std::string_view> {
    auto format(const vnd::CodeSourceLocation &val, format_context &ctx) const -> format_context::iterator {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
/** \endcond */
// NOLINTEND(*-include-cleaner)
