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
    switch (token.getType()) {
        using enum TokenType;
    case IDENTIFIER:
        checkIdentifier();
    default:
        _tokens = {};
    }
}

void vnd::Instruction::checkIdentifier() noexcept {
    using enum TokenType;
    using enum InstructionType;
    /* if(isExpression()) {
        this->_tokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, DOT_OPERATOR, OPEN_BRACKETS, OPEN_SQUARE_BRACKETS};
        this->emplaceUnaryOperator(type);
        this->emplaceExpressionTokens();
        return;
    }
    switch(this->lastInstructionType()) {
    case BLANK:
    case OPERATION:
        this->setLastInstructionType(OPERATION);
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL,      DOT_OPERATOR, COMMA,
                               OPEN_BRACKETS,  OPEN_SQUARE_BRACKETS, eofTokenType};
        if(previousTokens.empty()) { this->allowedTokens.emplace_back(OPEN_BRACKETS); }
        this->emplaceUnaryOperator(type);
        break;
    case DECLARATION:
        if(!this->isPreviousEmpty() && this->previousTokensLast() == COLON) {
            this->allowedTokens = {EQUAL_OPERATOR, OPEN_SQUARE_BRACKETS, eofTokenType};
            break;
        }
        this->allowedTokens = {COMMA, COLON};
        break;
    case FOR_STRUCTURE:
        if(this->isPreviousEmpty()) {
            this->allowedTokens = {};
            break;
        }
        if(this->previousTokensLast() == KEYWORD_VAR) {
            this->allowedTokens = {COLON};
            break;
        }
        this->allowedTokens = {EQUAL_OPERATOR};
        break;
    case DEFINITION:
        this->allowedTokens = {OPEN_BRACKETS};
        break;
    case PARAMETER_DEFINITION:
        if(this->previousTokensLast() == COLON) {
            this->allowedTokens = {COMMA, CLOSED_BRACKETS};
            break;
        }
        this->allowedTokens = {COLON};
        break;
    case RETURN_DEFINITION:
        this->allowedTokens = {COMMA, OPEN_CURLY_BRACKETS};
        break;
    default:
        this->allowedTokens = {};
        break;
    }*/
}


vnd::InstructionType vnd::Instruction::lastType() const noexcept {
    if(_types.empty()) { return InstructionType::BLANK; }
    return _types.back();
}

inline bool vnd::Instruction::lastTypeIs(const InstructionType &type) const noexcept { return lastType() == type; }

bool vnd::Instruction::isExpression() const noexcept {
    using enum InstructionType;
    return lastTypeIs(PARAMETER_EXPRESSION) || lastTypeIs(ASSIGNATION) || lastTypeIs(INITIALIZATION) ||
           lastTypeIs(ARRAY_INIZIALIZATION) || lastTypeIs(EXPRESSION) || lastTypeIs(SQUARE_EXPRESSION) ||
           lastTypeIs(RETURN_EXPRESSION) || isForExpression();
}

bool vnd::Instruction::isForExpression() const noexcept {
    using enum InstructionType;
    return lastTypeIs(FOR_INITIALIZATION) || lastTypeIs(FOR_CONDITION) || lastTypeIs(FOR_STEP);
}