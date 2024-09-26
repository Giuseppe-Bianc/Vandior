#pragma once
#include "headers.hpp"

constexpr double _PI = std::numbers::pi_v<double>;
constexpr double _TAU = std::numbers::pi * 2;
constexpr double _E = std::numbers::e;
constexpr double _INF = std::numeric_limits<double>::infinity();

namespace vnd {
    template <typename T1, typename T2> std::common_type_t<T1, T2> mod(T1 a, T2 b) {
        using result_type = std::common_type_t<T1, T2>;
        if constexpr(std::is_same_v<result_type, i64>) {
            std::cout << "Int" << std::endl;
            return a % b;
        }
        return std::fmod(static_cast<result_type>(a), static_cast<result_type>(b));
    }
    template <typename T1, typename T2> std::common_type_t<T1, T2> pow(T1 a, T2 b) {
        return static_cast<std::common_type_t<T1, T2>>(std::pow(a, b));
    }
}  // namespace vnd

inline f64 _sqrt(f64 x) { return std::sqrt(x); }
inline f64 _sin(f64 x) { return std::sin(x); }
inline f64 _cos(f64 x) { return std::cos(x); }
inline f64 _tan(f64 x) { return std::tan(x); }
inline f64 _asin(f64 x) { return std::asin(x); }
inline f64 _acos(f64 x) { return std::acos(x); }
inline f64 _atan(f64 x) { return std::atan(x); }
inline f64 _atan2(f64 y, f64 x) { return std::atan2(y, x); }
inline f64 _sinh(f64 x) { return std::sinh(x); }
inline f64 _cosh(f64 x) { return std::cosh(x); }
inline f64 _tanh(f64 x) { return std::tanh(x); }