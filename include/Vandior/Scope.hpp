#pragma once
#include "Expression.hpp"
#include "headers.hpp"
#include <iostream>

namespace vnd {

    class Scope {
    public:
        using FunType = std::pair<std::string, std::vector<std::string>>;
        static FunType make_FunType(const std::string &first, const std::vector<std::string> &second) {
            return std::make_pair(first, second);
        }
        [[nodiscard]] static std::shared_ptr<Scope> create(std::shared_ptr<Scope> parent) noexcept;
        [[nodiscard]] static std::shared_ptr<Scope> createMain() noexcept;
        [[nodiscard]] static bool isNumber(const std::string &type) noexcept;
        [[nodiscard]] static bool canAssign(const std::string &left, const std::string &right) noexcept;
        [[nodiscard]] static bool checkVector(std::string &type) noexcept;
        [[nodiscard]] std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
        void addType(const std::string_view &type) noexcept;
        void addConstant(const std::string_view &identifier, const std::string_view &type, const std::string &value) noexcept;
        void addVariable(const std::string_view &identifier, const std::string_view &type) noexcept;
        void addFun(const std::string_view identifier, const FunType &fun) noexcept;
        [[nodiscard]] bool isMainScope() const noexcept;
        [[nodiscard]] bool checkType(const std::string_view type) const noexcept;
        [[nodiscard]] std::pair<bool, bool> checkVariable(const std::string_view identifier,
                                                          const bool shadowing = false) const noexcept;
        [[nodiscard]] std::string_view getVariableType(const std::string &type,
                                                       const std::string_view &identifier) const noexcept;
        [[nodiscard]] std::string getFunType(const std::string &type, const std::string_view &identifier,
                                             const std::vector<Expression> &expressions) const noexcept;
        [[nodiscard]] std::string getConstValue(const std::string &type, const std::string_view &identifier) const noexcept;
    private:
        Scope(std::shared_ptr<Scope> parent) noexcept;
        static std::string getType(const std::string &type) noexcept;
        static std::vector<std::string> _numberTypes;
        std::unordered_map<std::string, std::string> _vars;
        std::unordered_map<std::string, std::pair<std::string, std::string>> _consts;
        std::unordered_set<std::string> _types;
        std::unordered_map<std::string, std::vector<FunType>> _funs;
        std::shared_ptr<Scope> _parent;
    };

}  // namespace vnd