#pragma once
// clang-format off
#include "headers.hpp"
#include "Log.hpp"
//#include "timer/Timer.hpp"
#include "lexer/Tokenizer.hpp"
//#include "FolderCreationResult.hpp"
#include "FileReader.hpp"
//#include "parser/Parser.hpp"
#include "parser/ParserException.hpp"
#include "transpiler/Transpiler.hpp"
// clang-format on

// NOLINTBEGIN(*-const-correctness)
namespace vnd {
    auto timeTokenizer(Tokenizer &tokenizer, std::vector<Token> &tokens) -> void;

    auto timeParser(std::unique_ptr<vnd::ASTNode> &ast, vnd::Parser &parser) -> void;

    [[nodiscard]] auto timeParse(Parser &parser) -> std::unique_ptr<ASTNode>;
}  // namespace vnd
// NOLINTEND(*-const-correctness)