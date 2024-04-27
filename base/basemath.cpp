#pragma once
#include <numbers>

constexpr double _PI = std::numbers::pi_v<double>;
constexpr double _TAU = std::numbers::pi * 2;
constexpr double _E = std::numbers::e;
constexpr double _INF = std::numeric_limits<double>::infinity(); 

namespace vnd {
	template<typename T1, typename T2>
	std::common_type_t<T1, T2> mod(T1 a, T2 b) {
		using result_type = std::common_type_t<T1, T2>;
		if constexpr (std::is_same_v<result_type, i64>) {
			std::cout << "Int" << std::endl;
			return a % b;
		}
		return std::fmod(static_cast<result_type>(a), static_cast<result_type>(b));	
	}
	template<typename T1, typename T2>
	std::common_type_t<T1, T2> pow(T1 a, T2 b)  {
		return static_cast<std::common_type_t<T1, T2>>(std::pow(a, b));
	}
}