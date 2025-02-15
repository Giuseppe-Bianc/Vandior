/*
 * Created by gbian on 09/10/2024.
 */

#pragma once

#include "../headersCore.hpp"

namespace vnd {
    static inline constexpr auto simpleFormat = "{}: Time = {}";
    static inline constexpr auto bigTimesFormat = "Time = {}";
    static inline constexpr auto bigTitleTimeFormat = "|{0: ^{1}}|{2: ^{3}}|";
    static inline constexpr auto bigFormat = "\n{0:-^{1}}\n{2}\n{0:-^{1}}";
    static inline constexpr auto compactFormat = "[{}]{}";
    static inline constexpr auto detailedFormat = "Timer '{}' measured a duration of {}";
    static inline constexpr auto blockPatternFormat = "{0:=^{1}}|{0:=^{1}}|{0:=^{1}}|{0:=^{1}}";
    static inline constexpr auto blockTimesFormat = "Time:{}";
    static inline constexpr auto blockFormat = "\n{0}\n{2: ^{1}}\n{0}\n{3: ^{1}}\n{0}";
    static inline constexpr auto minimalFormat = "{} - {}";
    static inline constexpr auto timeItFormat = "{} for {} tries";
    static inline constexpr std::size_t TILEPADDING = 10;
    using clock = ch::high_resolution_clock;
    using time_point = ch::time_point<clock>;
    using nanolld = ch::duration<long double, std::nano>;
    using microlld = ch::duration<long double, std::micro>;
    using millilld = ch::duration<long double, std::milli>;
    using seclld = ch::duration<long double>;
}  // namespace vnd