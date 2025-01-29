/*
 * Created by gbian on 12/10/2024.
 * Copyright (c) 2024 All rights reserved.
 */

#pragma once

#include <Vandior/vandior.hpp>

static inline constexpr std::size_t t_line = 5;
static inline constexpr std::size_t t_line2 = 42;
static inline constexpr std::size_t t_line3 = 100;
static inline constexpr std::size_t t_line4 = 10;
static inline constexpr std::size_t t_colum = 6;
static inline constexpr std::size_t t_colum2 = 5;
static inline constexpr std::size_t t_colum3 = 7;
static inline constexpr std::size_t t_colum4 = 8;
static inline constexpr std::size_t t_colum5 = 9;
static inline constexpr std::size_t t_colum6 = t_line4;
static inline constexpr std::size_t t_colum8 = 11;
static inline constexpr std::size_t t_colum10 = 15;
static inline constexpr std::size_t t_colum11 = 13;
static inline constexpr std::size_t t_colum13 = 17;
static inline constexpr std::size_t t_colum14 = 12;
static inline constexpr auto identf = vnd::TokenType::IDENTIFIER;
static inline constexpr auto inte = vnd::TokenType::INTEGER;
static inline constexpr auto doub = vnd::TokenType::DOUBLE;
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
static inline constexpr std::string_view filename = R"(.\unknown.vn)";
#else
static inline constexpr std::string_view filename = R"(./unknown.vn)";
#endif
static inline constexpr auto pIntInvArg = "parse_integer: invalid argument";
static inline constexpr auto pIntOutRng = "parse_integer: result out of range";
static inline constexpr auto pIntTrlChr = "parse_integer: trailing characters";
// clang-format off
static inline constexpr auto long_input = R"(x:i32[2] =  {{-22, 23}, function(1, 2)}, c:char = 'c' ,c:string = "ssssss", b:bool = true, cmp:c32 = 11.2if, dcmp: c64 = 11.2i, c: char = nullptr, cc:i32 = 12 + 21, cc + 12, xx:i32[2] = {2, 2}, xxx:i32[] = {2, 2, 2, 2}, ssss: tr = dnddd)"sv;
// clang-format on
static inline constexpr std::string_view ffilename = "file1.cpp";
static inline constexpr std::string_view filename2 = "example.cpp";
static inline constexpr std::string_view filename3 = "new_file.cpp";
static inline constexpr std::string_view filename4 = "unknown";
static inline constexpr std::string_view timerName = "My Timer";
static inline constexpr std::string_view timerName2 = "Timer";
static inline constexpr std::string_view timerBigs = "-----------";
static inline constexpr std::string_view timerTime1 = "ms";
static inline constexpr std::string_view timerTime2 = "ns";
static inline constexpr std::string_view testPaths = "/test/path";
static inline constexpr long long int timerSleap = 12;
static inline constexpr long long int timerSleap2 = 5;
static inline constexpr std::size_t timerCicles = 1000000;
static inline constexpr long double timerResolution = 5.0L;
static inline constexpr std::size_t timestampSize = 24;
static inline constexpr auto inputTabs = "\t\t\t\t"sv;
static inline constexpr auto inputMixed = "\t\t\tHello, World!"sv;
static inline constexpr auto inputText = "Hello, World!"sv;
static inline constexpr auto inputMixedMidle = "Hello\tWorld"sv;
static inline constexpr auto inputMixedSpace = "\t \t\tHello"sv;
static inline constexpr auto inputMixedSpace2 = "\t\t  \t\tWorld"sv;
static inline constexpr auto inputSpace = "     "sv;
static inline constexpr auto inputMixedSpace3 = "\t \t Hello"sv;