// NOLINTBEGIN(*-include-cleaner)
#include "Vandior/FunType.hpp"

namespace vnd {
    FunType FunType::create(const std::string &returnType, const std::vector<std::string> &params,
                            const std::vector<stringPair> &typeGeneric, const std::vector<stringPair> &funcGeneric,
                            const bool constructor) noexcept {
        return {returnType, params, typeGeneric, funcGeneric, constructor};
    }

    FunType FunType::createEmpty() noexcept { return {}; }

    std::string FunType::getReturnType() const noexcept { return _returnType; }

    std::vector<std::string> FunType::getParams() const noexcept { return _params; }

    std::vector<stringPair> FunType::getTypeGeneric() const noexcept { return _typeGeneric; }

    std::vector<stringPair> FunType::getFuncGeneric() const noexcept { return _funcGeneric; }

    bool FunType::isConstructor() const noexcept { return _constructor; }

}  // namespace vnd
   // NOLINTEND(*-include-cleaner)
