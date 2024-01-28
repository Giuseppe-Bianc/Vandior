#include <vector>
#include "Token.hpp"
#include "Expression.hpp"

namespace vnd {

    class ExpressionFactory {
    public:
        [[nodiscard]] static ExpressionFactory create(std::vector<Token>::iterator &iterator, 
                                               std::vector<Token>::iterator tokensEnd) noexcept;
        void parse(const TokenType &endToken) noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::string getExpression() noexcept;
    private:
        ExpressionFactory(std::vector<Token>::iterator &iterator, std::vector<Token>::iterator end) noexcept;
        std::vector<Token>::iterator &_iterator;
        std::vector<Token>::iterator _end;
        std::vector<Expression> _expressions;
    };

}  // namespace vnd