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

    void Scope::addVariable(const std::string_view identifier, const std::string_view type, const bool isConst) noexcept {
        if(isConst) {
            _consts[std::string{identifier}] = std::string{type};
            return;
        }
        _vars[std::string{identifier}] = type;
    }

    bool Scope::isMainScope() const noexcept { return _parent == nullptr; }

    bool Scope::checkType(const std::string_view type) const noexcept {
        if(_types.find(std::string{type}) != _types.end()) { return true; }
        if(_parent) { return _parent->checkType(type); }
        return false;
    }

    std::pair<bool, bool> Scope::checkVariable(const std::string_view identifier, const bool shadowing) const noexcept {
        if(_vars.find(std::string{identifier}) != _vars.end() || _consts.find(std::string{identifier}) != _consts.end()) {
            return {true, shadowing};
        }
        if(_parent) { return _parent->checkVariable(identifier, true); }
        return {false, false};
    }

    std::string_view Scope::getVariableType(const std::string_view identifier) const noexcept {
        if(_vars.find(std::string{identifier}) != _vars.end()) { return _vars.at(std::string{identifier}); }
        if(_consts.find(std::string{identifier}) != _consts.end()) { return _consts.at(std::string{identifier}); }
        if(_parent) { return _parent->getVariableType(identifier); }
        return "";
    }

}  // namespace vnd