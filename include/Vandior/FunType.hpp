#pragma once

#include "headers.hpp"

namespace vnd {

    class FunType {
    public:
        [[nodiscard]] static FunType create(const std::string &returnType, const std::vector<std::string> &params, const bool constructor = false) noexcept;
        [[nodiscard]] std::string getReturnType() const noexcept;
        [[nodiscard]] std::vector<std::string> getParams() const noexcept;
        [[nodiscard]] bool isContructor() const noexcept;
    private:
        FunType(const std::string &returnType, const std::vector<std::string> &params, const bool constructor) noexcept;
        std::string _returnType;
        std::vector<std::string> _params;
        bool _constructor;
    };

}  // namespace vnd
