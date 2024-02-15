#include "Vandior/FunType.hpp"

namespace vnd {

	FunType::FunType(const std::string &returnType, const std::vector<std::string> &params) noexcept
      : _returnType(returnType), _params(params) {}

	FunType FunType::create(const std::string &returnType, const std::vector<std::string> &params) noexcept {
        return {returnType, params};
    }

	std::string FunType::getReturnType() const noexcept { return _returnType; }

	std::vector<std::string> FunType::getParams() const noexcept { return _params; }

}  // namespace vnd
