#include "Vandior/FunType.hpp"

namespace vnd {
    FunType FunType::create(const std::string &returnType, const std::vector<std::string> &params,
                            const bool constructor) noexcept {
        return {returnType, params, constructor};
    }

    std::string FunType::getReturnType() const noexcept { return _returnType; }

    std::vector<std::string> FunType::getParams() const noexcept { return _params; }

    bool FunType::isContructor() const noexcept { return _constructor; }

}  // namespace vnd
