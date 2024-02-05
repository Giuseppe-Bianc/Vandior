#pragma once
#include "InstructionException.hpp"
#include "InstructionType.hpp"
#include "Token.hpp"
#include <vector>

namespace vnd {
    class Instruction {
    public:
        [[nodiscard]] static Instruction create(const std::string_view filename) noexcept;
        [[nodiscard]] std::vector<Token> getTokens() const noexcept;
        [[nodiscard]] InstructionType getLastType() const noexcept;
        [[nodiscard]] std::vector<std::string> typeToString() const noexcept;
        void checkToken(const Token &token);
        [[nodiscard]] bool canTerminate() const noexcept;
        [[nodiscard]] std::string toString() const noexcept;

    private:
        Instruction(const std::string_view filename) noexcept;
        static const std::vector<TokenType> _expressionStartTokens;
        std::vector<TokenType> _allowedTokens;
        std::vector<Token> _tokens;
        std::vector<InstructionType> _types;
        std::vector<bool> _booleanOperators;
        std::string_view _filename;
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
}  // namespace vnd

template <> struct fmt::formatter<vnd::Instruction> : fmt::formatter<std::string_view> {  // NOLINT(*-include-cleaner)
    template <typename FormatContext> auto format(const vnd::Instruction &val, FormatContext &ctx) {
        return fmt::formatter<std::string_view>::format(val.toString(), ctx);
    }
};
