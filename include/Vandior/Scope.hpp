#pragma once
#include "Expression.hpp"
#include "FunType.hpp"
#include "headers.hpp"
#include <iostream>

namespace vnd {

    class Scope {
    public:
        [[nodiscard]] static std::shared_ptr<Scope> create(std::shared_ptr<Scope> parent) noexcept;
        [[nodiscard]] static std::shared_ptr<Scope> createMain() noexcept;
        [[nodiscard]] static bool isNumber(const std::string &type) noexcept;
        [[nodiscard]] static bool isPrimitive(const std::string &type) noexcept;
        [[nodiscard]] static bool checkVector(std::string &type) noexcept;
        [[nodiscard]] static std::string getTypeValue(std::string &type) noexcept;
        [[nodiscard]] std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
        void addType(const std::string_view &type, const std::vector<std::string> &assignable) noexcept;
        void addConstant(const std::string_view &identifier, const std::string_view &type, const std::string &value) noexcept;
        void addVariable(const std::string_view &identifier, const std::string_view &type, const bool isVal) noexcept;
        void addFun(const std::string_view identifier, const FunType &fun) noexcept;
        [[nodiscard]] bool isMainScope() const noexcept;
        [[nodiscard]] bool checkType(const std::string_view type) const noexcept;
        [[nodiscard]] std::pair<bool, bool> checkVariable(const std::string_view identifier,
                                                          const bool shadowing = false) const noexcept;
        [[nodiscard]] std::string_view getVariableType(const std::string &type,
                                                       const std::string_view &identifier) const noexcept;
        [[nodiscard]] std::tuple<std::string, bool, std::optional<size_t>> getFunType(
            const std::string &type, const std::string_view &identifier,
            const std::vector<Expression> &expressions) const noexcept;
        [[nodiscard]] std::string getConstValue(const std::string &type, const std::string_view &identifier) const noexcept;
        [[nodiscard]] bool isConstant(const std::string &type, const std::string_view &identifier) const noexcept;
        [[nodiscard]] bool canAssign(const std::string &left, const std::string &right) const noexcept;
        [[nodiscard]] std::string addTmp(std::string key, std::string &type) noexcept;
        [[nodiscard]] std::string getTmp(const std::string &tmp) const noexcept;
        void eachTmp(const std::function<void(const std::string &key)> &fun) const noexcept;
        void clearTmp() noexcept;

    private:
        Scope(std::shared_ptr<Scope> parent) noexcept;
        static std::vector<std::string> _numberTypes;
        static std::vector<std::string> _primitiveTypes;
        [[nodiscard]] static std::string getKey(const std::string &type, const std::string_view &identifier) noexcept;
        [[nodiscard]] static std::string getType(const std::string &type) noexcept;
        std::unordered_map<std::string, std::string> _vars;
        std::unordered_map<std::string, std::string> _vals;
        std::unordered_map<std::string, std::pair<std::string, std::string>> _consts;
        std::unordered_map<std::string, std::vector<std::string>> _types;
        std::unordered_map<std::string, std::vector<FunType>> _funs;
        std::unordered_map<std::string, std::string> _tmp;
        std::shared_ptr<Scope> _parent;
    };

}  // namespace vnd