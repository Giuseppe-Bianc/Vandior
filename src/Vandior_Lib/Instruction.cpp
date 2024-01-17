#include <algorithm>
#include "Vandior/Instruction.hpp"

vnd::Instruction::Instruction() noexcept
  : _types({InstructionType::BLANK}),
    _allowedTokens({TokenType::K_MAIN, TokenType::K_VAR, TokenType::K_STRUCTURE, TokenType::K_FOR,
                   TokenType::K_FUN, TokenType::K_RETURN, TokenType::IDENTIFIER, TokenType::OPEN_CUR_PARENTESIS,
                   TokenType::CLOSE_CUR_PARENTESIS, eofTokenType}), _booleanOperators({false}) {
    _tokens.reserve({});
}

vnd::Instruction vnd::Instruction::create() noexcept { return Instruction(); }

std::vector<std::string> vnd::Instruction::typeToString() const noexcept {
	std::vector<std::string> result;
    result.reserve(_types.size());
    for(const InstructionType &i : _types) { result.emplace_back(FORMAT("{}", i)); }
    return result;
}

void vnd::Instruction::checkToken(const Token &token) {
    if(std::ranges::find(_allowedTokens, token.getType()) == _allowedTokens.end()) { throw InstructionException(token); }
}