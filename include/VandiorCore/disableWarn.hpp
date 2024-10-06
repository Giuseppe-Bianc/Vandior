/**
 * @file
 * @brief Header file containing macros for disabling compiler warnings.
 */
#pragma once
// NOLINTBEGIN(*-macro-usage)

#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
/**
 * @def DISABLE_WARNINGS_PUSH(...)
 * @brief Pushes a warning suppression for Microsoft Visual C++ Compiler.
 *
 * @param[in] ... Variable number of warning IDs to be disabled.
 */
#define DISABLE_WARNINGS_PUSH(...) __pragma(warning(push)) __pragma(warning(disable : __VA_ARGS__))
/**
 * @def DISABLE_WARNINGS_POP()
 * @brief Pops the previously pushed warning suppression for Microsoft Visual C++ Compiler.
 */
#define DISABLE_WARNINGS_POP() __pragma(warning(pop))
#else
/**
 * @def DISABLE_WARNINGS_PUSH(...)
 * @brief Placeholder for non-Microsoft compilers.
 *
 * @param[in] ... Variable number of warning IDs (ignored for non-Microsoft compilers).
 */
#define DISABLE_WARNINGS_PUSH(...)
/**
 * @def DISABLE_WARNINGS_POP()
 * @brief Placeholder for non-Microsoft compilers (ignored for non-Microsoft compilers).
 */
#define DISABLE_WARNINGS_POP()
#endif
// NOLINTEND(*-macro-usage)
