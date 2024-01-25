#pragma once
#include <vector>
#include "Token.hpp"
#include "InstructionType.hpp"
#include "InstructionException.hpp"

namespace vnd {
    class Instruction {
	public:
        static Instruction create() noexcept;
        [[nodiscard]] InstructionType getType() const noexcept;
        [[nodiscard]] std::vector<std::string> typeToString() const noexcept;
        void checkToken(const Token &token);
        bool canTerminate() const noexcept;
        [[nodiscard]] InstructionType getLastType() const noexcept;
        [[nodiscard]] std::string toString() const noexcept;
    private:
        static const std::vector<TokenType> _expressionStartTokens;
        std::vector<TokenType> _allowedTokens;
        std::vector<Token> _tokens;
        std::vector<InstructionType> _types;
        std::vector<bool> _booleanOperators;
        Instruction() noexcept;
        void checkIdentifier(const TokenType &type) noexcept;
        void checkValue(const TokenType &type) noexcept;
        void checkOperator(const TokenType &type) noexcept;
        void checkEqualOperator() noexcept;
        void checkBooleanLogicalOperator(const TokenType &type) noexcept;
        void checkComma() noexcept;
        void checkOpenParentesis(const TokenType &type) noexcept;
        void checkClosedParentesis(const TokenType &type) noexcept;
        void checkOpenCurParentesis() noexcept;
        void checkClosedCurParentesis() noexcept;
        void checkKMain() noexcept;
        void checkKVar() noexcept;
        void checkKStructure() noexcept;
        void checkKFor() noexcept;
        void checkKFun() noexcept;
        void checkKReturn() noexcept;
        void setLastType(const InstructionType &type) noexcept;
        void addType(const InstructionType &type) noexcept;
        void removeType() noexcept;
        [[nodiscard]] inline bool lastTypeIs(const InstructionType &type) const noexcept;
        [[nodiscard]] inline bool getLastBooleanOperator() const noexcept;
        void setLastBooleanOperator(const bool present) noexcept; 
        void addBooleanOperator() noexcept;
        void removeBooleanOperator() noexcept;
        [[nodiscard]] TokenType getLastTokenType() const noexcept;
        [[nodiscard]] inline bool isEmpty() const noexcept;
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