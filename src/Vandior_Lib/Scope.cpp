#include "Vandior/Scope.hpp"

namespace vnd {

    Scope::Scope(std::shared_ptr<Scope> parent) noexcept: _identifiers({}), _types(), _parent(parent) {}

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> parent) noexcept { return std::make_shared<Scope>(Scope(parent)); }

    std::shared_ptr<Scope> Scope::createMain() noexcept {
        std::shared_ptr<Scope> mainScope = std::make_shared<Scope>(Scope(nullptr));
        mainScope->addType("int");
        mainScope->addType("float");
        mainScope->addType("double");
        mainScope->addType("char");
        mainScope->addType("bool");
        return mainScope;
    }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return _parent; }

    void Scope::removeParent() noexcept { _parent = nullptr; }

    void Scope::addType(std::string_view type) noexcept {
        _types.emplace(type);
    }

    bool Scope::isMainScope() const noexcept { return _parent == nullptr; }
    /*void Scope::checkVariable(const std::string &variable) { LINFO(this->identifiers[variable]); }

    bool Scope::checkType(const std::string &type) const { return this->types.at(type); }*/

}  // namespace vnd