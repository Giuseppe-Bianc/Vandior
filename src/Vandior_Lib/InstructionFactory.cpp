#include "Vandior/InstructionFactory.hpp"

namespace vnd {
    // NOLINTBEGIN(*-include-cleaner)
    const TokenTypeVec InstructionFactory::_startTokens = {TokenType::K_MAIN,               TokenType::K_VAR,
                                                           TokenType::K_IF,                 TokenType::K_WHILE,
                                                           TokenType::K_FOR,                TokenType::K_FUN,
                                                           TokenType::K_RETURN,             TokenType::K_BREAK,
                                                           TokenType::IDENTIFIER,           TokenType::OPEN_CUR_PARENTESIS,
                                                           TokenType::CLOSE_CUR_PARENTESIS, eofTokenType};

    const TokenTypeVec InstructionFactory::_expressionStartTokens = {TokenType::IDENTIFIER,      TokenType::INTEGER,
                                                                     TokenType::DOUBLE,          TokenType::CHAR,
                                                                     TokenType::STRING,          TokenType::BOOLEAN,
                                                                     TokenType::MINUS_OPERATOR,  TokenType::NOT_OPERATOR,
                                                                     TokenType::OPEN_PARENTESIS, TokenType::OPEN_CUR_PARENTESIS,
                                                                     TokenType::K_NULLPTR};

    InstructionFactory::InstructionFactory(const std::string_view filename, const TokenVec &tokens) noexcept
      : _instruction(Instruction::create(filename)), _filename(filename), _tokens(tokens) {}

    InstructionFactory InstructionFactory::create(const std::string_view filename, const TokenVec &tokens) noexcept {
        return InstructionFactory{filename, tokens};
    }

    Instruction InstructionFactory::getInstruction() const noexcept { return _instruction; }

    std::vector<Instruction> InstructionFactory::getInstructions() const noexcept { return _instructions; }

    void InstructionFactory::addInstruction() noexcept {
        _instructions.emplace_back(_instruction);
        _instruction = Instruction::create(_filename);
        _allowedTokens = _startTokens;
    }

    void InstructionFactory::checkToken(const Token &token) {
        if(std::ranges::find(_allowedTokens, token.getType()) == _allowedTokens.end()) { throw InstructionException(token); }
        auto tokType = token.getType();
        switch(tokType) {
            using enum TokenType;
        case IDENTIFIER:
            [[fallthrough]];
        case UNARY_OPERATOR:
            checkIdentifier(tokType);
            break;
        case INTEGER:
        case DOUBLE:
        case CHAR:
        case STRING:
        case BOOLEAN:
            [[fallthrough]];
        case K_NULLPTR:
            checkValue(tokType);
            break;
        case OPERATOR:
            [[fallthrough]];
        case MINUS_OPERATOR:
            checkOperator(tokType);
            break;
        case EQUAL_OPERATOR:
            [[fallthrough]];
        case OPERATION_EQUAL:
            checkEqualOperator();
            break;
        case BOOLEAN_OPERATOR:
        case NOT_OPERATOR:
            [[fallthrough]];
        case LOGICAL_OPERATOR:
            checkBooleanLogicalOperator(tokType);
            break;
        case DOT_OPERATOR:
            [[fallthrough]];
        case COLON:
            _allowedTokens = {IDENTIFIER};
            break;
        case COMMA:
            checkComma();
            break;
        case OPEN_PARENTESIS:
            [[fallthrough]];
        case OPEN_SQ_PARENTESIS:
            checkOpenParentesis(tokType);
            break;
        case CLOSE_PARENTESIS:
            [[fallthrough]];
        case CLOSE_SQ_PARENTESIS:
            checkClosedParentesis(tokType);
            break;
        case OPEN_CUR_PARENTESIS:
            checkOpenCurParentesis();
            break;
        case CLOSE_CUR_PARENTESIS:
            checkClosedCurParentesis();
            break;
        case K_MAIN:
            checkKMain();
            break;
        case K_VAR:
            checkKVar();
            break;
        case K_IF:
            [[fallthrough]];
        case K_WHILE:
            checkKStructure();
            break;
        case K_ELSE:
            checkKElse();
            break;
        case K_FOR:
            checkKFor();
            break;
        case K_BREAK:
            _instruction.setLastType(InstructionType::BREAK);
            _allowedTokens = {eofTokenType};
            break;
        case K_FUN:
            checkKFun();
            break;
        case K_RETURN:
            checkKReturn();
            break;
        default:
            _allowedTokens = {};
            break;
        }
        if(tokType != eofTokenType) { _instruction.emplaceToken(token); }
    }
    void InstructionFactory::checkIdentifier(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.isExpression()) {
            _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
            emplaceUnaryOperator(type);
            emplaceExpressionTokens();
            return;
        }
        switch(_instruction.getLastType()) {
        case BLANK:
        case OPERATION:
            _instruction.setLastType(OPERATION);
            _allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA, eofTokenType};
            emplaceUnaryOperator(type);
            break;
        case DECLARATION:
            if(!_instruction.isEmpty() && _instruction.getLastTokenType() == COLON) {
                _allowedTokens = {EQUAL_OPERATOR, OPEN_SQ_PARENTESIS, eofTokenType};
                break;
            }
            _allowedTokens = {COMMA, COLON};
            break;
        case FOR_STRUCTURE:
            if(_instruction.isEmpty()) {
                _allowedTokens = {};
                break;
            }
            if(_instruction.getLastTokenType() == K_VAR) {
                _allowedTokens = {COLON};
                break;
            }
            _allowedTokens = {EQUAL_OPERATOR};
            break;
        case DEFINITION:
            _allowedTokens = {OPEN_PARENTESIS};
            break;
        case PARAMETER_DEFINITION:
            if(_instruction.getLastTokenType() == COLON) {
                _allowedTokens = {COMMA, OPEN_SQ_PARENTESIS, CLOSE_PARENTESIS};
                break;
            }
            _allowedTokens = {COLON};
            break;
        case RETURN_DEFINITION:
            _allowedTokens = {COMMA, OPEN_SQ_PARENTESIS, OPEN_CUR_PARENTESIS};
            break;
        default:
            _allowedTokens = {};
            break;
        }
    }

    void InstructionFactory::checkValue(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.isExpression()) {
            _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
            if(type == STRING) { _allowedTokens.insert(_allowedTokens.end(), {DOT_OPERATOR, OPEN_SQ_PARENTESIS}); }
            emplaceExpressionTokens();
            return;
        }
        _allowedTokens = {};
    }

    void InstructionFactory::checkOperator(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.isExpression()) {
            _allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, OPEN_PARENTESIS};
            if(type == OPERATOR) { _allowedTokens.emplace_back(MINUS_OPERATOR); }
            return;
        }
        _allowedTokens = {};
    }

    void InstructionFactory::checkEqualOperator() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _allowedTokens = _expressionStartTokens;
        if(_instruction.lastTypeIs(OPERATION)) {
            _instruction.setLastType(ASSIGNATION);
            return;
        }
        if(_instruction.lastTypeIs(DECLARATION)) {
            _instruction.setLastType(INITIALIZATION);
            return;
        }
        if(_instruction.lastTypeIs(FOR_STRUCTURE)) {
            _instruction.addType(FOR_INITIALIZATION);
            return;
        }
        _allowedTokens = {};
    }

    void InstructionFactory::checkBooleanLogicalOperator(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.isExpression()) {
            _allowedTokens = _expressionStartTokens;
            if(type != NOT_OPERATOR) {
                _allowedTokens.emplace_back(NOT_OPERATOR);
                _instruction.setLastBooleanOperator(type == BOOLEAN_OPERATOR);
            }
            return;
        }
        _allowedTokens = {};
    }

    void InstructionFactory::checkComma() noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.lastTypeIsAny_of({OPERATION, DECLARATION, PARAMETER_DEFINITION, RETURN_DEFINITION})) {
            _allowedTokens = {IDENTIFIER};
            return;
        }
        if(_instruction.lastTypeIs(FOR_INITIALIZATION)) {
            _instruction.setLastType(FOR_CONDITION);
        } else if(_instruction.lastTypeIs(FOR_CONDITION)) {
            _instruction.setLastType(FOR_STEP);
        }
        _instruction.setLastBooleanOperator(false);
        _allowedTokens = _expressionStartTokens;
    }

    void InstructionFactory::checkOpenParentesis(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.addBooleanOperator();
        _allowedTokens = _expressionStartTokens;
        if(type == OPEN_PARENTESIS) {
            _allowedTokens.emplace_back(CLOSE_PARENTESIS);
            if(_instruction.lastTypeIs(DEFINITION)) {
                _instruction.addType(PARAMETER_DEFINITION);
                _allowedTokens = {IDENTIFIER, CLOSE_PARENTESIS};
                return;
            }
            if(_instruction.isLastTokenTypeAny_of({IDENTIFIER, CLOSE_SQ_PARENTESIS})) {
                _instruction.addType(PARAMETER_EXPRESSION);
                return;
            }
            _instruction.addType(EXPRESSION);
            return;
        }
        if(_instruction.lastTypeIs(DECLARATION) || _instruction.lastTypeIs(PARAMETER_DEFINITION) ||
           _instruction.lastTypeIs(RETURN_DEFINITION)) {
            _allowedTokens.emplace_back(CLOSE_SQ_PARENTESIS);
        }
        _instruction.addType(SQUARE_EXPRESSION);
    }

    void InstructionFactory::checkClosedParentesis(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.removeType();
        _instruction.removeBooleanOperator();
        if(_instruction.isExpression()) {
            _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, DOT_OPERATOR, OPEN_SQ_PARENTESIS};
            if(type == CLOSE_SQ_PARENTESIS) { _allowedTokens.insert(_allowedTokens.end(), {UNARY_OPERATOR, OPEN_PARENTESIS}); }
            emplaceExpressionTokens();
            return;
        }
        switch(_instruction.getLastType()) {
        case OPERATION:
            _allowedTokens = {DOT_OPERATOR, OPEN_SQ_PARENTESIS};
            if(type == CLOSE_PARENTESIS) {
                _allowedTokens.emplace_back(eofTokenType);
                break;
            }
            _allowedTokens.insert(_allowedTokens.end(), {EQUAL_OPERATOR, OPERATION_EQUAL, UNARY_OPERATOR, COMMA, OPEN_PARENTESIS});
            break;
        case DECLARATION:
            _allowedTokens = {EQUAL_OPERATOR, eofTokenType};
            if(type == CLOSE_SQ_PARENTESIS) { _allowedTokens.emplace_back(OPEN_SQ_PARENTESIS); }
            break;
        case STRUCTURE:
            [[fallthrough]];
        case ELSE:
            _allowedTokens = {OPEN_CUR_PARENTESIS};
            break;
        case DEFINITION:
            _instruction.setLastType(RETURN_DEFINITION);
            _allowedTokens = {COLON, OPEN_CUR_PARENTESIS};
            break;
        case PARAMETER_DEFINITION:
            _allowedTokens = {COMMA, OPEN_SQ_PARENTESIS, CLOSE_PARENTESIS};
            break;
        case RETURN_DEFINITION:
            _allowedTokens = {COMMA, OPEN_SQ_PARENTESIS, OPEN_CUR_PARENTESIS};
            break;
        default:
            _allowedTokens = {};
            break;
        }
    }

    void InstructionFactory::checkOpenCurParentesis() noexcept {
        using enum InstructionType;
        using enum TokenType;
        if(_instruction.isLastTokenTypeAny_of({EQUAL_OPERATOR, COMMA, OPEN_PARENTESIS, OPEN_CUR_PARENTESIS, K_RETURN})) {
            _instruction.addType(ARRAY_INIZIALIZATION);
            _allowedTokens.emplace_back(CLOSE_CUR_PARENTESIS);
            return;
        }
        if(_instruction.lastTypeIs(BLANK)) { _instruction.setLastType(OPEN_SCOPE); }
        _allowedTokens = {eofTokenType, CLOSE_CUR_PARENTESIS};
    }

    void InstructionFactory::checkClosedCurParentesis() noexcept {
        using enum InstructionType;
        using enum TokenType;
        if(_instruction.lastTypeIs(ARRAY_INIZIALIZATION)) {
            _instruction.removeType();
            if(_instruction.lastTypeIs(ARRAY_INIZIALIZATION)) {
                _allowedTokens = {COMMA, CLOSE_CUR_PARENTESIS};
                return;
            }
            emplaceExpressionTokens();
            return;
        }
        if(_instruction.lastTypeIs(BLANK)) { _instruction.setLastType(CLOSE_SCOPE); }
        _allowedTokens = {K_ELSE, eofTokenType};
    }

    void InstructionFactory::checkKMain() noexcept {
        using enum InstructionType;
        using enum TokenType;
        _instruction.setLastType(MAIN);
        _allowedTokens = {OPEN_CUR_PARENTESIS};
    }

    void InstructionFactory::checkKVar() noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.lastTypeIs(BLANK)) { _instruction.setLastType(DECLARATION); }
        _allowedTokens = {IDENTIFIER};
    }

    void InstructionFactory::checkKStructure() noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(_instruction.lastTypeIs(BLANK)) { _instruction.setLastType(STRUCTURE); }
        _allowedTokens = {OPEN_PARENTESIS};
    }

    void InstructionFactory::checkKElse() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.setLastType(ELSE);
        _allowedTokens = {K_IF, OPEN_CUR_PARENTESIS};
    }

    void InstructionFactory::checkKFor() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.setLastType(FOR_STRUCTURE);
        _allowedTokens = {K_VAR, IDENTIFIER};
    }

    void InstructionFactory::checkKFun() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.setLastType(DEFINITION);
        _allowedTokens = {IDENTIFIER};
    }

    void InstructionFactory::checkKReturn() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _instruction.setLastType(RETURN_EXPRESSION);
        _allowedTokens = _expressionStartTokens;
        _allowedTokens.emplace_back(eofTokenType);
    }

     bool InstructionFactory::emplaceTokenType(const InstructionType &type, const TokenType token) noexcept {
        if(_instruction.lastTypeIs(type)) [[likely]] {
            _allowedTokens.emplace_back(token);
            return true;
        }
        return false;
    }

    void InstructionFactory::emplaceExpressionTokens() noexcept {
        using enum InstructionType;
        using enum TokenType;
        emplaceBooleanOperator();
        if(emplaceTokenType(SQUARE_EXPRESSION, CLOSE_SQ_PARENTESIS)) { return; }
        if(emplaceTokenType(EXPRESSION, CLOSE_PARENTESIS)) { return; }
        if(_instruction.lastTypeIs(PARAMETER_EXPRESSION)) {
            _allowedTokens.insert(_allowedTokens.end(), {CLOSE_PARENTESIS, COMMA});
            return;
        }
        if(_instruction.lastTypeIs(ARRAY_INIZIALIZATION)) {
            _allowedTokens.insert(_allowedTokens.end(), {CLOSE_CUR_PARENTESIS, COMMA});
            return;
        }
        if(emplaceForTokens()) { return; }
        emplaceCommaEoft();
    }

    inline void InstructionFactory::emplaceCommaEoft() noexcept {
        _allowedTokens.insert(_allowedTokens.end(), {TokenType::COMMA, eofTokenType});
    }

    inline void InstructionFactory::emplaceBooleanOperator() noexcept {
        if(!_instruction.getLastBooleanOperator()) { _allowedTokens.emplace_back(TokenType::BOOLEAN_OPERATOR); }
    }

    inline bool InstructionFactory::emplaceForTokens() noexcept {
        if(_instruction.isForExpression()) [[likely]] {
            _allowedTokens.emplace_back(TokenType::OPEN_CUR_PARENTESIS);
            if(!_instruction.lastTypeIs(InstructionType::FOR_STEP)) { _allowedTokens.emplace_back(TokenType::COMMA); }
            return true;
        }
        return false;
    }

    inline void InstructionFactory::emplaceUnaryOperator(const TokenType &type) noexcept {
        using enum vnd::TokenType;
        if(type == IDENTIFIER) { _allowedTokens.insert(_allowedTokens.end(), {DOT_OPERATOR, OPEN_PARENTESIS, OPEN_SQ_PARENTESIS}); }
        if(type != UNARY_OPERATOR) { _allowedTokens.emplace_back(UNARY_OPERATOR); }
    }
    // NOLINTEND(*-include-cleaner)
}  // namespace vnd
