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
        [[nodiscard]] std::string parse(const TokenType &endToken) noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::string getExpression() noexcept;
    private:
        ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end,
                          std::shared_ptr<Scope> scope) noexcept;
        static std::string checkType(std::tuple<bool, bool, std::string> &oldType, const std::string_view newType) noexcept;
        std::vector<Token>::iterator &_iterator;
        std::vector<Token>::iterator _end;
        std::shared_ptr<Scope> _scope;
        std::vector<Expression> _expressions;
        std::string_view getTokenType(const Token &token) const noexcept;
    };

}  // namespace vnd