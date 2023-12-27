#pragma once
// NOLINTBEGIN
// #define GLM_FORCE_INTRINSICS
// #define GLM_ENABLE_EXPERIMENTAL
// #define GLM_FORCE_SILENT_WARNINGS
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #define GLM_FORCE_WIN32
// #define GLM_FORCE_SIZE_T_LENGTH
// #define GLM_FORCE_PURE
// #define GLM_FORCE_EXPLICIT_CTOR
// #define GLM_FORCE_CXX20
// #define GLM_FORCE_UNRESTRICTED_GENTYPE
// #define GLM_FORCE_PRECISION_HIGHP_DOUBLE
// #define GLM_FORCE_PRECISION_HIGHP_FLOAT
// #define GLM_FORCE_PRECISION_HIGHP_INT
// #define GLM_FORCE_PRECISION_HIGHP_UINT

#include "disableWarn.hpp"
// clang-format off
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820
    6244 6285 6385 6386 26409 26415 26418
    26429 26432 26437 26438 26440 26446 26447
    26450 26451 26455 26457 26459 26460 26461
    26467 26472 26473 26474 26475 26481 26482
    26485 26490 26491 26493 26494 26495 26496
    26497 26498 26800 26814 26818 26826)

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
#include <fmt/format.h>
#include <fmt/core.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <omp.h>
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
#include "casts.hpp"
// #include "glm_matld.h"
// #include "glm_prety_string_cast.h"
// Restore warning levels.
DISABLE_WARNINGS_POP()

// clang-format off
#ifdef _WIN32
#ifdef _MSC_VER
#ifdef USE_STD_FORMAT
#define D_FORMAT(...) std::format(__VA_ARGS__)
#else
#define D_FORMAT(...) fmt::format(__VA_ARGS__)
#endif
#endif
#else
#define D_FORMAT(...) fmt::format(__VA_ARGS__)
#endif
// clang-format on

static inline constexpr long double NINFINITY = std::numeric_limits<long double>::infinity();
static inline constexpr long double PI = std::numbers::pi_v<long double>;
static inline constexpr long double TWO_PI = 2 * PI;
static inline constexpr long double HALF_PI = PI / 2;
static inline constexpr char CNL = '\n';
static inline constexpr char CCR = '\r';
static inline constexpr char PNT = '.';
static inline constexpr char ECR = 'E';
static inline constexpr const char *CRNL = "\r\n";

#ifdef _WIN32                              // Windows
static inline constexpr auto NEWL = CRNL;  // Windows
#elif defined macintosh                    // OS 9
static inline constexpr const char *NEWL = &CCR;  // Classic Mac OS
#elif defined __unix__                     // Linux and Unix-like systems
static inline constexpr const char *NEWL = &CNL;  // Linux and Unix
#elif defined __APPLE__                    // macOS
static inline constexpr const char *NEWL = &CNL;  // macOS
#elif defined __VMS                        // OpenVMS
static inline constexpr const char *NEWL = &CRNL;  // OpenVMS
#elif defined __FreeBSD__                  // FreeBSD
static inline constexpr const char *NEWL = &CNL;  // FreeBSD
#else
static inline constexpr const char *NEWL = &CNL;  // Default case
#endif

//#define SYSPAUSE()                                                                                                               \
//    do {                                                                                                                         \
//        LINFO("Press enter to exit...");                                                                                         \
//        std::cin.ignore();                                                                                                       \
//    } while(0);

// NOLINTEND