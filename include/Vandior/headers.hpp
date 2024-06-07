#pragma once
// NOLINTBEGIN

#include "disableWarn.hpp"
// clang-format off
/** \cond */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820
    6244 6285 6385 6386 26409 26415 26418
    26429 26432 26437 26438 26440 26465
    26446 26447 26450 26451 26455 26457
    26459 26460 26461 26467 26472 26473
    26474 26475 26481 26482 26485 26490
    26491 26493 26494 26495 26496 26497
    26498 26800 26814 26818 26826)
/** \endcond */
#include <any>
#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#ifdef _WIN32    // Check if the target platform is Windows
#ifdef _MSC_VER  // Check if the compiler is MSVC

#include <format>

#endif
#endif
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <ostream>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <source_location>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
// clang-format on
#include "cast/casts.hpp"
#include "glm_matld.hpp"
#include "glm_prety_string_cast.hpp"
#include "move.hpp"
// Restore warning levels.
DISABLE_WARNINGS_POP()
DISABLE_WARNINGS_PUSH(26476 26446 26482 26497 26472 26440 26447 26490 26481 26429 26493 26438 26455 26432 26496 26485 26819)
#include <nlohmann/json.hpp>
DISABLE_WARNINGS_POP()

namespace fs = std::filesystem;

static inline constexpr long double NINFINITY = std::numeric_limits<long double>::infinity();
static inline constexpr long double PI = std::numbers::pi_v<long double>;
static inline constexpr long double TWO_PI = 2 * PI;
static inline constexpr long double HALF_PI = PI / 2;
static inline constexpr const auto *CNL = "\n";
static inline constexpr const auto *CCR = "\r";
static inline constexpr auto NL = CNL[0];
static inline constexpr auto CR = CCR[0];
static inline constexpr char PNT = '.';
static inline constexpr char ECR = 'E';
static inline constexpr const auto *CRNL = "\r\n";
static inline constexpr char ctab = '\t';

#ifdef _WIN32                                     // Windows
static inline constexpr const auto *NEWL = CRNL;  // Windows
#elif defined macintosh                           // OS 9
static inline constexpr const auto *NEWL = &CCR;  // Classic Mac OS
#elif defined __unix__                            // Linux and Unix-like systems
static inline constexpr const auto *NEWL = CNL;  // Linux and Unix
#elif defined __APPLE__                           // macOS
static inline constexpr const auto *NEWL = CNL;  // macOS
#elif defined __VMS                               // OpenVMS
static inline constexpr const auto *NEWL = CRNL;  // OpenVMS
#elif defined __FreeBSD__                         // FreeBSD
static inline constexpr const auto *NEWL = CNL;  // FreeBSD
#else
static inline constexpr const auto *NEWL = CNL;  // Default case
#endif
/**
 * @def SYSPAUSE
 * @brief A macro to pause the system and wait for user input.
 * This macro outputs a message to the user and waits for them to press
 * the enter key before continuing. It is useful for pausing the execution
 * of a console application to allow the user to read any final output
 * before the program exits.
 * Usage example:
 * @code
 * SYSPAUSE();
 * @endcode
 */
#define SYSPAUSE()                                                                                                                         \
    do {                                                                                                                                   \
        LINFO("Press enter to exit...");                                                                                                   \
        std::cin.ignore();                                                                                                                 \
    } while(0);

/**
 * @brief Converts the given parameter into a string literal.
 * This macro converts the provided parameter into a string literal.
 * For example, if `x` is provided as `param`, the macro expands to `"param"`.
 * @param x The parameter to convert into a string.
 * @return The string representation of the parameter.
 */
#define STRINGIFY(x) #x

/**
 * @brief Converts the given parameter into a string literal.
 * This macro is an alias for STRINGIFY(x). It converts the provided parameter into a string literal.
 * For example, if `x` is provided as `param`, the macro expands to `"param"`.
 * @param x The parameter to convert into a string.
 * @return The string representation of the parameter.
 */
#define TOSTRING(x) STRINGIFY(x)

/**
 * @brief Creates a unique pointer to an object of the specified type.
 * This macro creates a unique pointer to an object of the specified type, optionally forwarding arguments to its constructor.
 * @param type The type of the object to create.
 * @param ... The arguments to pass to the constructor of the object.
 * @return A unique pointer to the created object.
 */
#define MAKE_UNIQUE(type, ...) std::make_unique<type>(__VA_ARGS__)

/**
 * @brief Creates a shared pointer to an object of the specified type.
 * This macro creates a shared pointer to an object of the specified type, optionally forwarding arguments to its constructor.
 * @param type The type of the object to create.
 * @param ... The arguments to pass to the constructor of the object.
 * @return A shared pointer to the created object.
 */
#define MAKE_SHARED(type, ...) std::make_shared<type>(__VA_ARGS__)

/**
 * @brief Gets the index of the active alternative in a variant.
 * This macro retrieves the index of the active alternative in a variant.
 * @param var The variant to inspect.
 * @return The index of the active alternative in the variant.
 */
#define GET_VARIANT_INDEX(var) var.index()

/**
 * @brief Gets the value of the specified type from a variant.
 * This macro retrieves the value of the specified type from a variant.
 * @param var The variant to extract the value from.
 * @param type The type of value to extract.
 * @return The value of the specified type stored in the variant.
 */
#define GET_VARIANT_TYPE(var, type) std::get<type>(var)
/** \cond */

/**
 * @brief Specialization of fmt::formatter for glm::vec.
 */
template <typename T, glm::length_t L, glm::qualifier Q> struct fmt::formatter<glm::vec<L, T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::vec.
     * @param vector The vector to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(const glm::vec<L, T, Q> &vector, FormatContext &ctx) {
        return formatter<std::string_view>::format(glmp::to_string(vector), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for glm::mat.
 */
template <typename T, glm::length_t C, glm::length_t R, glm::qualifier Q>
struct fmt::formatter<glm::mat<C, R, T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::mat.
     * @param matrix The matrix to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(const glm::mat<C, R, T, Q> &matrix, FormatContext &ctx) {
        return formatter<std::string_view>::format(glmp::to_string(matrix), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for glm::qua.
 */
template <typename T, glm::qualifier Q> struct fmt::formatter<glm::qua<T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::qua.
     * @param quaternion The quaternion to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(const glm::qua<T, Q> &quaternion, FormatContext &ctx) {
        return formatter<std::string_view>::format(glmp::to_string(quaternion), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for nlohmann::json.
 */
template <> struct fmt::formatter<nlohmann::basic_json<>> : formatter<std::string_view> {
    /**
     * @brief Format function for std::filesystem::path.
     * @param path The path to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(const nlohmann::basic_json<> &json, FormatContext &ctx) {
        return formatter<std::string_view>::format(json.dump(4), ctx);
    }
};
/** \endcond */

/**
 * @typedef StringPair
 * @brief A pair of strings.
 *
 * This type alias defines a pair of strings using `std::pair` from the C++ Standard Library.
 * It is useful for storing two associated strings together, such as key-value pairs or name-description pairs.
 */
using StringPair = std::pair<std::string, std::string>;

/**
 * @typedef StringPairVec
 * @brief A vector of string pairs.
 * This type alias defines a vector of `StringPair` using `std::vector` from the C++ Standard Library.
 * It is useful for storing a list of associated string pairs, such as a collection of key-value pairs.
 */
using StringPairVec = std::vector<StringPair>;

/**
 * @typedef StringVec
 * @brief A vector of strings.
 * This type alias defines a vector of strings using `std::vector` from the C++ Standard Library.
 * It is useful for storing a list of strings, such as a collection of words or lines from a text.
 */
using StringVec = std::vector<std::string>;

/**
 * @typedef StrViewVec
 * @brief A vector of string views.
 *
 * This type alias defines a vector of string views using `std::vector` from the C++ Standard Library.
 * `std::string_view` provides a non-owning view of a string, which can be useful for performance-sensitive
 * operations where creating a copy of a string is not necessary.
 * This is useful for storing a list of string views that reference parts of strings without owning them.
 */
using StrViewVec = std::vector<std::string_view>;

/**
 * @typedef OptionalSizeT
 * @brief An optional size_t value.
 *
 * This type alias defines an optional size_t using `std::optional` from the C++ Standard Library.
 * `std::optional` can be used to represent a value that may or may not be present. In this case, it represents
 * an optional `size_t` value, which can be useful for functions that may or may not return a valid size or index.
 */
using OptionalSizeT = std::optional<size_t>;

/**
 * @concept StringOrStringView
 * @brief Concept that checks if a type is either std::string or std::string_view.
 *
 * This concept is used to constrain template parameters to ensure that only `std::string` or `std::string_view` types are accepted.
 * It is useful for generic programming where a function or class template should only work with string types.
 *
 * @tparam T The type to check.
 *
 * @note This concept uses `std::same_as` from the C++ Standard Library, which checks if two types are the same.
 *
 * Example usage:
 * @code
 * template <StringOrStringView T>
 * void printString(T str) {
 *     std::cout << str << std::endl;
 * }
 * @endcode
 */
template <typename T>
concept StringOrStringView = std::same_as<std::remove_cvref_t<T>, std::string> || std::same_as<std::remove_cvref_t<T>, std::string_view> ||
                             (requires(const T &t) {
                                 { std::ranges::begin(t) } -> std::convertible_to<typename T::const_iterator>;
                                 { std::ranges::end(t) } -> std::convertible_to<typename T::const_iterator>;
                                 { t.data() } -> std::convertible_to<const char *>;
                                 { t.size() } -> std::integral;
                                 { t.length() } -> std::integral;
                             });
// NOLINTEND
