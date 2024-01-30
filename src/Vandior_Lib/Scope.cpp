#include "Vandior/Scope.hpp"

namespace vnd {

    Scope::Scope(std::shared_ptr<Scope> parent) noexcept: _vars({}), _consts({}), _types(), _parent(parent) {}

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> parent) noexcept { return std::make_shared<Scope>(Scope(parent)); }

    std::shared_ptr<Scope> Scope::createMain() noexcept {
        std::shared_ptr<Scope> mainScope = std::make_shared<Scope>(Scope(nullptr));
        mainScope->addType("int");
        mainScope->addType("float");
        mainScope->addType("double");
        mainScope->addType("char");
        mainScope->addType("bool");
        mainScope->addType("string");
        return mainScope;
    }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return _parent; }

    void Scope::removeParent() noexcept { _parent = nullptr; }

    void Scope::addType(const std::string_view type) noexcept {
        _types.emplace(type);
    }

    void Scope::addVariable(const std::string_view identifer, const std::string_view type, const bool isConst) noexcept {
        if(isConst) {
            _consts[identifer] = type;
            return;
        }
        _vars[identifer] = type;
    }

    bool Scope::isMainScope() const noexcept { return _parent == nullptr; }

    bool Scope::checkType(const std::string_view type) const noexcept {
        if(_types.find(type) != _types.end()) { return true; }
        if(_parent) { return _parent->checkType(type); }
        return false;
    }

    std::pair<bool, bool> Scope::checkVariable(const std::string_view type, const bool shadowing) const noexcept {
        if(_vars.find(type) != _vars.end() || _consts.find(type) != _consts.end()) { return {true, shadowing}; }
        if(_parent) { return _parent->checkVariable(type, true); }
        return {false, false};
    }

}  // namespace vnd