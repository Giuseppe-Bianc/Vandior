#pragma once
#include <vector>
#include "Token.hpp"
#include "InstructionType.hpp"
#include "InstructionException.hpp"

namespace vnd {
    class Instruction {
	public:
        static Instruction create() noexcept;
        std::string test() { return "test"; }
        [[nodiscard]] std::vector<std::string> typeToString() const noexcept;
        void checkToken(const Token &token);
    private:
        std::vector<TokenType> _allowedTokens;
        std::vector<Token> _tokens;
        std::vector<InstructionType> _types;
        std::vector<bool> _booleanOperators;
        Instruction() noexcept;
        void checkIdentifier(const TokenType &type) noexcept;
        void checkValue(const TokenType &type) noexcept;
        void checkOperator(const TokenType &type) noexcept;

        [[nodiscard]] InstructionType getLastType() const noexcept;
        void setLastType(const InstructionType &type) noexcept;
        [[nodiscard]] inline bool lastTypeIs(const InstructionType &type) const noexcept;
        [[nodiscard]] TokenType getLastTokenType() const noexcept;
        [[nodiscard]] inline bool isEmpty() const noexcept;
        [[nodiscard]] inline bool getLastBooleanOperator() const noexcept;
        [[nodiscard]] bool isExpression() const noexcept;
        [[nodiscard]] bool isForExpression() const noexcept;
        [[nodiscard]] inline bool emplaceTokenType(const InstructionType &instruction, const TokenType token) noexcept;
        void emplaceExpressionTokens() noexcept;
        inline void emplaceCommaEoft() noexcept;
        inline void emplaceBooleanOperator() noexcept;
        inline bool emplaceForTokens() noexcept;
        inline void emplaceUnaryOperator(const TokenType &type) noexcept;
	};
}