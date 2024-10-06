// NOLINTBEGIN(*-missing-std-forward, *-qualified-auto, *-implicit-bool-conversion, *-identifier-length)
#pragma once

#include "disableWarn.hpp"

DISABLE_WARNINGS_PUSH(4521 26429 26481)

namespace vnd {
    /////////////////////////////
    // remove_rvalue_reference

    /**
     * @brief Namespace containing utilities related to removing rvalue references.
     */
    namespace no_adl {
        /**
         * @brief Template struct for removing rvalue reference.
         * @tparam T Type from which rvalue reference needs to be removed.
         */
        template <typename T> struct remove_rvalue_reference {
            using type = T;  ///< Resulting type after removing rvalue reference.
        };

        /**
         * @brief Template specialization for removing rvalue reference from a type.
         * @tparam T Type with rvalue reference.
         */
        template <typename T> struct remove_rvalue_reference<T &&> {
            using type = T;  ///< Resulting type after removing rvalue reference.
        };
    }  // namespace no_adl

    using no_adl::remove_rvalue_reference;  ///< Alias for easier access to remove_rvalue_reference.

    /**
     * @brief Alias template for removing rvalue reference.
     * @tparam T Type from which rvalue reference needs to be removed.
     */
    template <typename T> using remove_rvalue_reference_t = typename vnd::remove_rvalue_reference<T>::type;

    /**
     * @brief Checks if the provided expression is a valid identifier expression.
     *
     * An identifier expression is considered valid if it consists of alphanumeric characters,
     * underscores, and colons only.
     *
     * @param strExpr The expression to be checked.
     * @return true if the expression is a valid identifier expression, false otherwise.
     */
    constexpr bool is_id_expression(char const *const strExpr) noexcept {
        for(auto it = strExpr; *it; ++it) {
            if(!('A' <= *it && *it <= 'Z') && !('a' <= *it && *it <= 'z') && !('0' <= *it && *it <= '9') && *it != '_' && *it != ':') {
                return false;
            }
        }
        return true;
    }
}  // namespace vnd

#define vnd_is_id_expression(...) vnd::is_id_expression(#__VA_ARGS__)

/////////////////////////////////////////////
// safer variants of std::move

namespace vnd::move_detail {
    /**
     * @brief Moves the given object into a new context, allowing efficient transfer of its resources.
     *
     * This function is similar to std::move, but includes additional checks and assertions.
     *
     * @tparam Decltype The type of the destination object. Use `auto` to deduce the type.
     * @tparam bIsIdExpression A boolean indicating whether the provided object is an identifier expression.
     * @param obj The object to be moved.
     * @return decltype(auto) Returns an rvalue reference to the moved object.
     * @note This function should be used to perform move operations in contexts where strict conditions need to be enforced.
     * @remark This function ensures that the object being moved is not const and is not already an rvalue.
     * It also checks whether moving out of an lvalue reference is intentional and provides guidance for using the function correctly.
     * @exception If the object is const, a std::logic_error is thrown with the message "Cannot move out of const."
     * @exception If the object is already an rvalue, a std::logic_error is thrown with the message "Unnecessary move; already an rvalue."
     * @exception If an attempt is made to move out of an lvalue reference, a std::logic_error is thrown with the message
     * "Are you sure you want to move out of an lvalue reference? Then use vnd_move_always."
     * @exception If bIsIdExpression is false, a std::logic_error is thrown with guidance on using the function correctly.
     * @see vnd_move_always
     * @see std::move
     * @warning Improper use of this function may lead to undefined behavior or unintended consequences.
     * Always ensure that the object being moved is in a valid state after the move operation.
     */
    template <typename Decltype, bool bIsIdExpression> constexpr decltype(auto) move(auto &&obj) noexcept {
        static_assert(!std::is_const<std::remove_reference_t<decltype(obj)>>::value, "Cannot move out of const.");
        static_assert(!std::is_rvalue_reference<decltype(obj)>::value, "Unnecessary move; already an rvalue.");

        static_assert(!std::is_lvalue_reference<Decltype>::value,
                      "Are you sure you want to move out of an lvalue reference? Then use vnd_move_always.");
        static_assert(bIsIdExpression, "Instead of `vnd_move(obj.member)`, use `vnd_move(obj).member`. Instead of `vnd_move(ptr->member)`, "
                                       "use `vnd_move_always(ptr->member)`.");

        return static_cast<std::remove_reference_t<Decltype> &&>(obj);
    }

    namespace no_adl {

        /**
         * @brief A utility for conditionally moving or forwarding an object.
         *
         * @tparam Decltype The type of the object to be moved or forwarded.
         * @tparam bIsIdExpression A boolean indicating whether the object is an id expression.
         */
        template <typename Decltype, bool bIsIdExpression> struct move_if_owned final {
            /**
             * @brief Static assertion to ensure correct usage.
             */
            static_assert(std::is_reference<Decltype>::value || bIsIdExpression,
                          "Instead of `vnd_move_if_owned(obj.member)`, use `vnd_move_if_owned(obj).member`.");

            /**
             * @brief Alias for the type after moving.
             */
            using type = Decltype &&;
        };

        /**
         * @brief A utility for conditionally forwarding a constant object.
         *
         * @tparam Decltype The type of the object to be forwarded.
         * @tparam bIsIdExpression A boolean indicating whether the object is an id expression.
         */
        template <typename Decltype, bool bIsIdExpression> struct const_forward final {
            static_assert(std::is_reference<Decltype>::value || bIsIdExpression,
                          "Instead of `vnd_const_forward(obj.member)`, use `vnd_const_forward(obj).member`.");

            /**
             * @brief Alias for the type after forwarding.
             */
            using type =
                std::conditional_t<std::is_lvalue_reference<Decltype>::value, Decltype, vnd::remove_rvalue_reference_t<Decltype> const &&>;
        };
    }  // namespace no_adl

}  // namespace vnd::move_detail

// T -> T&& (e.g. local variable)
// T& -> error (e.g. lvalue reference argument)
// T&& -> T&& (e.g. rvalue reference argument)
#define vnd_move(...)                                                                                                                      \
    (static_cast<decltype(vnd::move_detail::move<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>(__VA_ARGS__))>(__VA_ARGS__))

// T -> T&& (e.g. local variable)
// T& -> T& (e.g. lvalue reference argument)
// T&& -> T&& (e.g. rvalue reference argument)
#define vnd_move_if_owned(...)                                                                                                             \
    (static_cast<typename vnd::move_detail::no_adl::move_if_owned<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>::type>(        \
        __VA_ARGS__))

// MSVC sometimes forgets a const when decltyping.
#define vnd_move_if_owned_msvc_workaround(Type, ...)                                                                                       \
    (static_cast<typename vnd::move_detail::no_adl::move_if_owned<Type, vnd_is_id_expression(__VA_ARGS__)>::type>(__VA_ARGS__))

/**
 * @brief Moves the input value, asserting that it is not const.
 *
 * This function is similar to std::move but includes a static assertion to ensure that the input
 * value is not const. This is useful for enforcing move semantics in a context where constness
 * should be checked at compile time.
 *
 * @tparam T The type of the input value.
 * @param t The input value to be moved.
 * @return A rvalue reference to the input value.
 * @note The function is marked as constexpr and noexcept.
 * @throws static_assert if the input value is const.
 */
template <typename T> [[nodiscard]] constexpr std::remove_reference_t<T> &&vnd_move_always(T &&t) noexcept {
    static_assert(!std::is_const<std::remove_reference_t<T>>::value, "Cannot move out of const.");
    return static_cast<std::remove_reference_t<T> &&>(t);
}

/**
 * @brief Moves the input value, even if it is const.
 *
 * This function performs a move operation on the input value without checking for constness.
 * It allows const values to be moved, which is generally not recommended but can be useful
 * in specific scenarios where such behavior is required.
 *
 * @tparam T The type of the input value.
 * @param t The input value to be moved.
 * @return A rvalue reference to the input value.
 * @note The function is marked as constexpr and noexcept.
 */
template <typename T> [[nodiscard]] constexpr std::remove_reference_t<T> &&vnd_move_always_even_const(T &&t) noexcept {
    return static_cast<std::remove_reference_t<T> &&>(t);
}

/**
 * @brief Forwards the input value with const qualification.
 *
 * This macro provides a way to forward a value while preserving constness based on the
 * type of the input expression. It supports forwarding of const rvalue references,
 * lvalue references, and rvalue references.
 *
 * @param ... The input value to be forwarded.
 * @return The forwarded value with appropriate const qualification.
 * @note This macro relies on `vnd::move_detail::no_adl::const_forward` and `vnd_is_id_expression`.
 *
 */
#define vnd_const_forward(...)                                                                                                             \
    (static_cast<typename vnd::move_detail::no_adl::const_forward<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>::type>(        \
        __VA_ARGS__))
DISABLE_WARNINGS_POP()
// NOLINTEND(*-missing-std-forward, *-qualified-auto, *-implicit-bool-conversion, *-identifier-length)