#include "Vandior/Instruction.hpp"

vnd::InstructionException::InstructionException(const Token &token)
  : std::runtime_error(FORMAT("Unexpected token {}\nLine: {}, Column: {}", token.getValue(), token.getLine(), token.getColumn())) {}