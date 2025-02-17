/**
 * @file Log.hpp
 * @brief Header file for logging functionality using the SPDLOG library.
 * @details This file provides macros and initialization functions for logging using the SPDLOG library.
 * The logging macros cover various logging levels including trace, debug, info, warn, error, and critical.
 * The initialization macro sets up the logging configuration with default settings.
 *
 * @defgroup Logging Macros
 * @brief Macros for logging messages at various levels.
 * @{
 * @section Overview
 * This module provides a set of macros for logging messages at different severity levels. The macros
 * wrap around the corresponding functions provided by the SPDLOG library, making it easy to integrate
 * logging into your application. The available macros are:
 * - LTRACE: Logs trace messages, which are the most detailed and typically used for debugging.
 * - LDEBUG: Logs debug messages, useful during development and testing.
 * - LINFO: Logs informational messages about the application's state.
 * - LWARN: Logs warning messages that indicate potential issues.
 * - LERROR: Logs error messages for non-critical errors.
 * - LCRITICAL: Logs critical messages for severe errors requiring immediate attention.
 *
 * @section Initialization
 * The logging system must be initialized before any logging can occur. Use the INIT_LOG macro to set up
 * the logging configuration with default settings. This includes setting a default pattern for log messages
 * and creating a console logger.
 *
 * @code
 * INIT_LOG();
 * LINFO("Logging system initialized.");
 * @endcode
 */
#pragma once
// NOLINTBEGIN(*-include-cleaner)

// clang-format off
#include "disableWarn.hpp"
#include <source_location>
#include <iostream>
#include "format.hpp"
// clang-format on

/** \cond */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)
/** \endcond */

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

DISABLE_WARNINGS_POP()
/** \cond */
#ifndef _MSC_VER
/**
 * @brief Disable GCC warning about uninitialized variables for spdlog.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#include <spdlog/details/null_mutex.h>
#pragma GCC diagnostic pop
#endif
/** \endcond */

/**
 * @brief Macro for logging trace messages using SPDLOG_TRACE.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log detailed tracing information for debugging purposes.
 * This macro is a wrapper around the SPDLOG_TRACE macro provided by the spdlog library.
 * Example usage:
 * @code
 * LTRACE("This is a trace message: {}", some_variable);
 * @endcode
 * @see spdlog::trace
 */
#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)

/**
 * @brief Macro for logging debug messages using SPDLOG_DEBUG.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log debug information helpful during development and testing.
 * This macro is a wrapper around the SPDLOG_DEBUG macro provided by the spdlog library.
 * Example usage:
 * @code
 * LDEBUG("This is a debug message: {}", some_variable);
 * @endcode
 * @see spdlog::debug
 */
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)

/**
 * @brief Macro for logging informational messages using SPDLOG_INFO.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log general information about the application's state.
 * This macro is a wrapper around the SPDLOG_INFO macro provided by the spdlog library.
 * Example usage:
 * @code
 * LINFO("This is an info message: {}", some_variable);
 * @endcode
 * @see spdlog::info
 */
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)

/**
 * @brief Macro for logging warning messages using SPDLOG_WARN.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log non-critical warnings that might indicate potential issues.
 * This macro is a wrapper around the SPDLOG_WARN macro provided by the spdlog library.
 * Example usage:
 * @code
 * LWARN("This is a warning message: {}", some_variable);
 * @endcode
 * @see spdlog::warn
 */
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)

/**
 *  @brief Macro for logging error messages using SPDLOG_ERROR.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log errors that do not prevent the application from continuing.
 * This macro is a wrapper around the SPDLOG_ERROR macro provided by the spdlog library.
 * Example usage:
 * @code
 * LERROR("This is an error message: {}", some_variable);
 * @endcode
 * @see spdlog::error
 */
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)

/**
 * @brief Macro for logging critical messages using SPDLOG_CRITICAL.
 * @param ... Variable arguments to be formatted and logged.
 * @details Use this macro to log critical errors that require immediate attention.
 * This macro is a wrapper around the SPDLOG_CRITICAL macro provided by the spdlog library.
 * Example usage:
 * @code
 * LCRITICAL("This is a critical message: {}", some_variable);
 * @endcode
 * @see spdlog::critical
 */
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

inline std::string get_current_timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    return FORMAT("{}.{:03d}", std::chrono::system_clock::from_time_t(std::chrono::system_clock::to_time_t(now)), ms.count());
}

// clang-format off
inline void my_error_handler(const std::string &msg) {
    const std::source_location &location = std::source_location::current();
    std::cerr << FORMAT("Error occurred:\n  Timestamp: {}\n", get_current_timestamp());
    std::cerr << FORMAT("  Thread ID: {}\n", std::this_thread::get_id());
    std::cerr << FORMAT("  Message: {}\n", msg);
    std::cerr << FORMAT("  Function: {}, File: {}, Line: {}, Column: {}\n", location.function_name(), location.file_name(), location.line(),location.column());
}
// clang-format on

inline void setup_logger() {
    std::vector<spdlog::sink_ptr> sinks;

    // Console sink (colored, for trace to info levels)
    const auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdout_sink->set_level(spdlog::level::trace);  // Log info and below (trace, debug, info)

    // Stderr sink (colored, for warn to critical levels)
    const auto stderr_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    stderr_sink->set_level(spdlog::level::warn);  // Log warn and above (warn, error, critical)

    // Add sinks to the logger, ensuring no duplicate output for same log level
    sinks.push_back(stdout_sink);
    // sinks.push_back(stderr_sink);

    // Create logger with the defined sinks
    const auto logger = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());
    logger->set_pattern(R"(%^[%T %l] %v%$)");  // Log pattern
    logger->set_level(spdlog::level::trace);   // Minimum log level (trace)

    // Set this logger as the default logger
    spdlog::set_default_logger(logger);
}

/**
 * @brief Initialize the logging system with default configurations.
 * @details This macro initializes the logging system with a default pattern and creates a console logger.
 * If the initialization fails, it outputs an error message to stderr.
 * The default pattern used is "[HH:MM:SS level] message".
 * Example usage:
 * @code
 * INIT_LOG();
 * @endcode
 * @see spdlog::set_pattern
 * @see spdlog::stdout_color_mt
 * @see spdlog::set_default_logger
 */
#define INIT_LOG()                                                                                                                         \
    spdlog::set_error_handler(my_error_handler);                                                                                           \
    try {                                                                                                                                  \
        setup_logger();                                                                                                                    \
    } catch(const spdlog::spdlog_ex &ex) {                                                                                                 \
        std::cerr << "Logger initialization failed: " << ex.what() << '\n';                                                                \
    } catch(const std::exception &e) { std::cerr << "Unhandled exception: " << e.what() << '\n'; } catch(...) {                            \
        std::cerr << "An unknown error occurred Logger initialization failed.\n";                                                          \
    }

/// @}
// NOLINTEND(*-include-cleaner)
