// NOLINTBEGIN(*-include-cleaner, *-env33-c)
#include "Costanti.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26408 26409 26415 26418 26426 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26462 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826 26827)
#include <CLI/CLI.hpp>

DISABLE_WARNINGS_POP()

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vandior`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>
#include <system_error>

#define HIDE_SYSTEM_OUTPUT

namespace {
    auto timeTokenizer(vnd::Tokenizer &tokenizer, std::vector<vnd::Token> &tokens) -> void {
        tokens.clear();
        vnd::AutoTimer timer("tokenization");
        tokens = tokenizer.tokenize();
    }

}  // namespace
DISABLE_WARNINGS_PUSH(26461 26821)

// NOLINTNEXTLINE(*-function-cognitive-complexity)
auto main(int argc, const char *const argv[]) -> int {
    // NOLINTNEXTLINE
    INIT_LOG()
    try {
        CLI::App app{FORMAT("{} version {}", Vandior::cmake::project_name, Vandior::cmake::project_version)};  // NOLINT(*-include-cleaner)
        // std::optional<std::string> message;  // NOLINT(*-include-cleaner)
        std::optional<std::string> path;
        // app.add_option("-m,--message", message, "A message to print back out");
        app.add_option("-i,--input", path, "The input file");
        bool show_version = false;
        bool compile = false;
        bool run = false;
        app.add_flag("--version, -v", show_version, "Show version information");
        app.add_flag("--compile, -c", compile, "Compile the resulting code");
        app.add_flag("--run, -r", run, "Compile the resulting code and execute it");
        CLI11_PARSE(app, argc, argv)

        if(show_version) {
            LINFO("{}", Vandior::cmake::project_version);
            return EXIT_SUCCESS;  // NOLINT(*-include-cleaner)
        }
        auto resultFolderCreation = vnd::FolderCreationResult::createFolderNextToFile(path.value_or(filename.data()), "vnbuild");
        const auto &vnBuildFolder = resultFolderCreation.pathcref();
        if(!resultFolderCreation.success()) { return EXIT_FAILURE; }
        auto resultFolderCreationsrc = vnd::FolderCreationResult::createFolder("src", vnBuildFolder);
        auto vnSrcFolder = resultFolderCreationsrc.pathcref();
        if(!resultFolderCreationsrc.success()) {
            return EXIT_FAILURE;
        } else {
            LINFO("build folder path {}", vnSrcFolder);
        }
        auto str = vnd::readFromFile(path.value_or(filename.data()));
        const std::string_view code(str);
        vnd::Tokenizer tokenizer{code, path.value_or(filename.data())};
        std::vector<vnd::Token> tokens;
        timeTokenizer(tokenizer, tokens);
        LINFO("num tokens {}", tokens.size());

        // 2 + 3 + (4.2 / 2) * 3 + y + (true / false) - 'd' * "ciao"
        std::string input;
        std::getline(std::cin, input);
        LINFO("Input: {}", input);
        vnd::Parser parser{input, "input.vn"};
        auto ast = parser.parse();
        LINFO("print interlal function");
        LINFO("{}", ast->print());
        LINFO("comp_print interlal function");
        LINFO("{}", ast->comp_print());
        LINFO("pretyPrint external function");
        prettyPrint(*ast);
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
    return EXIT_SUCCESS;  // Return appropriate exit code
}
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-env33-c)
