#include "Vandior/Instruction.hpp"
#include <algorithm>

namespace vnd {
    const std::vector<TokenType> Instruction::_expressionStartTokens = {
        TokenType::IDENTIFIER,      TokenType::INTEGER,           TokenType::DOUBLE,         TokenType::CHAR,
        TokenType::STRING,          TokenType::BOOLEAN,           TokenType::MINUS_OPERATOR, TokenType::NOT_OPERATOR,
        TokenType::OPEN_PARENTESIS, TokenType::OPEN_CUR_PARENTESIS
    };

    Instruction::Instruction() noexcept
      : _allowedTokens({TokenType::K_MAIN, TokenType::K_VAR, TokenType::K_STRUCTURE, TokenType::K_FOR, TokenType::K_FUN,
                        TokenType::K_RETURN, TokenType::IDENTIFIER, TokenType::OPEN_CUR_PARENTESIS,
                        TokenType::CLOSE_CUR_PARENTESIS, eofTokenType}),
        _types({InstructionType::BLANK}), _booleanOperators({false}) {
        _tokens.reserve({});
    }

    Instruction Instruction::create() noexcept { return {}; }

    std::vector<Token> Instruction::getTokens() const noexcept {
        if(_tokens.empty()) { return {{TokenType::UNKNOWN, "", 0, 0}}; }
        return _tokens;
    }

    InstructionType Instruction::getLastType() const noexcept {
        if(_types.empty()) { return InstructionType::BLANK; }
        return _types.back();
    }

    std::vector<std::string> Instruction::typeToString() const noexcept {
        std::vector<std::string> result;
        result.reserve(_types.size());
        std::transform(_types.begin(), _types.end(), std::back_inserter(result),
                       [](const InstructionType &instruction) { return FORMAT("{}", instruction); });
        return result;
    }

    std::string Instruction::toString() const noexcept {
        std::string result = "";
        if(_tokens.empty()) { return ""; }
        result += std::to_string(_tokens[0].getLine()) + "\t";
        for(const Token &i : _tokens) { result += std::string(i.getValue()) + " "; }
        return result;
    }

    void Instruction::checkToken(const Token &token) {
        if(std::ranges::find(_allowedTokens, token.getType()) == _allowedTokens.end()) { throw InstructionException(token); }
        switch(token.getType()) {
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
        case EQUAL_OPERATOR:
            [[fallthrough]];
        case OPERATION_EQUAL:
            checkEqualOperator();
            break;
        case BOOLEAN_OPERATOR:
        case NOT_OPERATOR:
            [[fallthrough]];
        case LOGICAL_OPERATOR:
            checkBooleanLogicalOperator(token.getType());
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
            checkOpenParentesis(token.getType());
            break;
        case CLOSE_PARENTESIS:
            [[fallthrough]];
        case CLOSE_SQ_PARENTESIS:
            checkClosedParentesis(token.getType());
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
        case K_STRUCTURE:
            checkKStructure();
            break;
        case K_FOR:
            checkKFor();
            break;
        case K_FUN:
            checkKFun();
            break;
        case K_RETURN:
            checkKReturn();
            break;
        default:
            _tokens = {};
            break;
        }
        _tokens.emplace_back(token);
    }

    bool Instruction::canTerminate() const noexcept {
        return std::ranges::find(_allowedTokens, eofTokenType) != _allowedTokens.end();
    }

    void Instruction::checkIdentifier(const TokenType &type) noexcept {
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
            _allowedTokens = {EQUAL_OPERATOR,  OPERATION_EQUAL,    DOT_OPERATOR, COMMA,
                              OPEN_PARENTESIS, OPEN_SQ_PARENTESIS, eofTokenType};
            emplaceUnaryOperator(type);
            break;
        case DECLARATION:
            if(!isEmpty() && getLastTokenType() == COLON) {
                _allowedTokens = {EQUAL_OPERATOR, OPEN_SQ_PARENTESIS, eofTokenType};
                break;
            }
            _allowedTokens = {COMMA, COLON};
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

    void Instruction::checkValue(const TokenType &type) noexcept {
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

    void Instruction::checkOperator(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(isExpression()) {
            _allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, OPEN_PARENTESIS};
            if(type == OPERATOR) { _allowedTokens.emplace_back(MINUS_OPERATOR); }
            return;
        }
        _allowedTokens = {};
    }

    void Instruction::checkEqualOperator() noexcept {
        using enum TokenType;
        using enum InstructionType;
        _allowedTokens = _expressionStartTokens;
        if(lastTypeIs(OPERATION)) {
            this->setLastType(ASSIGNATION);
            return;
        }
        if(lastTypeIs(DECLARATION)) {
            this->setLastType(INITIALIZATION);
            return;
        }
        if(lastTypeIs(FOR_STRUCTURE)) {
            this->addType(FOR_INITIALIZATION);
            return;
        }
        _allowedTokens = {};
    }

    void Instruction::checkBooleanLogicalOperator(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(isExpression()) {
            _allowedTokens = _expressionStartTokens;
            if(type != NOT_OPERATOR) {
                _allowedTokens.emplace_back(NOT_OPERATOR);
                setLastBooleanOperator(type == BOOLEAN_OPERATOR);
            }
            return;
        }
        _allowedTokens = {};
    }

    void Instruction::checkComma() noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(lastTypeIs(OPERATION) || lastTypeIs(DECLARATION) || lastTypeIs(PARAMETER_DEFINITION) ||
           lastTypeIs(RETURN_DEFINITION)) {
            _allowedTokens = {IDENTIFIER};
            return;
        }
        if(lastTypeIs(FOR_INITIALIZATION)) {
            setLastType(FOR_CONDITION);
        } else if(lastTypeIs(FOR_CONDITION)) {
            setLastType(FOR_STEP);
        }
        setLastBooleanOperator(false);
        _allowedTokens = _expressionStartTokens;
    }

    void Instruction::checkOpenParentesis(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        addBooleanOperator();
        _allowedTokens = _expressionStartTokens;
        if(type == OPEN_PARENTESIS) {
            _allowedTokens.emplace_back(CLOSE_PARENTESIS);
            if(lastTypeIs(DEFINITION)) {
                addType(PARAMETER_DEFINITION);
                _allowedTokens = {IDENTIFIER, CLOSE_PARENTESIS};
                return;
            }
            if(getLastTokenType() == IDENTIFIER || getLastTokenType() == CLOSE_SQ_PARENTESIS) {
                addType(PARAMETER_EXPRESSION);
                return;
            }
            addType(EXPRESSION);
            return;
        }
        if(lastTypeIs(DECLARATION)) { _allowedTokens.emplace_back(CLOSE_SQ_PARENTESIS); }
        addType(SQUARE_EXPRESSION);
    }

    void Instruction::checkClosedParentesis(const TokenType &type) noexcept {
        using enum TokenType;
        using enum InstructionType;
        removeType();
        removeBooleanOperator();
        if(isExpression()) {
            _allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, DOT_OPERATOR, OPEN_SQ_PARENTESIS};
            if(type == CLOSE_SQ_PARENTESIS) {
                _allowedTokens.emplace_back(UNARY_OPERATOR);
                _allowedTokens.emplace_back(OPEN_PARENTESIS);
            }
            emplaceExpressionTokens();
            return;
        }
        switch(getLastType()) {
        case OPERATION:
            _allowedTokens = {DOT_OPERATOR, OPEN_SQ_PARENTESIS};
            if(type == CLOSE_PARENTESIS) {
                _allowedTokens.emplace_back(eofTokenType);
                break;
            }
            _allowedTokens.emplace_back(EQUAL_OPERATOR);
            _allowedTokens.emplace_back(OPERATION_EQUAL);
            _allowedTokens.emplace_back(UNARY_OPERATOR);
            _allowedTokens.emplace_back(COMMA);
            _allowedTokens.emplace_back(OPEN_PARENTESIS);
            break;
        case DECLARATION:
            _allowedTokens = {EQUAL_OPERATOR, eofTokenType};
            if(type == CLOSE_SQ_PARENTESIS) { _allowedTokens.emplace_back(OPEN_SQ_PARENTESIS); }
            break;
        case STRUCTURE:
            _allowedTokens = {OPEN_CUR_PARENTESIS};
            break;
        case DEFINITION:
            setLastType(RETURN_DEFINITION);
            _allowedTokens = {COLON, OPEN_CUR_PARENTESIS};
            break;
        default:
            _allowedTokens = {};
            break;
        }
    }

    void Instruction::checkOpenCurParentesis() noexcept {
        using enum InstructionType;
        using enum TokenType;
        if(getLastTokenType() == EQUAL_OPERATOR || getLastTokenType() == COMMA || getLastTokenType() == OPEN_PARENTESIS ||
           getLastTokenType() == OPEN_CUR_PARENTESIS) {
            addType(ARRAY_INIZIALIZATION);
            _allowedTokens.emplace_back(CLOSE_CUR_PARENTESIS);
            return;
        }
        if(lastTypeIs(BLANK)) { setLastType(OPEN_SCOPE); }
        _allowedTokens = {eofTokenType, CLOSE_CUR_PARENTESIS};
    }

    void Instruction::checkClosedCurParentesis() noexcept {
        using enum InstructionType;
        using enum TokenType;
        if(lastTypeIs(ARRAY_INIZIALIZATION)) {
            removeType();
            if(lastTypeIs(ARRAY_INIZIALIZATION)) {
                _allowedTokens = {COMMA, CLOSE_CUR_PARENTESIS};
                return;
            }
            _allowedTokens = {DOT_OPERATOR, OPEN_SQ_PARENTESIS};
            emplaceExpressionTokens();
            return;
        }
        if(lastTypeIs(BLANK)) { setLastType(CLOSE_SCOPE); }
        _allowedTokens = {eofTokenType};
    }

    void Instruction::checkKMain() noexcept {
        using enum InstructionType;
        using enum TokenType;
        this->setLastType(MAIN);
        _allowedTokens = {OPEN_CUR_PARENTESIS};
    }

    void Instruction::checkKVar() noexcept {
        using enum TokenType;
        using enum InstructionType;
        if(lastTypeIs(BLANK)) { setLastType(DECLARATION); }
        _allowedTokens = {IDENTIFIER};
    }

    void Instruction::checkKStructure() noexcept {
        using enum TokenType;
        using enum InstructionType;
        setLastType(STRUCTURE);
        _allowedTokens = {OPEN_PARENTESIS};
    }

    void Instruction::checkKFor() noexcept {
        using enum TokenType;
        using enum InstructionType;
        setLastType(FOR_STRUCTURE);
        _allowedTokens = {K_VAR, IDENTIFIER};
    }

    void Instruction::checkKFun() noexcept {
        using enum TokenType;
        using enum InstructionType;
        setLastType(DEFINITION);
        _allowedTokens = {IDENTIFIER};
    }

    void Instruction::checkKReturn() noexcept {
        using enum TokenType;
        using enum InstructionType;
        setLastType(RETURN_EXPRESSION);
        _allowedTokens = _expressionStartTokens;
        _allowedTokens.emplace_back(eofTokenType);
    }

    void Instruction::setLastType(const InstructionType &type) noexcept {
        if(_types.empty()) { return; }
        _types.pop_back();
        _types.emplace_back(type);
    }

    void Instruction::addType(const InstructionType &type) noexcept { _types.emplace_back(type); }

    void Instruction::removeType() noexcept {
        if(_types.empty()) { return; }
        _types.pop_back();
    }

    TokenType Instruction::getLastTokenType() const noexcept {
        if(_tokens.empty()) { return TokenType::UNKNOWN; }
        return _tokens.back().getType();
    }

    inline bool Instruction::lastTypeIs(const InstructionType &type) const noexcept { return getLastType() == type; }

    [[nodiscard]] inline bool Instruction::getLastBooleanOperator() const noexcept {
        if(_booleanOperators.empty()) { return false; }
        return _booleanOperators.back();
    }

    void Instruction::setLastBooleanOperator(const bool present) noexcept {
        if(_booleanOperators.empty()) { return; }
        _booleanOperators.pop_back();
        _booleanOperators.emplace_back(present);
    }

    void Instruction::addBooleanOperator() noexcept { _booleanOperators.emplace_back(false); }

    void Instruction::removeBooleanOperator() noexcept {
        if(_booleanOperators.empty()) { return; }
        _booleanOperators.pop_back();
    }

    inline bool Instruction::isEmpty() const noexcept { return _tokens.empty(); }

    bool Instruction::isExpression() const noexcept {
        using enum InstructionType;
        return lastTypeIs(PARAMETER_EXPRESSION) || lastTypeIs(ASSIGNATION) || lastTypeIs(INITIALIZATION) ||
               lastTypeIs(ARRAY_INIZIALIZATION) || lastTypeIs(EXPRESSION) || lastTypeIs(SQUARE_EXPRESSION) ||
               lastTypeIs(RETURN_EXPRESSION) || isForExpression();
    }

    bool Instruction::isForExpression() const noexcept {
        using enum InstructionType;
        return lastTypeIs(FOR_INITIALIZATION) || lastTypeIs(FOR_CONDITION) || lastTypeIs(FOR_STEP);
    }

    inline bool Instruction::emplaceTokenType(const InstructionType &instruction, const TokenType token) noexcept {
        if(lastTypeIs(instruction)) {
            _allowedTokens.emplace_back(token);
            return true;
        }
        return false;
    }

    void Instruction::emplaceExpressionTokens() noexcept {
        using enum InstructionType;
        using enum TokenType;
        emplaceBooleanOperator();
        if(emplaceTokenType(SQUARE_EXPRESSION, CLOSE_SQ_PARENTESIS)) { return; }
        if(emplaceTokenType(EXPRESSION, CLOSE_PARENTESIS)) { return; }
        if(lastTypeIs(PARAMETER_EXPRESSION)) {
            _allowedTokens.emplace_back(CLOSE_PARENTESIS);
            _allowedTokens.emplace_back(COMMA);
            return;
        }
        if(lastTypeIs(ARRAY_INIZIALIZATION)) {
            _allowedTokens.emplace_back(CLOSE_CUR_PARENTESIS);
            _allowedTokens.emplace_back(COMMA);
            return;
        }
        if(emplaceForTokens()) { return; }
        emplaceCommaEoft();
    }

    inline void Instruction::emplaceCommaEoft() noexcept {
        _allowedTokens.emplace_back(TokenType::COMMA);
        _allowedTokens.emplace_back(eofTokenType);
    }

    inline void Instruction::emplaceBooleanOperator() noexcept {
        if(!getLastBooleanOperator()) { _allowedTokens.emplace_back(TokenType::BOOLEAN_OPERATOR); }
    }

    inline bool Instruction::emplaceForTokens() noexcept {
        if(isForExpression()) {
            _allowedTokens.emplace_back(TokenType::OPEN_CUR_PARENTESIS);
            if(!lastTypeIs(InstructionType::FOR_STEP)) { _allowedTokens.emplace_back(TokenType::COMMA); };
            return true;
        }
        return false;
    }

    inline void Instruction::emplaceUnaryOperator(const TokenType &type) noexcept {
        if(type != TokenType::UNARY_OPERATOR) { _allowedTokens.emplace_back(TokenType::UNARY_OPERATOR); }
    }

}  // namespace vnd
