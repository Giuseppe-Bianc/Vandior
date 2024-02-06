#include "Vandior/Scope.hpp"

namespace vnd {

    // NOLINTNEXTLINE
    std::vector<std::string> Scope::_numberTypes = {"int", "float"};

    Scope::Scope(std::shared_ptr<Scope> parent) noexcept : _parent(std::move(parent)) {}

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> parent) noexcept {
        return std::make_shared<Scope>(Scope(std::move(parent)));
    }

    std::shared_ptr<Scope> Scope::createMain() noexcept {
        auto mainScope = std::make_shared<Scope>(Scope(nullptr));
        mainScope->addType("int");
        mainScope->addType("float");
        mainScope->addType("double");
        mainScope->addType("char");
        mainScope->addType("bool");
        mainScope->addType("string");
        mainScope->addType("Object");
        mainScope->addVariable("Object.a", "int", false);
        mainScope->addVariable("Object.s", "string", false);
        mainScope->addFun("test", make_FunType("int", {}));
        mainScope->addFun("testPar", make_FunType("int", {"int", "int"}));
        mainScope->addFun("testPar", make_FunType("int", {"string"}));
        mainScope->addFun("createObject", make_FunType("Object", {}));
        mainScope->addFun("string.size", make_FunType("int", {}));
        mainScope->addFun("Object.f", make_FunType("float", {"float"}));
        mainScope->addFun("Object.fs", make_FunType("string", {}));
        return mainScope;
    }

    bool Scope::isNumber(const std::string &type) noexcept {
        return std::ranges::find(Scope::_numberTypes, type) != Scope::_numberTypes.end();
    }

    bool Scope::canAssign(const std::string &left, const std::string &right) noexcept {
        return (Scope::isNumber(left) && Scope::isNumber(right)) || left == right;
    }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return _parent; }

    void Scope::removeParent() noexcept { _parent = nullptr; }

    void Scope::addType(const std::string_view type) noexcept { _types.emplace(type); }

    void Scope::addVariable(const std::string_view identifier, const std::string_view type, const bool isConst) noexcept {
        if(isConst) {
            _consts[std::string{identifier}] = std::string{type};
            return;
        }
        _vars[std::string{identifier}] = type;
    }

    void Scope::addFun(const std::string_view identifier, const FunType &fun) noexcept {
        auto key = std::string{identifier};
        if(_funs.find(key) == _funs.end()) { _funs[key] = {}; }
        _funs[key].emplace_back(fun);
    }

    bool Scope::isMainScope() const noexcept { return _parent == nullptr; }

    bool Scope::checkType(const std::string_view type) const noexcept {  // NOLINT(*-no-recursion)
        if(_types.find(std::string{type}) != _types.end()) { return true; }
        if(_parent) { return _parent->checkType(type); }
        return false;
    }

    // NOLINTNEXTLINE
    std::pair<bool, bool> Scope::checkVariable(const std::string_view identifier, const bool shadowing) const noexcept {
        if(_vars.find(std::string{identifier}) != _vars.end() || _consts.find(std::string{identifier}) != _consts.end()) {
            return {true, shadowing};
        }
        if(_parent) { return _parent->checkVariable(identifier, true); }
        return {false, false};
    }

    std::string_view Scope::getVariableType(const std::string &type, const std::string_view &identifier) const noexcept {  // NOLINT(*-no-recursion)
        std::string key = type + std::string(identifier);
        if(_vars.find(key) != _vars.end()) { return _vars.at(key); }
        if(_consts.find(key) != _consts.end()) { return _consts.at(key); }
        if(_parent) { return _parent->getVariableType(type, identifier); }
        return "";
    }

    std::string Scope::getFunType(const std::string &type, const std::string_view &identifier,
                                  const std::vector<Expression> &expressions) const noexcept {  // NOLINT(*-no-recursion)
        std::string key = type + std::string(identifier);
        bool found = false;
        if(_funs.find(key) != _funs.end()) {
            for(const FunType &fun : _funs.at(key)) {
                found = true;
                if(std::get<1>(fun).size() != expressions.size()) [[likely]] {
                    found = false;
                } else [[unlikely]] {
                    for(size_t par = 0; par != std::get<1>(fun).size(); par++) {
                        if(!Scope::canAssign(get<1>(fun).at(par), expressions.at(par).getType())) { found = false; }
                    }
                }
                if(found) { return std::string{std::get<0>(fun)}; }
            }
        }
        if(_parent) { return _parent->getFunType(type, identifier, expressions); }
        return "";
    }

}  // namespace vnd
