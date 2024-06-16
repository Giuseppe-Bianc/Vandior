//
// Created by gbian on 15/05/2024.
//

#pragma once

#include "Vandior/vandior.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape, readability-function-cognitive-complexity)
#ifdef _WIN32  // Windows
#ifdef __MINGW32__
constexpr std::string_view filename = R"(..\..\..\..\input.vn)";  // windows mingw form editor, use this when building for mingw
#elifdef __clang__
constexpr std::string_view filename = R"(..\..\..\..\input.vn)";  // windows mingw form editor, use this when building for clang
#else
constexpr std::string_view filename = R"(..\..\..\input.vn)";
#endif
#elif defined __unix__  // Linux and Unix-like systems
// constexpr std::string_view filename = "../../../../input.vn";  // Linux and Unix  form editor
constexpr std::string_view filename = "../../../input.vn";  // Linux and Unix
#endif
