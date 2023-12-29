#pragma once
// NOLINTBEGIN

// clang-format off
#include <Vandior/vandior_export.hpp>
#include "disableWarn.hpp"
// clang-format on

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

DISABLE_WARNINGS_POP()

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#include <spdlog/details/null_mutex.h>
#pragma GCC diagnostic pop
#endif

#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
// NOLINTEND
