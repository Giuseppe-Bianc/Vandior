/**
 * @file format.hpp
 * @brief A set of macros for convenient use of the fmt library.
 * This file provides macros for easy integration with the fmt library, a modern C++ formatting library.
 * It includes macros for formatting strings, pointers, and joining containers with delimiters.
 * @note This file requires the fmt library to be included separately.
 */
#pragma once

#include <fmt/core.h>    // NOLINT(*-include-cleaner)
#include <fmt/format.h>  // NOLINT(*-include-cleaner)
// NOLINTBEGIN
/**
 * @def FORMAT(...)
 * @brief Macro for formatting strings using the fmt library.
 * This macro wraps the fmt::format function for convenient string formatting.
 * @param ... The format string and arguments.
 * @return The formatted string.
 */
#define FORMAT(...) fmt::format(__VA_ARGS__)

/**
 * @def FMT_PTR(ptr)
 * @brief Macro for formatting pointers using the fmt library.
 * This macro wraps the fmt::ptr function for formatting pointers.
 * @param ptr The pointer to be formatted.
 * @return The formatted pointer string.
 */
#define FMT_PTR(ptr) fmt::ptr(ptr)

/**
 * @def FMT_JOIN(container, delimiter)
 * @brief Macro for joining container elements with a delimiter using the fmt library.
 * This macro wraps the fmt::join function for joining container elements with a specified delimiter.
 * @param container The container to be joined.
 * @param delimiter The delimiter to be used between elements.
 * @return The joined string.
 */
#define FMT_JOIN(container, delimiter) fmt::join(container, delimiter)
// NOLINTEND