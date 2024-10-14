#pragma once
// clang-format off
#include "headers.hpp"
#include "lexer/Tokenizer.hpp"
#include "parser/ParserException.hpp"
#include "parser/Statement.hpp"
#include "transpiler/Transpiler.hpp"
// clang-format on

// NOLINTBEGIN(*-const-correctness)
namespace vnd {
    auto timeParser(std::vector<Statement> &ast, vnd::Parser &parser) -> void;

    [[nodiscard]] auto timeParse(Parser &parser) -> std::vector<Statement>;
}  // namespace vnd

// NOLINTEND(*-const-correctness)
