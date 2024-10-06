#pragma once
// clang-format off
#include "headers.hpp"
#include "lexer/Tokenizer.hpp"
#include "parser/ParserException.hpp"
#include "transpiler/Transpiler.hpp"
// clang-format on

// NOLINTBEGIN(*-const-correctness)
namespace vnd {
    auto timeParser(std::unique_ptr<vnd::ASTNode> &ast, vnd::Parser &parser) -> void;

    [[nodiscard]] auto timeParse(Parser &parser) -> std::unique_ptr<ASTNode>;
}  // namespace vnd

// NOLINTEND(*-const-correctness)
