#include "Vandior/Scope.hpp"

namespace vnd {

    Scope::Scope() noexcept: _identifiers({}), _types({"int", "float", "double", "char", "bool"}), _parent(nullptr) {}

    std::shared_ptr<Scope> Scope::create() noexcept { return std::make_shared<Scope>(Scope()); }

    /*Scope::Scope(std::shared_ptr<Scope> _parent) noexcept : parent(_parent) {}

    std::shared_ptr<Scope> Scope::create() noexcept { return std::shared_ptr<Scope>(); }

    std::shared_ptr<Scope> Scope::create(std::shared_ptr<Scope> _parent) noexcept{ return std::shared_ptr<Scope>(_parent); }

    std::shared_ptr<Scope> Scope::getParent() const noexcept { return this->parent; }

    void Scope::removeParent() noexcept { this->parent = nullptr; }

    void Scope::checkVariable(const std::string &variable) { LINFO(this->identifiers[variable]); }

    bool Scope::checkType(const std::string &type) const { return this->types.at(type); }*/

}  // namespace vnd*/