#include "Vandior/Scope.hpp"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

namespace vnd {

    // NOLINTBEGIN
    std::vector<std::string> Scope::_numberTypes = {"int", "float"};
    std::vector<std::string> Scope::_primitiveTypes = {"int", "float", "double", "char", "bool", "string"};
    // NOLINTEND

    Scope::Scope(std::shared_ptr<Scope> parent) noexcept : _parent(std::move(parent)) {}

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> parent) noexcept {
        return std::make_shared<Scope>(Scope{std::move(parent)});
    }

    std::shared_ptr<Scope> Scope::createMain() noexcept {
        auto mainScope = std::make_shared<Scope>(Scope{nullptr});
        mainScope->addType("int", {});
        mainScope->addType("float", {});
        mainScope->addType("double", {});
        mainScope->addType("char", {});
        mainScope->addType("bool", {});
        mainScope->addType("string", {});
        mainScope->addType("Object", {});
        mainScope->addType("Derived", {"Object"});
        mainScope->addVariable("Object.a", "int", false);
        mainScope->addVariable("Object.test", "float", true);
        mainScope->addVariable("Object.s", "string", false);
        mainScope->addVariable("Derived._derivedProperty", "bool", false);
        mainScope->addVariable("Derived.obj", "Object", false);
        mainScope->addConstant("Object.c", "int", "2");
        mainScope->addConstant("Derived._derivedConst", "bool", "true");
        mainScope->addFun("_test", FunType::create("int", {}));
        mainScope->addFun("testPar", FunType::create("int", {"int", "int"}));
        mainScope->addFun("testPar", FunType::create("int", {"string"}));
        mainScope->addFun("createObject", FunType::create("Object", {}));
        mainScope->addFun("Object", FunType::create("Object", {}, true));
        mainScope->addFun("Derived", FunType::create("Derived", {}, true));
        mainScope->addFun("Derived", FunType::create("Derived", {"Object", "bool"}, true));
        mainScope->addFun("createDerived", FunType::create("Derived", {}));
        mainScope->addFun("[].size", FunType::create("int", {}));
        mainScope->addFun("string.size", FunType::create("int", {}));
        mainScope->addFun("Object.f", FunType::create("float", {"float"}));
        mainScope->addFun("Object.fs", FunType::create("string", {}));
        mainScope->addFun("Derived.derivedFun", FunType::create("bool", {"Object"}));
        mainScope->addFun("Derived.object", FunType::create("Object", {}));
        return mainScope;
    }

    bool Scope::isNumber(const std::string &type) noexcept {
        return std::ranges::find(Scope::_numberTypes, type) != Scope::_numberTypes.end();
    }

    bool Scope::isPrimitive(const std::string &type) noexcept {
        return std::ranges::find(Scope::_primitiveTypes, type) != Scope::_primitiveTypes.end();
    }

    bool Scope::checkVector(std::string &type) noexcept {
        if(type == "string") {
            type = "char";
            return true;
        }
        if(type.back() != ']') { return false; }
        while(type.back() != '[') { type.pop_back(); }
        type.pop_back();
        return true;
    }

    std::string Scope::getKey(const std::string &type, const std::string_view &identifier) noexcept {
        if(type.empty()) { return std::string(identifier); }
        return FORMAT("{}.{}", type, identifier);
    }

    std::string Scope::getType(const std::string &type) noexcept {
        if(type.ends_with("]")) [[unlikely]] { return "[]"; }
        return type;
    }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return _parent; }

    void Scope::removeParent() noexcept { _parent = nullptr; }

    void Scope::addType(const std::string_view &type, const std::vector<std::string> &assignable) noexcept {
        _types[std::string(type)] = assignable;
    }

    void Scope::addConstant(const std::string_view &identifier, const std::string_view &type, const std::string &value) noexcept {
        _consts[std::string{identifier}] = std::make_pair(type, value);
    }

    void Scope::addVariable(const std::string_view &identifier, const std::string_view &type, const bool isVal) noexcept {
        if(isVal) { _vals[std::string{identifier}] = type; }
        _vars[std::string{identifier}] = type;
    }

    void Scope::addFun(const std::string_view identifier, const FunType &fun) noexcept {
        auto key = std::string{identifier};
        if(!_funs.contains(key)) { _funs.try_emplace(key); }
        _funs[key].emplace_back(fun);
    }

    bool Scope::isMainScope() const noexcept { return _parent == nullptr; }

    bool Scope::checkType(const std::string_view type) const noexcept {  // NOLINT(*-no-recursion)
        if(_types.contains(std::string{type})) { return true; }
        if(_parent) { return _parent->checkType(type); }
        return false;
    }

    // NOLINTNEXTLINE
    std::pair<bool, bool> Scope::checkVariable(const std::string_view identifier, const bool shadowing) const noexcept {
        if(_vars.contains(std::string{identifier}) || _vals.contains(std::string{identifier}) ||
           _consts.contains(std::string{identifier})) {
            return {true, shadowing};
        }
        if(_parent) { return _parent->checkVariable(identifier, true); }
        return {false, false};
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::string_view Scope::getVariableType(const std::string &type, const std::string_view &identifier) const noexcept {
        auto key = Scope::getKey(type, identifier);
        if(_vars.contains(key)) { return _vars.at(key); }
        if(_vals.contains(key)) { return _vals.at(key); }
        if(_consts.contains(key)) { return _consts.at(key).first; }
        if(_types.contains(type)) {
            for(std::string i : _types.at(type)) {
                std::string_view result = getVariableType(i, identifier);
                if(!result.empty()) { return result; }
            }
        }
        if(_parent) { return _parent->getVariableType(type, identifier); }
        return "";
    }

    // NOLINTNEXTLINE(*-no-recursion)
    std::pair<std::string, bool> Scope::getFunType(const std::string &type, const std::string_view &identifier,
                                                   const std::vector<Expression> &expressions) const noexcept {
        auto key = Scope::getKey(Scope::getType(type), identifier);
        bool found = false;
        if(_funs.contains(key)) {
            for(const auto &i : _funs.at(key)) {
                auto params = i.getParams();
                found = true;
                if(params.size() != expressions.size()) [[likely]] {
                    found = false;
                } else [[unlikely]] {
                    for(size_t par = 0; par != params.size(); par++) {
                        if(!canAssign(params.at(par), expressions.at(par).getType())) { found = false; }
                    }
                }
                if(found) { return {i.getReturnType(), i.isContructor()}; }
            }
        }
        if(_types.contains(type)) {
            for(std::string i : _types.at(type)) {
                auto [result, constructor] = getFunType(i, identifier, expressions);
                if(!result.empty()) { return {result, constructor}; }
            }
        }
        if(_parent) { return _parent->getFunType(type, identifier, expressions); }
        return {"", false};
    }

    std::string Scope::getConstValue(const std::string &type, const std::string_view &identifier) const noexcept {
        auto key = Scope::getKey(type, identifier);
        if(_consts.contains(key)) { return _consts.at(key).second; }
        if(_types.contains(type)) {
            for(std::string i : _types.at(type)) {
                std::string result = getConstValue(i, identifier);
                if(!result.empty()) { return result; }
            }
        }
        if(_parent) { return _parent->getConstValue(type, identifier); }
        return "";
    }

    bool Scope::isConstant(const std::string &type, const std::string_view &identifier) const noexcept {
        auto key = Scope::getKey(type, identifier);
        if(_consts.contains(key) || _vals.contains(key)) { return true; }
        if(_types.contains(type)) {
            for(std::string i : _types.at(type)) {
                bool result = isConstant(i, identifier);
                if(result) { return result; }
            }
        }
        if(_parent) { return _parent->isConstant(type, identifier); }
        return false;
    }

    bool Scope::canAssign(const std::string &left, const std::string &right) const noexcept {
        if((Scope::isNumber(left) && Scope::isNumber(right)) || left == right) { return true; }
        std::pair<std::string, std::string> types = {left, right};
        if((types.first.ends_with("[]") || types.second.ends_with("[]")) && Scope::checkVector(types.first) &&
           Scope::checkVector(types.second)) {
            return canAssign(types.first, types.second);
        }
        if(_types.contains(right)) {
            for(std::string i : _types.at(right)) {
                if(canAssign(left, i)) { return true; }
            }
        }
        if(_parent) { return _parent->canAssign(left, right); }
        return false;
    }

}  // namespace vnd

DISABLE_WARNINGS_POP()
