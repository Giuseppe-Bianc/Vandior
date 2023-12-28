#include "Vandior/vandior.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26408 26409 26415 26418 26426 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26462 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826 26827)

#include <CLI/CLI.hpp>

DISABLE_WARNINGS_POP()

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vandior`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

template <typename T> void print_table(std::string_view label) {
    using lim = std::numeric_limits<T>;
    LINFO("{0:-^40}\n{1}\n{0:-^40}\n", "", label);
    LINFO("lowest        {: .20}\n", lim::lowest());
    LINFO("min           {: .20}\n", lim::min());
    LINFO("max           {: .20}\n", lim::max());
    LINFO("denorm_min    {: .20}\n", lim::denorm_min());
    LINFO("epsilon       {: .20}\n", lim::epsilon());
    LINFO("round_error   {: .20}\n", lim::round_error());
    LINFO("min_exponent  {: }\n", lim::min_exponent);
    LINFO("max_exponent  {: }\n", lim::max_exponent);
    LINFO("quiet_NaN      {}\n", lim::quiet_NaN());
    LINFO("digits        {: }\n", lim::digits);
    LINFO("digits10      {: }\n", lim::digits10);
    LINFO("+0            {: }\n", T(0));
    LINFO("-0            {: }\n", T(-0.0));
}
// NOLINTNEXTLINE(bugprone-exception-escape, readability-function-cognitive-complexity)
int main(int argc, const char *const argv[]) {
    setupSpdlog();
    try {
        AutoTimer timer{"main"};
        CLI::App app{FORMAT("{} version {}", Vandior::cmake::project_name, Vandior::cmake::project_version)};

        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        CLI11_PARSE(app, argc, argv)

        if(show_version) {
            LINFO("{}\n", Vandior::cmake::project_version);
            return EXIT_SUCCESS;
        }

        print_table<float>("FLOAT");
        print_table<double>("DOUBLE");
        print_table<long double>("LONG DOUBLE");

    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
}
