#include "Vandior/InstructionException.hpp"

vnd::InstructionException::InstructionException(const Token &token)
  : std::runtime_error(
        FORMAT("Unexpected token '{}' Line: {}, Column: {}", token.getValue(), token.getLine(), token.getColumn())) {}
