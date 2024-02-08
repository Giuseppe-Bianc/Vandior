//
// Created by gbian on 02/02/2024.
//

#pragma once
#include "headers.hpp"  // NOLINT(*-include-cleaner)

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

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
        CodeSourceLocation() noexcept = default;

        /**
         * @brief Constructor with all parameters.
         * @param fileName The name of the source file.
         * @param line The line number in the source file.
         * @param column The column number in the source file.
         */
        CodeSourceLocation(const std::string_view &fileName, std::size_t line, std::size_t column) noexcept
          : _fileName(fileName), _line(line), _column(column) {}

        /**
         * @brief Perfect forwarding constructor.
         * @param fileName The name of the source file.
         * @param line The line number in the source file.
         * @param column The column number in the source file.
         */
        template <typename FileNameT, typename = std::enable_if_t<std::is_convertible_v<FileNameT, std::string_view>>>
        CodeSourceLocation(FileNameT &&fileName, std::size_t line, std::size_t column) noexcept
          : _fileName(std::forward<FileNameT>(fileName)), _line(line), _column(column) {}

        /**
         * @brief Copy constructor.
         * @param other The CodeSourceLocation object to copy.
         */
        CodeSourceLocation(const CodeSourceLocation &other) = default;

        /**
         * @brief Move constructor.
         * @param other The CodeSourceLocation object to move.
         */
        CodeSourceLocation(CodeSourceLocation &&other) noexcept = default;

        /**
         * @brief Copy assignment operator.
         * @param other The CodeSourceLocation object to copy.
         * @return A reference to the assigned CodeSourceLocation.
         */
        CodeSourceLocation &operator=(const CodeSourceLocation &other) = default;

        /**
         * @brief Move assignment operator.
         * @param other The CodeSourceLocation object to move.
         * @return A reference to the assigned CodeSourceLocation.
         */
        CodeSourceLocation &operator=(CodeSourceLocation &&other) noexcept = default;

        /**
         * @brief Destructor.
         */
        ~CodeSourceLocation() = default;

        /**
         * @brief Getter method for file name.
         * @return The file name as a string view.
         */
        [[nodiscard]] std::string_view getFileName() const noexcept { return _fileName; }

        /**
         * @brief Getter method for line number.
         * @return The line number.
         */
        [[nodiscard]] std::size_t getLine() const noexcept { return _line; }

        /**
         * @brief Getter method for column number.
         * @return The column number.
         */
        [[nodiscard]] std::size_t getColumn() const noexcept { return _column; }

        /**
         * @brief Setter method for file name.
         */
        void setFileName(const std::string_view &fileName) noexcept { _fileName = fileName; }

        /**
         * @brief Setter method for line number.
         */
        void setLine(size_t line) noexcept { _line = line; }

        /**
         * @brief Setter method for column number.
         */
        void setColumn(size_t column) noexcept { _column = column; }

        /**
         * @brief Creates a CodeSourceLocation object representing an unknown location.
         * @return A CodeSourceLocation object with "unknown" file name and (0, 0) coordinates.
         */
        static CodeSourceLocation unknown() noexcept { return CodeSourceLocation("unknown", 0, 0); }

        /**
         * @brief Equality operator.
         * @param other The CodeSourceLocation to compare with.
         * @return True if the CodeSourceLocations are equal, false otherwise.
         */
        bool operator==(const CodeSourceLocation &other) const noexcept = default;

        /**
         * @brief Inequality operator.
         * @param other The CodeSourceLocation to compare with.
         * @return True if the CodeSourceLocations are not equal, false otherwise.
         */
        bool operator!=(const CodeSourceLocation &other) const noexcept = default;

        /**
         * @brief Spaceship operator for comparison.
         * @param other The CodeSourceLocation to compare with.
         * @return The result of the three-way comparison.
         */
        auto operator<=>(const CodeSourceLocation &other) const noexcept = default;

        std::string toString() const noexcept { return FORMAT("(file:{}, line:{}, column:{})", _fileName, _line, _column); }

    private:
        std::string_view _fileName;  ///< The name of the source file.
        std::size_t _line{};         ///< The line number in the source file.
        std::size_t _column{};       ///< The column number in the source file.
    };

}  // namespace vnd

DISABLE_WARNINGS_POP()

template <> struct fmt::formatter<vnd::CodeSourceLocation> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const vnd::CodeSourceLocation &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
