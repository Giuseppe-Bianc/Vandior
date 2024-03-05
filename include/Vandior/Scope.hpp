#pragma once
#include "Expression.hpp"
#include "FunType.hpp"
#include "ScopeType.hpp"
#include "headers.hpp"
#include <iostream>

namespace vnd {
    /**
     * A class use to represent a scope of a program.
     */
    class Scope {
    public:
        /**
         * @brief Creates a Scope instance using a shared pointer.
         * @param parent Shared pointer to the parent scope.
         * @param type ScopeType of the scope.
         * @return Shared pointer to an Scope instance.
         */
        [[nodiscard]] static std::shared_ptr<Scope> create(std::shared_ptr<Scope> parent, const ScopeType &type) noexcept;

        /**
         * @brief Creates the scope used as main scope in the program (not by the main function).
         * @return Shared pointer to the main scope.
         */
        [[nodiscard]] static std::shared_ptr<Scope> createMain() noexcept;

        /**
         * @brief Checks if a type is a numeric type.
         * @param type String representing the type to check.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] static bool isNumber(const std::string &type) noexcept;

        /**
         * @brief Checks if a type is a primitive type.
         * @param type String representing the type to check.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] static bool isPrimitive(const std::string &type) noexcept;

        /**
         * @brief Checks if a type is a vector type.
         * @param type String representing the type to check. If the checks succeeds, the string is modified into the indexed
         * type.
         * @return Bool indicating the result of the check.
         */
        [[nodiscard]] static bool checkVector(std::string &type) noexcept;

        /**
         * @brief Gets the C++ equivalent of a type.
         * @param type representing the type to transpile.
         * @return String representing the C++ transpiled type.
         */
        [[nodiscard]] static std::string getTypeValue(std::string &type) noexcept;

        /**
         * @brief Gets the parent scope.
         * @return Shared pointer to the parent scope.
         */
        [[nodiscard]] std::shared_ptr<Scope> getParent() const noexcept;

        /**
         * @brief Gets the scope type.
         * @return ScopeType of the scope.
         */
        [[nodiscard]] ScopeType getType() const noexcept;

        /**
         * @brief Removes the parent scope.
         */
        void removeParent() noexcept;

        /**
         * @brief Adds a type to the scope.
         * @param type String_view representing the type to add.
         * @param assignable Vector of types to which the type can be assigned.
         */
        void addType(const std::string_view &type, const std::vector<std::string> &assignable = {}) noexcept;

        /**
         * @brief Adds a compile time evaluated constant to the scope.
         * @param identifier String_view representing the identifier of the constant.
         * @param type String_view representing the type of the constant.
         * @param value String_view representing the value of the constant.
         */
        void addConstant(const std::string_view &identifier, const std::string_view &type, const std::string &value) noexcept;

        /**
         * @brief Adds a variable to the scope.
         * @param identifier String_view representing the identifier of the variable.
         * @param type String_view representing the type of the variable.
         * @param isVal Bool flag indicating if the variable is a constant.
         */
        void addVariable(const std::string_view &identifier, const std::string_view &type, const bool isVal) noexcept;

        /**
         * @brief Adds a function to the scope.
         * @param identifier String_view representing the identifier of the variable.
         * @param fun FunType containing the function informations.
         */
        void addFun(const std::string_view identifier, const FunType &fun) noexcept;

        /**
         * @brief Checks if it's the global scope.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool isGlobalScope() const noexcept;

        /**
         * @brief Checks if a type exixst in the scope.
         * @param type String_view representing the type to check.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool checkType(const std::string_view type) const noexcept;

        /**
         * @brief Checks if a variable exixst in the scope.
         * @param identifier String_view representing the identifier of the variable.
         * @param shadowing Bool flag indicates if the variable can be shadowed.
         * @return Pair containing the result of the check and if the variable is shadowed.
         */
        [[nodiscard]] std::pair<bool, bool> checkVariable(const std::string_view identifier,
                                                          const bool shadowing = false) const noexcept;

        /**
         * @brief Gets the type of a variable.
         * @param type String representing the type containing the property. Is the variable is not a property, the parameter is
         * empty.
         * @param identifier String_view representing the identifier of the variable.
         * @return String containing the type. Is the variable does not exist, the result is empty.
         */
        [[nodiscard]] std::string_view getVariableType(const std::string &type,
                                                       const std::string_view &identifier) const noexcept;

        /**
         * @brief Gets the type of a function.
         * @param type String representing the type containing the method. Is the function is not a method, the parameter is
         * empty.
         * @param identifier String_view representing the identifier of the function.
         * @param expressions Vector of parameter types.
         * @return Tuple containing the function informations: the type (empty if the function does not exist),
         * the flag indicating if the function is a constructor and the optional position of the first variadic parameter.
         */
        [[nodiscard]] std::tuple<std::string, bool, std::optional<size_t>> getFunType(
            const std::string &type, const std::string_view &identifier,
            const std::vector<Expression> &expressions) const noexcept;

        /**
         * @brief Gets the value of a compile time evaluated constant.
         * @param type String representing the type containing the property. Is the variable is not a property, the parameter is
         * empty.
         * @param identifier String_view representing the identifier of the function.
         * @return String containing value of the conatant. Is the variable does not exist or is not a compile time evaluated
         * constant, the result is empty.
         */
        [[nodiscard]] std::string getConstValue(const std::string &type, const std::string_view &identifier) const noexcept;

        /**
         * @brief Checks if a variable is a constant.
         * @param type String representing the type containing the property. Is the variable is not a property, the parameter is
         * empty.
         * @param identifier String_view representing the identifier of the function.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool isConstant(const std::string &type, const std::string_view &identifier) const noexcept;

        /**
         * @brief Checks if a a type can be assigned.
         * @param left String representing the type of the variable to be assigned.
         * @param right String representing the type of the expression to assign.
         * @return Bool containing the result of the check.
         */
        [[nodiscard]] bool canAssign(const std::string &left, const std::string &right) const noexcept;

    private:
        /**
         * @brief Constructor of a Scope.
         * @param parent Shared pointer to the parent scope.
         * @param type ScopeType of the scope.
         */
        explicit Scope(std::shared_ptr<Scope> parent, const ScopeType &type) noexcept;

        static std::vector<std::string> _numberTypes;     ///< Vector of numeric types
        static std::vector<std::string> _primitiveTypes;  ///< Vector of primitive types

        /**
         * @brief Gets key used by the maps.
         * @param type String representing the type.
         * @param identifier String_view representing the identifier.
         * @return String containing the key.
         */
        [[nodiscard]] static std::string getKey(const std::string &type, const std::string_view &identifier) noexcept;

        /**
         * @brief Gets the internal representation of an array or vector type.
         * @param type String representing the type.
         * @return String containing the internal representation. If the type is not an array or vector, the result is the same as
         * the parameter.
         */
        [[nodiscard]] static std::string getType(const std::string &type) noexcept;

        /**
         * @brief gets the type of a generic parameter.
         * @param param String containing the parameter type.
         * @param typeGeneric Vector of generic parameters of the type.
         * @return String containing the parameter type. If the parameter is not generic, the result is the same of param.
         */
        [[nodiscard]] static std::string getParamType(
            const std::string &param, const std::vector<std::pair<std::string, std::string>> &typeGeneric) noexcept;

        std::unordered_map<std::string, std::string> _vars;  ///< Map of variables identifiers and types.
        std::unordered_map<std::string, std::string> _vals;  ///< Map of constants identifiers and types.
        std::unordered_map<std::string, std::pair<std::string, std::string>>
            _consts;  ///< Map of compile time constants identifiers and types.
        std::unordered_map<std::string, std::vector<std::string>> _types;  ///< Map of types and assignabled types.
        std::unordered_map<std::string, std::vector<FunType>> _funs;       ///< Map of function identifiers and informations.
        std::shared_ptr<Scope> _parent;                                    ///< Shared pointer to the parent scope.
        ScopeType _type;

        /**
         * @brief gets the function corresponding to an identifier.
         * @param type String representing the type containing the method. Is the function is not a method, the parameter is
         * empty.
         * @param identifier String_view representing the identifier of the function.
         * @return Vector containing the found functions.
         */
        [[nodiscard]] std::vector<FunType> getFuns(const std::string &type, const std::string_view &identifier) const noexcept;

        /**
         * @brief Specializes the generic types of a funtion.
         * @param fun FunType to specialize.
         * @param typeSpecialized Vector of type specialized parameters.
         * @return Pair of the specialized FunType and flag that indicates if the specialization succeeds.
         */
        [[nodiscard]] std::pair<FunType, bool> specializeFun(const FunType &fun,
                                                             const std::vector<std::string> &typeSpecialized) const noexcept;
    };

}  // namespace vnd
