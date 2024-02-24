#pragma once

#include "headers.hpp"

namespace vnd {
    /**
     * @brief A class used as internal representation of a function.
     */
    class FunType {
    public:
        /**
         * @brief Creates a FunType instance.
         * @param returnType String containing return type. If the function has multiple return types, the are separated by a whitespace.
         * @param params Vector containing the types of the parameters. If it's a variadic function, the last type ends with "...".
         * @param constructor Bool flag that indicates if it's a constructor.
         * @return FunType instance.
         */
        [[nodiscard]] static FunType create(const std::string &returnType, const std::vector<std::string> &params,
                                            const bool constructor = false) noexcept;

        /**
         * @brief Gets the return type of the function.
         * @return String containing the return type. If the function has multiple return types, the are separated by a whitespace.
         */
        [[nodiscard]] std::string getReturnType() const noexcept;

        /**
         * @brief Gets the parameter types.
         * @return vector containing the types of the parameters. If it's a variadic function, the last type ends with "...".
         */
        [[nodiscard]] std::vector<std::string> getParams() const noexcept;

        /**
         * @brief Indicates if the function is a constructor,
         * @return the constructor flag.
         */
        [[nodiscard]] bool isConstructor() const noexcept;

    private:
        /**
         * @brief Constructor of a FunType.
         * @param returnType String containing return type. If the function has multiple return types, the are separated by a whitespace.
         * @param params Vector containing the types of the parameters. If it's a variadic function, the last type ends with "...".
         * @param constructor Bool flag that indicates if it's a constructor.
         */
        FunType(const std::string &returnType, const std::vector<std::string> &params, const bool constructor) noexcept
          : _returnType(returnType), _params(params), _constructor(constructor) {}

        std::string _returnType;            ///< String of the return types separate by a whitespace.
        std::vector<std::string> _params;   ///< Vector of type parameters.
        bool _constructor;                  ///< Flag that indicates if the function is a constructor.
    };

}  // namespace vnd
