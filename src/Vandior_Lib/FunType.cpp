#include "Vandior/FunType.hpp"

namespace vnd {
    // NOLINTNEXTLINE(*-include-cleaner)
    FunType FunType::create(const std::string &returnType, const std::vector<std::string> &params,
                            const std::vector<std::pair<std::string, std::string>> &typeGeneric,
                            const std::vector<std::pair<std::string, std::string>> &funcGeneric,
                            const bool constructor) noexcept {
        return {returnType, params, typeGeneric, funcGeneric, constructor};
    }

    FunType FunType::createEmpty() noexcept { return {"", {}, {}, {}, false}; }

    std::string FunType::getReturnType() const noexcept { return _returnType; } // NOLINT(*-include-cleaner)

    std::vector<std::string> FunType::getParams() const noexcept { return _params; } // NOLINT(*-include-cleaner)

    std::vector<std::pair<std::string, std::string>> FunType::getTypeGeneric() const noexcept { return _typeGeneric; }

    std::vector<std::pair<std::string, std::string>> FunType::getFuncGeneric() const noexcept { return _funcGeneric; }

    bool FunType::isConstructor() const noexcept { return _constructor; }

}  // namespace vnd
