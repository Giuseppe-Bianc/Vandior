// NOLINTBEGIN(*-include-cleaner, *-env33-c)
#include "FileReaderError.hpp"
#include "Vandior/vandior.hpp"
#include <future>

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26408 26409 26415 26418 26426 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26462 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826 26827)
#include <CLI/CLI.hpp>

DISABLE_WARNINGS_POP()

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vandior`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#define HIDE_SYSTEM_UOTPUT

namespace {
    auto timeTokenizer(vnd::Tokenizer &tokenizer, std::vector<vnd::Token> &tokens) -> void {
        tokens.clear();
        vnd::AutoTimer timer("tokenization");
        tokens = tokenizer.tokenize();
    }
    auto readFromFile(const std::string &filename) -> std::string {
        vnd::AutoTimer timer(FORMAT("reading file {}", filename));
        const auto &filePath = std::filesystem::path(filename);
        if(!std::filesystem::exists(filePath)) { throw FILEREADEREERRORF("File not found: {}", filePath); }
        if(!std::filesystem::is_regular_file(filePath)) { throw FILEREADEREERRORF("Path is not a regular file: {}", filePath); }

        std::stringstream buffer;
        // NOLINTNEXTLINE(*-include-cleaner,  hicpp-signed-bitwise)
        if(std::ifstream fileStream{filePath, std::ios::in | std::ios::binary}; fileStream.is_open()) {
            // Ensure
            // that the file is opened securely
            fileStream.exceptions(std::ios::failbit | std::ios::badbit);  // NOLINT(hicpp-signed-bitwise)

            try {
                buffer << fileStream.rdbuf();
            } catch(const std::ios_base::failure &e) {
                throw FILEREADEREERRORF("Unable to read file: {}. Reason: {}", filePath, e.what());
            }
        } else {
            // Handle the case when the file cannot be opened,
            // You might throw an exception or return an error indicator
            throw FILEREADEREERRORF("Unable to open file: {}", filePath);
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
#ifdef __MINGW32__
constexpr std::string_view filename = "../../../../input.vn";  // windows mingw form editor, use this when building for mingw
#else
constexpr std::string_view filename = "../../../input.vn";
#endif
#elif defined __unix__  // Linux and Unix-like systems
// constexpr std::string_view filename = "../../../../input.vn";  // Linux and Unix  form editor
constexpr std::string_view filename = "../../../input.vn";  // Linux and Unix
#endif
auto extractInstructions(const std::vector<vnd::Token> &tokens) -> std::vector<vnd::Instruction> {
    std::vector<vnd::Instruction> instructions;
    auto line = tokens.at(0).getLine();
    vnd::AutoTimer ictim("Instructions creation time");
    for(const vnd::Token &token : tokens) {
        if(token.getType() == vnd::TokenType::COMMENT) [[unlikely]] { continue; }
        if(token.getLine() >= line) [[likely]] {
            if(instructions.empty() || instructions.back().canTerminate()) [[likely]] {
                // if(!instructions.empty()) { LINFO("{}", instructions.back().getLastType()); }
                instructions.emplace_back(vnd::Instruction::create(filename));
            } else if(instructions.back().typeToString().back() != "EXPRESSION" && token.getType() != vnd::TokenType::STRING)
                [[unlikely]] {
                throw vnd::InstructionException(token);
            }
            line = token.getLine() + 1;
        }
        instructions.back().checkToken(token);
    }
    return instructions;
}
// NOLINTNEXTLINE(*-function-cognitive-complexity)
auto main(int argc, const char *const argv[]) -> int {
    // NOLINTNEXTLINE
    INIT_LOG()
    if(std::system("python --version") != 0) {
        LERROR("Python not found");
        return EXIT_FAILURE;
    }
    // LINFO("{}", code);
    // LINFO("code length {}", code.length());
    try {
        CLI::App app{
            FORMAT("{} version {}", Vandior::cmake::project_name, Vandior::cmake::project_version)};  // NOLINT(*-include-cleaner)

        // std::optional<std::string> message;  // NOLINT(*-include-cleaner)
        std::optional<std::string> path;
        // app.add_option("-m,--message", message, "A message to print back out");
        app.add_option("-i,--input", path, "The input file");
        bool show_version = false;
        bool run = false;
        app.add_flag("--version, -v", show_version, "Show version information");
        app.add_flag("--run, -r", run, "Run the compilation on resulting code");
        CLI11_PARSE(app, argc, argv)

        if(show_version) {
            LINFO("{}", Vandior::cmake::project_version);
            return EXIT_SUCCESS;  // NOLINT(*-include-cleaner)
        }
        std::string str = readFromFile(path.value_or(filename.data()));

        std::string_view code(str);
        vnd::Tokenizer tokenizer{code, filename};
        std::vector<vnd::Token> tokens;
        timeTokenizer(tokenizer, tokens);
        // for(const auto &item : tokens) { LINFO("{}", item); }
        std::vector<vnd::Instruction> instructions = extractInstructions(tokens);
        vnd::Timer tim("transpiling time");
        if(vnd::Transpiler transpiler = vnd::Transpiler::create(instructions); !transpiler.transpile()) { return EXIT_FAILURE; }
        LINFO("{}", tim);
        if(run) {
            std::string_view command;
#ifdef HIDE_SYSTEM_UOTPUT
#ifdef _WIN32
            command = "g++ --version > NUL";
#else
            command = "g++ --version > /dev/null";
#endif
#else
            command = "g++ --version";
#endif

            if(std::system(command.data()) == 0) {
                vnd::Timer rtim("compile code time");

                // Compile the code
                int compileResult = std::system("g++ --std=c++20 output.cpp");
                LINFO("{}", rtim);
                if(compileResult != 0) {
                    LERROR("Compilation failed");
                    return EXIT_FAILURE;
                }
            } else {
                LERROR("Failed to execute command: {}", command);
                return EXIT_FAILURE;
            }
        }
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
    return EXIT_SUCCESS;  // Return appropriate exit code
}
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-env33-c)
