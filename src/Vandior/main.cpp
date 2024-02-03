#include "FileReaderError.hpp"
#include "Vandior/vandior.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26408 26409 26415 26418 26426 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26462 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826 26827)

#include <CLI/CLI.hpp>

DISABLE_WARNINGS_POP()

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vandior`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

namespace {
    auto setupSpdlog() -> void {
        spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
        const auto console = spdlog::stdout_color_mt(R"(console)");
        spdlog::set_default_logger(console);
    }
    auto timeTokenizer(vnd::Tokenizer &tokenizer, std::vector<vnd::Token> &tokens) -> void {
        tokens.clear();
        AutoTimer timer("tokenizer.tokenize()");
        tokens = tokenizer.tokenize();
    }
    auto readFromFile(const std::string &filename) -> std::string {
        AutoTimer timer("readFromFile");
        std::filesystem::path filePath = filename;

        if(!std::filesystem::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filename); }
        if(!std::filesystem::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filename); }

        std::stringstream buffer;

        if(std::ifstream fileStream{filePath, std::ios::in | std::ios::binary}; fileStream.is_open()) {  // NOLINT(*-include-cleaner,  hicpp-signed-bitwise)
            // Ensure
            // that the file is opened securely
            fileStream.exceptions(std::ios::failbit | std::ios::badbit);  // NOLINT(hicpp-signed-bitwise)

            try {
                buffer << fileStream.rdbuf();
            } catch(const std::ios_base::failure &e) {
                throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filename, e.what());
            }
        } else {
            // Handle the case when the file cannot be opened,
            // You might throw an exception or return an error indicator
            throw FILEREADEREERRORF("Unable to open file: {}", filename);
        }

        // Extract the content as a string
        return buffer.str();
    }
}  // namespace
// constexpr std::string_view code2 = R"('a' '\\' '')";
// constexpr std::string_view code2 = R"("a" "\\" "")";
DISABLE_WARNINGS_PUSH(26461 26821)
// NOLINTNEXTLINE(bugprone-exception-escape, readability-function-cognitive-complexity)
#ifdef _WIN32  // Windows
constexpr std::string_view filename = "../../../input.vn";
#elif defined __unix__  // Linux and Unix-like systems
constexpr std::string_view filename = "../../../input.vn";  // Linux and Unix
#endif
auto main(int argc, const char *const argv[]) -> int {
    // NOLINTNEXTLINE
    setupSpdlog();
    std::string str;
    try {
        str = readFromFile(filename.data());
    } catch(std::runtime_error &e) { LINFO("error {}", e.what()); }
    std::string_view code(str);
    LINFO("{}", code);
    LINFO("code length {}", code.length());
    try {
        CLI::App app{
            FORMAT("{} version {}", Vandior::cmake::project_name, Vandior::cmake::project_version)};  // NOLINT(*-include-cleaner)

        std::optional<std::string> message;  // NOLINT(*-include-cleaner)
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        CLI11_PARSE(app, argc, argv)

        if(show_version) {
            LINFO("{}", Vandior::cmake::project_version);
            return EXIT_SUCCESS;  // NOLINT(*-include-cleaner)
        }
        vnd::Tokenizer tokenizer{code, filename};
        std::vector<vnd::Token> tokens;
        timeTokenizer(tokenizer, tokens);
        for(const auto &item : tokens) { LINFO("{}", item); }
        std::vector<vnd::Instruction> instructions;
        AutoTimer tim("tokenizer total time");
        size_t line = tokens.at(0).getLine();
        for(const vnd::Token &token : tokens) {
            if(token.getType() == vnd::TokenType::COMMENT) [[unlikely]] { continue; }
            if(token.getLine() >= line) [[likely]] {
                if(instructions.empty() || instructions.back().canTerminate()) [[likely]] {
                    if(!instructions.empty()) { LINFO("{}", instructions.back().getLastType()); }
                    instructions.emplace_back(vnd::Instruction::create(filename));
                } else if(instructions.back().typeToString().back() != "EXPRESSION" && token.getType() != vnd::TokenType::STRING)
                    [[unlikely]] {
                    throw vnd::InstructionException(token);
                    break;
                }
                line = token.getLine() + 1;
            }
            instructions.back().checkToken(token);
        }
        vnd::Transpiler transpiler = vnd::Transpiler::create(instructions);
        transpiler.transpile();
        /* vnd::Parser parser(code);
        Timer timeAst("ast creation time");
        auto ast = parser.parse();
        prettyPrint(*ast);
        LINFO("{}", timeAst);*/
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }  // NOLINT(*-include-cleaner)

    return EXIT_SUCCESS;  // Return appropriate exit code
}
DISABLE_WARNINGS_POP()
