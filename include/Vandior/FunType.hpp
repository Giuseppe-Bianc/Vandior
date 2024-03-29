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
         * @param returnType String containing return type. If the function has multiple return types, the are separated by a
         * whitespace.
         * @param params Vector containing the types of the parameters. If it's a variadic function, the last type ends with
         * "...".
         * @param typeGeneric Vector of generic parameters of the class.
         * @param funcGeneric Vector of generic parameters of the function.
         * @param constructor Bool flag that indicates if it's a constructor.
         * @return FunType instance.
         */
        [[nodiscard]] static FunType create(const std::string &returnType, const std::vector<std::string> &params,
                                            const std::vector<stringPair> &typeGeneric = {},
                                            const std::vector<stringPair> &funcGeneric = {}, const bool constructor = false) noexcept;

        /**
         * @brief Creates an empty FunType instance
         * @return an empty FunType instance.
         */
        [[nodiscard]] static FunType createEmpty() noexcept;

        /**
         * @brief Gets the return type of the function.
         * @return String containing the return type. If the function has multiple return types, the are separated by a
         * whitespace.
         */
        [[nodiscard]] std::string getReturnType() const noexcept;

        /**
         * @brief Gets the parameter types.
         * @return vector containing the types of the parameters. If it's a variadic function, the last type ends with "...".
         */
        [[nodiscard]] std::vector<std::string> getParams() const noexcept;

        /**
         * @brief Gets the generic parameters of the class.
         * @return vector containing the parameters.
         */
        [[nodiscard]] std::vector<stringPair> getTypeGeneric() const noexcept;

        /**
         * @brief Gets the generic parameters of the function.
         * @return vector containing the parameters.
         */
        [[nodiscard]] std::vector<stringPair> getFuncGeneric() const noexcept;

        /**
         * @brief Indicates if the function is a constructor,
         * @return the constructor flag.
         */
        [[nodiscard]] bool isConstructor() const noexcept;

    private:
        /**
         * @brief Default constructor of a FunType.
         */
        FunType() = default;
        /**
         * @brief Constructor of a FunType.
         * @param returnType String containing return type. If the function has multiple return types, the are separated by a
         * whitespace.
         * @param params Vector containing the types of the parameters. If it's a variadic function, the last type ends with
         * "...".
         * @param typeGeneric Vector of generic parameters of the class.
         * @param funcGeneric Vector of generic parameters of the function.
         * @param constructor Bool flag that indicates if it's a constructor.
         */
        FunType(const std::string &returnType, const std::vector<std::string> &params, const std::vector<stringPair> &typeGeneric,
                const std::vector<stringPair> &funcGeneric, const bool constructor) noexcept
          : _returnType(returnType), _params(params), _typeGeneric(typeGeneric), _funcGeneric(funcGeneric), _constructor(constructor) {}

        std::string _returnType;                                        ///< String of the return types separate by a whitespace.
        std::vector<std::string> _params;                               ///< Vector of type parameters.
        std::vector<std::pair<std::string, std::string>> _typeGeneric;  ///< Vector of generic types associated at the type.
        std::vector<std::pair<std::string, std::string>> _funcGeneric;  ///< Vector of generic types associated at the function.
        bool _constructor;                                              ///< Flag that indicates if the function is a constructor.
    };

}  // namespace vnd
