#include "Vandior/FunType.hpp"

namespace vnd {
    // NOLINTNEXTLINE(*-include-cleaner)
    FunType FunType::create(const std::string &returnType, const std::vector<std::string> &params,
                            const bool constructor) noexcept {
        return {returnType, params, constructor};
    }

    std::string FunType::getReturnType() const noexcept { return _returnType; } // NOLINT(*-include-cleaner)

    std::vector<std::string> FunType::getParams() const noexcept { return _params; } // NOLINT(*-include-cleaner)

    bool FunType::isContructor() const noexcept { return _constructor; }

}  // namespace vnd
