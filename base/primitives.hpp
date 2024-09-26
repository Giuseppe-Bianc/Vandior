#pragma once
#include <complex>
#include <cstdint>

/**
 * This function is a formatter for the complex numbers using fmt.
 * \cond
 */
#define COMPLEX_FORMATTER(type)                                                                                                            \
    template <> struct fmt::formatter<type> {                                                                                              \
        constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }                                                            \
        template <typename FormatContext> auto format(const type &num, FormatContext &ctx) {                                               \
            return fmt::format_to(ctx.out(), "({}, {})", std::real(num), std::imag(num));                                                  \
        }                                                                                                                                  \
    };

/** \endcond */

// Signed integer types
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Unsigned integer types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Floating points types
using f32 = float;
using f64 = double;

// Complex data types
using c32 = std::complex<f32>;
using c64 = std::complex<f64>;
using namespace std::complex_literals;

COMPLEX_FORMATTER(c32)
COMPLEX_FORMATTER(c64)