// NOLINTBEGIN(*-include-cleaner, *-env33-c)

#include "Costanti.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26408 26409 26415 26418 26426 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26462 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826 26827)
#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

DISABLE_WARNINGS_POP()
namespace vnd {
    // NOLINTNEXTLINE(*-use-anonymous-namespace)
    static auto timeTokenizer(Tokenizer &tokenizer, std::vector<Token> &tokens) -> void {
        tokens.clear();
#ifdef INDEPT
        const AutoTimer timer("tokenization");
#endif
        tokens = tokenizer.tokenize();
    }
}  // namespace vnd

DISABLE_WARNINGS_PUSH(26461 26821)

// static inline constexpr auto sequence = std::views::iota(0, 9999);

template <typename T> std::vector<std::pair<size_t, const T &>> enumerate(const std::vector<T> &vec) {
    std::vector<std::pair<size_t, const T &>> enumerated;
    for(std::size_t index = 0; const auto &item : vec) {
        enumerated.emplace_back(index, item);
        ++index;
    }
    return enumerated;
}
// NOLINTNEXTLINE(*-function-cognitive-complexity)
auto main(int argc, const char *const argv[]) -> int {
    // NOLINTNEXTLINE
    INIT_LOG()
    std::cout << FORMATST("messagio da {}\n", "std::format");
    try {
        CLI::App app{FORMAT("{} version {}", Vandior::cmake::project_name, Vandior::cmake::project_version)};  // NOLINT(*-include-cleaner)
        // std::optional<std::string> message;  // NOLINT(*-include-cleaner)
        std::optional<std::string> path;
        // app.add_option("-m,--message", message, "A message to print back out");
        app.add_option("-i,--input", path, "The input file");
        bool show_version = false;
        bool compile = false;
        bool run = false;
        bool clean = false;
        app.add_flag("--version, -v", show_version, "Show version information");
        app.add_flag("--compile, -c", compile, "Compile the resulting code");
        app.add_flag("--run, -r", run, "Compile the resulting code and execute it");
        app.add_flag("--clean, -x", clean, "Clean before building");
        CLI11_PARSE(app, argc, argv)
        if(show_version) {
            LINFO("{}", Vandior::cmake::project_version);
            return EXIT_SUCCESS;
        }
        const auto porfilename = path.value_or(filename.data());
        if(clean) {
            const auto folderPath = vnd::GetBuildFolder(fs::path(porfilename));
            LINFO("folder  to delete {}", folderPath);
            auto unused = vnd::FolderDeletionResult::deleteFolder(folderPath);
            LINFO("Cleaning the project");
            // return EXIT_SUCCESS;
        }
        const auto str = vnd::readFromFile(porfilename);
        const std::string_view code(str);
        vnd::Tokenizer tokenizer{code, porfilename};
        std::vector<vnd::Token> tokens;
        vnd::timeTokenizer(tokenizer, tokens);
        LINFO("num tokens {}", tokens.size());
        std::string input;
        std::getline(std::cin, input);
        LINFO("Input: {}", input);
        vnd::Parser parser{input, "input.vn"};
        for(const auto progrmamAST = vnd::timeParse(parser); const auto &statement : progrmamAST) {
            const auto &token = statement.get_token();
            // FIXME: Output the actual token, not the default one.
            if(token.getType() == vnd::TokenType::UNKNOWN) {
                LINFO("lo statement non e' generato da nessun token");
            } else {
                LINFO("{}", token);
            }
            for(const auto &[index, node] : enumerate(statement.get_nodes())) {
                LINFO("AST num {}:", index);
                node ? prettyPrint(*node) : LINFO("EMPTY");
            }
        }
        vnd::Transpiler transpiler{input, filename};
        LINFO("transpiled code: {}", transpiler.transpile());
    } catch(const std::exception &e) {
        // Handle any other types of exceptions
        LERROR("Unhandled exception in main: {}", e.what());
    } catch(...) {
        // Handle any other types of exceptions
        LERROR("An unknown error occurred while creating the folder.");
    }
    return EXIT_SUCCESS;  // Return appropriate exit code
}

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-env33-c)
