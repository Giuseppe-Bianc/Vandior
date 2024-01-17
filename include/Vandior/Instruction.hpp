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
        void checkIdentifier() noexcept;

        [[nodiscard]] InstructionType lastType() const noexcept;
        [[nodiscard]] inline bool lastTypeIs(const InstructionType &type) const noexcept;
        [[nodiscard]] bool isExpression() const noexcept;
        [[nodiscard]] bool isForExpression() const noexcept;
	};
}