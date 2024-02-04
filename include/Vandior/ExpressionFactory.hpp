#include <vector>
#include <tuple>
#include <memory>
#include "Token.hpp"
#include "Scope.hpp"
#include "Expression.hpp"

namespace vnd {

    class ExpressionFactory {
    public:
        [[nodiscard]] static ExpressionFactory create(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                                                      std::shared_ptr<Scope> scope) noexcept;
        [[nodiscard]] std::string parse(const std::vector<TokenType> &endToken) noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] Expression getExpression() noexcept;
        [[nodiscard]] std::vector<Expression> getExpressions() noexcept;
    private:
        ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                          std::shared_ptr<Scope> scope) noexcept;
        using TupType = std::tuple<bool, bool, std::string>;
        [[nodiscard]] static std::string checkType(TupType &oldType, const std::string_view newType) noexcept;
        std::vector<Token>::iterator &_iterator;
        std::vector<Token>::iterator _end;
        std::shared_ptr<Scope> _scope;
        std::vector<std::string> _text;
        std::vector<Expression> _expressions;
        char _lastOperator;
        [[nodiscard]] std::string_view getTokenType(const Token &token) const noexcept;
        void emplaceToken() noexcept;
        [[nodiscard]] std::string writeToken() noexcept;
        [[nodiscard]] std::string handleFun(TupType &type) noexcept;
        [[nodiscard]] std::string handleInnerExpression(TupType &type) noexcept;
        [[nodiscard]] std::string handleToken(TupType &type) noexcept;
    };

}  // namespace vnd