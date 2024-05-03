
// think-cell public library
//
// Copyright (C) think-cell Software GmbH
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include "disableWarn.hpp"

DISABLE_WARNINGS_PUSH(4521)

namespace vnd {
	/////////////////////////////
	// remove_rvalue_reference

	namespace no_adl {
		template <typename T>
		struct remove_rvalue_reference {
			using type=T;
		};
		template <typename T>
		struct remove_rvalue_reference<T&&> {
			using type=T;
		};
	}

	using no_adl::remove_rvalue_reference;

	template<typename T>
	using remove_rvalue_reference_t=typename vnd::remove_rvalue_reference<T>::type;

	constexpr bool is_id_expression(char const* const strExpr) noexcept {
		for(auto it = strExpr; *it; ++it)
			if(!('A' <= *it && *it <= 'Z') && !('a' <= *it && *it <= 'z') && !('0' <= *it && *it <= '9') && *it != '_' && *it != ':')
				return false;
		return true;
	}
}

#define vnd_is_id_expression(...) vnd::is_id_expression(#__VA_ARGS__)

/////////////////////////////////////////////
// safer variants of std::move

namespace vnd::move_detail {
	template <typename Decltype, bool bIsIdExpression>
	constexpr decltype(auto) move(auto&& obj) noexcept {
		static_assert(!std::is_const<std::remove_reference_t<decltype(obj)>>::value, "Cannot move out of const.");
		static_assert(!std::is_rvalue_reference<decltype(obj)>::value, "Unnecessary move; already an rvalue.");

		static_assert(!std::is_lvalue_reference<Decltype>::value, "Are you sure you want to move out of an lvalue reference? Then use vnd_move_always.");
		static_assert(bIsIdExpression, "Instead of `vnd_move(obj.member)`, use `vnd_move(obj).member`. Instead of `vnd_move(ptr->member)`, use `vnd_move_always(ptr->member)`.");

		return static_cast<std::remove_reference_t<Decltype>&&>(obj);
	}

	namespace no_adl {
		template <typename Decltype, bool bIsIdExpression>
		struct move_if_owned final {
			static_assert(std::is_reference<Decltype>::value || bIsIdExpression, "Instead of `vnd_move_if_owned(obj.member)`, use `vnd_move_if_owned(obj).member`.");

			using type = Decltype&&;
		};

		template <typename Decltype, bool bIsIdExpression>
		struct const_forward final {
			static_assert(std::is_reference<Decltype>::value || bIsIdExpression, "Instead of `vnd_const_forward(obj.member)`, use `vnd_const_forward(obj).member`.");

			using type = std::conditional_t<std::is_lvalue_reference<Decltype>::value, Decltype, vnd::remove_rvalue_reference_t<Decltype> const&&>;
		};
	}
}

// T -> T&& (e.g. local variable)
// T& -> error (e.g. lvalue reference argument)
// T&& -> T&& (e.g. rvalue reference argument)
#define vnd_move(...) (static_cast<decltype(vnd::move_detail::move<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>(__VA_ARGS__))>(__VA_ARGS__))

// T -> T&& (e.g. local variable)
// T& -> T& (e.g. lvalue reference argument)
// T&& -> T&& (e.g. rvalue reference argument)
#define vnd_move_if_owned(...) (static_cast<typename vnd::move_detail::no_adl::move_if_owned<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>::type>(__VA_ARGS__))

// MSVC sometimes forgets a const when decltyping.
#define vnd_move_if_owned_msvc_workaround(Type, ...) (static_cast<typename vnd::move_detail::no_adl::move_if_owned<Type, vnd_is_id_expression(__VA_ARGS__)>::type>(__VA_ARGS__))

// Implementing vnd_move_always and vnd_move_always_even_const as functions avoids workaround for MSVC.
template<typename T>
[[nodiscard]] constexpr std::remove_reference_t<T>&& vnd_move_always(T&& t) noexcept { // same as std::move, but asserts on non-constness of argument
	static_assert(!std::is_const<std::remove_reference_t<T>>::value, "Cannot move out of const.");
	return static_cast<std::remove_reference_t<T>&&>(t);
}

template<typename T>
[[nodiscard]] constexpr std::remove_reference_t<T>&& vnd_move_always_even_const(T&& t) noexcept {
	return static_cast<std::remove_reference_t<T>&&>(t);
}

// T -> T const&&, T& -> T&, T&& -> T const&&
#define vnd_const_forward(...) (static_cast<typename vnd::move_detail::no_adl::const_forward<decltype(__VA_ARGS__), vnd_is_id_expression(__VA_ARGS__)>::type>(__VA_ARGS__))
DISABLE_WARNINGS_POP()