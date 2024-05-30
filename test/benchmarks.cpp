//
// Created by gbian on 30/05/2024.
//
// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers)

#include "catch2/benchmark/catch_benchmark.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Vandior/vandior.hpp>

#ifdef _WIN32  // Windows
#ifdef __MINGW32__
constexpr std::string_view tfile = R"(..\..\..\test\test_imp.vn)";  // windows mingw form editor, use this when building for mingw
#elifdef __clang__
constexpr std::string_view tfile = R"(..\..\..\test\test_imp.vn)";  // windows mingw form editor, use this when building for clang
#else
constexpr std::string_view tfile = R"(..\..\test\test_imp.vn)";
#endif
#elif defined __unix__  // Linux and Unix-like systems
// constexpr std::string_view tfile = "../../../test/test_imp.vn";  // Linux and Unix  form editor
constexpr std::string_view tfile = "../../test/test_imp.vn";  // Linux and Unix
#endif

TEST_CASE("Tokenizer Benchmark", "[benchmark]") {
    const std::string input = R"(fun funzione(num: i8[][2]): i8[2], Derived {
        println("Funzione:\t{}", num)
        var s: string = "CCCCC"
        var d: Derived
        return {1, 2}, d
    })";

    vnd::Tokenizer tokenizer(input);

    BENCHMARK("Tokenize sample input") { return tokenizer.tokenize(); };
}

TEST_CASE("Tokenizer Benchmark from file", "[benchmark]") {
    const std::string input = vnd::readFromFile(tfile.data());
    vnd::Tokenizer tokenizer(input);

    BENCHMARK("Tokenize input file") { return tokenizer.tokenize(); };
}

// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers, *-magic-numbers)