#include "Vandior/Instruction.hpp"
#include <algorithm>

namespace vnd {
    // NOLINTBEGIN(*-include-cleaner)
    const std::vector<TokenType> Instruction::_expressionStartTokens = {TokenType::IDENTIFIER,      TokenType::INTEGER,
                                                                        TokenType::DOUBLE,          TokenType::CHAR,
                                                                        TokenType::STRING,          TokenType::BOOLEAN,
                                                                        TokenType::MINUS_OPERATOR,  TokenType::NOT_OPERATOR,
                                                                        TokenType::OPEN_PARENTESIS, TokenType::OPEN_CUR_PARENTESIS,
                                                                        TokenType::K_NULLPTR};

    Instruction::Instruction(const std::string_view filename) noexcept : _filename(filename) {
        _tokens.reserve(10);  // NOLINT(*-avoid-magic-numbers, *-magic-numbers)
    }

    Instruction Instruction::create(const std::string_view filename) noexcept { return Instruction{filename}; }

    std::vector<Token> Instruction::getTokens() const noexcept {
        if(_tokens.empty()) { return {{TokenType::UNKNOWN, "", {_filename, 0, 0}}}; }
        return _tokens;
    }

    InstructionType Instruction::getLastType() const noexcept {
        if(_types.empty()) { return InstructionType::BLANK; }
        return _types.back();
    }

    std::vector<std::string> Instruction::typeToString() const noexcept {
        std::vector<std::string> result;
        result.reserve(_types.size());
        std::ranges::transform(_types, std::back_inserter(result),
                               [](const InstructionType &instruction) { return FORMAT("{}", instruction); });
        return result;
    }

    std::string Instruction::toString() const noexcept {
        std::string result;
        if(_tokens.empty()) { return ""; }
        result += FORMAT("{}\t", _tokens.front().getLine());
        for(const auto &iter : _tokens) {
            switch(iter.getType()) {
            case TokenType::CHAR:
                result += FORMAT("'{}'", iter.getValue());
                break;
            case TokenType::STRING:
                result += FORMAT(R"("{}")", iter.getValue());
                break;
            default:
                result += FORMAT("{} ", iter.getValue());
                break;
            }
        }
        return result;
    }

    void Instruction::setLastType(const InstructionType &type) noexcept {
        if(_types.empty()) [[unlikely]] { return; }
        _types.back() = type;
    }

    void Instruction::addType(const InstructionType &type) noexcept { _types.emplace_back(type); }

    void Instruction::removeType() noexcept {
        if(_types.empty()) [[unlikely]] { return; }
        _types.pop_back();
    }

    TokenType Instruction::getLastTokenType() const noexcept {
        if(_tokens.empty()) [[unlikely]] { return TokenType::UNKNOWN; }
        return _tokens.back().getType();
    }

    bool Instruction::isLastTokenTypeAny_of(const std::initializer_list<TokenType> &tokenTypes) const noexcept {
        auto lastTokenType = getLastTokenType();
        // Assuming values is a collection of values to check against
        return std::ranges::any_of(tokenTypes, [&](const auto &value) { return lastTokenType == value; });
    }
    bool Instruction::lastTypeIs(const InstructionType &type) const noexcept { return getLastType() == type; }
    bool Instruction::lastTypeIsAny_of(const std::initializer_list<InstructionType> &types) const noexcept {
        auto last_type = getLastType();

        // Assuming values is a collection of values to check against
        return std::ranges::any_of(types, [&](const auto &value) { return last_type == value; });
    }
    bool Instruction::getLastBooleanOperator() const noexcept {
        if(_booleanOperators.empty()) [[unlikely]] { return false; }
        return _booleanOperators.back();
    }

    void Instruction::setLastBooleanOperator(const bool present) noexcept {
        if(_booleanOperators.empty()) [[unlikely]] { return; }
        _booleanOperators.back() = present;
    }

    void Instruction::addBooleanOperator() noexcept { _booleanOperators.emplace_back(false); }

    void Instruction::removeBooleanOperator() noexcept {
        if(_booleanOperators.empty()) [[unlikely]] { return; }
        _booleanOperators.pop_back();
    }

    bool Instruction::isEmpty() const noexcept { return _tokens.empty(); }

    bool Instruction::isExpression() const noexcept {
        using enum InstructionType;
        return lastTypeIsAny_of({PARAMETER_EXPRESSION, ASSIGNATION, INITIALIZATION, ARRAY_INIZIALIZATION, EXPRESSION, SQUARE_EXPRESSION,
                                 RETURN_EXPRESSION}) ||
               isForExpression();
    }

    bool Instruction::isForExpression() const noexcept {
        using enum InstructionType;
        return lastTypeIsAny_of({FOR_INITIALIZATION, FOR_CONDITION, FOR_STEP});
    }
    // NOLINTEND(*-include-cleaner)
}  // namespace vnd
