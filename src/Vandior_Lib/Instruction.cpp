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
        [[fallthrough]];
    case UNARY_OPERATOR:
        checkIdentifier(token.getType());
        break;
    case INTEGER:
    case DOUBLE:
    case CHAR:
    case STRING:
        [[fallthrough]];
    case BOOLEAN:
        checkValue(token.getType());
        break;
    case OPERATOR:
        [[fallthrough]];
    case MINUS_OPERATOR:
        checkOperator(token.getType());
        break;
    default:
        _tokens = {};
        break;
    }
    _tokens.emplace_back(token);
}

void vnd::Instruction::checkIdentifier(const TokenType &type) noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(isExpression()) {
        _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, DOT_OPERATOR, OPEN_PARENTESIS, OPEN_SQ_PARENTESIS};
        emplaceUnaryOperator(type);
        emplaceExpressionTokens();
        return;
    }
    switch(getLastType()) {
    case BLANK:
    case OPERATION:
        setLastType(OPERATION);
        _allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, DOT_OPERATOR, COMMA,
                               OPEN_PARENTESIS,  OPEN_SQ_PARENTESIS, eofTokenType};
        emplaceUnaryOperator(type);
        break;
    case DECLARATION:
        if(!isEmpty() && getLastTokenType() == COLON) {
            _allowedTokens = {EQUAL_OPERATOR, OPEN_SQ_PARENTESIS, eofTokenType};
            break;
        }
        break;
    case FOR_STRUCTURE:
        if(isEmpty()) {
            _allowedTokens = {};
            break;
        }
        if(getLastTokenType() == K_VAR) {
            _allowedTokens = {COLON};
            break;
        }
        _allowedTokens = {EQUAL_OPERATOR};
        break;
    case DEFINITION:
        _allowedTokens = {OPEN_PARENTESIS};
        break;
    case PARAMETER_DEFINITION:
        if(getLastTokenType() == COLON) {
            _allowedTokens = {COMMA, CLOSE_PARENTESIS};
            break;
        }
        _allowedTokens = {COLON};
        break;
    case RETURN_DEFINITION:
        _allowedTokens = {COMMA, OPEN_CUR_PARENTESIS};
        break;
    default:
        _allowedTokens = {};
        break;
    }
}

void vnd::Instruction::checkValue(const TokenType &type) noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(isExpression()) {
        _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(type == STRING) {
            _allowedTokens.emplace_back(DOT_OPERATOR);
            _allowedTokens.emplace_back(OPEN_SQ_PARENTESIS);
        }
        this->emplaceExpressionTokens();
        return;
    }
    _allowedTokens = {};
}

void vnd::Instruction::checkOperator(const TokenType &type) noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(isExpression()) {
        _allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, OPEN_PARENTESIS};
        if(type == MINUS_OPERATOR) { _allowedTokens.emplace_back(OPERATOR); }
        return;
    }
    _allowedTokens = {};
}


vnd::InstructionType vnd::Instruction::getLastType() const noexcept {
    if(_types.empty()) { return InstructionType::BLANK; }
    return _types.back();
}

void vnd::Instruction::setLastType(const InstructionType &type) noexcept {
    if(_types.empty()) { return; }
    _types.pop_back();
    _types.emplace_back(type);
}

TokenType vnd::Instruction::getLastTokenType() const noexcept {
    if(_tokens.empty()) { return TokenType::UNKNOWN; }
    return _tokens.back().getType();
}

inline bool vnd::Instruction::lastTypeIs(const InstructionType &type) const noexcept { return getLastType() == type; }

[[nodiscard]] inline bool vnd::Instruction::getLastBooleanOperator() const noexcept {
    if(_booleanOperators.empty()) { return false; }
    return _booleanOperators.back();
}

inline bool vnd::Instruction::isEmpty() const noexcept { return _tokens.empty(); }

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

inline bool vnd::Instruction::emplaceTokenType(const InstructionType &instruction, const TokenType token) noexcept {
    if(lastTypeIs(instruction)) {
        _allowedTokens.emplace_back(token);
        return true;
    }
    return false;
}

void vnd::Instruction::emplaceExpressionTokens() noexcept {
    using enum InstructionType;
    using enum TokenType;
    emplaceBooleanOperator();
    if(emplaceTokenType(SQUARE_EXPRESSION, CLOSE_SQ_PARENTESIS)) { return; }
    if(this->emplaceTokenType(EXPRESSION, CLOSE_PARENTESIS)) { return; }
    if(lastTypeIs(PARAMETER_EXPRESSION)) {
        _allowedTokens.emplace_back(CLOSE_PARENTESIS);
        _allowedTokens.emplace_back(COMMA);
        return;
    }
    if(lastTypeIs(ARRAY_INIZIALIZATION)) {
        _allowedTokens.emplace_back(CLOSE_SQ_PARENTESIS);
        _allowedTokens.emplace_back(COMMA);
        return;
    }
    if(this->emplaceForTokens()) { return; }
    this->emplaceCommaEoft();
}

inline void vnd::Instruction::emplaceCommaEoft() noexcept {
    _allowedTokens.emplace_back(TokenType::COMMA);
    _allowedTokens.emplace_back(eofTokenType);
}

inline void vnd::Instruction::emplaceBooleanOperator() noexcept {
    if(!getLastBooleanOperator()) { _allowedTokens.emplace_back(TokenType::BOOLEAN_OPERATOR); }
}

inline bool vnd::Instruction::emplaceForTokens() noexcept {
    if(isForExpression()) {
        _allowedTokens.emplace_back(TokenType::OPEN_CUR_PARENTESIS);
        if(!lastTypeIs(InstructionType::FOR_STEP)) { _allowedTokens.emplace_back(TokenType::COMMA); };
        return true;
    }
    return false;
}

inline void vnd::Instruction::emplaceUnaryOperator(const TokenType& type) noexcept {
    if(type != TokenType::UNARY_OPERATOR) { _allowedTokens.emplace_back(TokenType::UNARY_OPERATOR); }
}