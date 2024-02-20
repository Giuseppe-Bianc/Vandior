#include "Vandior/InstructionException.hpp"

// clang-format off
//  NOLINTNEXTLINE(*-include-cleaner)
vnd::InstructionException::InstructionException(const Token &token) : std::runtime_error(FORMAT("Unexpected token '{}' Line: {}, Column: {}", token.getValue(), token.getLine(), token.getColumn())) {}
// clang-format off