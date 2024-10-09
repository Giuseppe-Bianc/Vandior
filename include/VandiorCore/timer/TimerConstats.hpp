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
}  // namespace vnd