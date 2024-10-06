//
// Created by gbian on 01/06/2024.
//

// NOLINTBEGIN(*-include-cleaner, *-macro-usage)
#pragma once
#include <bit>

/**
 * @defgroup BitTypeCastingMacros the Bit Type Casting Macros
 * @{
 */

/**
 * @brief Macro to cast a value to bool using std::bit_cast.
 * This macro is used to cast a value to a boolean type using the underlying bit representation.
 * @param x The value to be casted.
 * @return The value casted to bool.
 * Usage example: @code{.cpp} bool b = BC_BOOL(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_BOOL(x) std::bit_cast<bool>(x)

/**
 * @brief Macro to cast a value to std::byte using std::bit_cast.
 * This macro is used to cast a value to std::byte type.
 * @param x The value to be casted.
 * @return The value casted to std::byte.
 * Usage example: @code{.cpp} std::byte b = BC_B(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_B(x) std::bit_cast<std::byte>(x)

/**
 * @brief Macro to cast a value to std::intptr_t using std::bit_cast.
 * This macro is used to cast a value to std::intptr_t type, which is useful for holding a pointer.
 * @param x The value to be casted.
 * @return The value casted to std::intptr_t.
 * Usage example: @code{.cpp} std::intptr_t i = BC_IPTR(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_IPTR(x) std::bit_cast<std::intptr_t>(x)

/**
 * @brief Macro to cast a value to std::uintptr_t using std::bit_cast.
 * This macro is used to cast a value to std::uintptr_t type, which is an unsigned integer pointer type.
 * @param x The value to be casted.
 * @return The value casted to std::uintptr_t.
 * Usage example: @code{.cpp} std::uintptr_t ui = BC_UIPTR(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_UIPTR(x) std::bit_cast<std::uintptr_t>(x)

/**
 * @brief Macro to cast a value to std::int8_t using std::bit_cast.
 * This macro is used to cast a value to std::int8_t type, which is an 8-bit signed integer.
 * @param x The value to be casted.
 * @return The value casted to std::int8_t.
 * Usage example: @code{.cpp} std::int8_t i8 = BC_I8T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_I8T(x) std::bit_cast<std::int8_t>(x)

/**
 * @brief Macro to cast a value to std::int16_t using std::bit_cast.
 * This macro is used to cast a value to std::int16_t type, which is a 16-bit signed integer.
 * @param x The value to be casted.
 * @return The value casted to std::int16_t.
 * Usage example: @code{.cpp} std::int16_t i16 = BC_I16T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_I16T(x) std::bit_cast<std::int16_t>(x)

/**
 * @brief Macro to cast a value to std::int32_t using std::bit_cast.
 * This macro is used to cast a value to std::int32_t type, which is a 32-bit signed integer.
 * @param x The value to be casted.
 * @return The value casted to std::int32_t.
 * Usage example: @code{.cpp} std::int32_t i32 = BC_I32T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_I32T(x) std::bit_cast<std::int32_t>(x)

/**
 * @brief Macro to cast a value to std::int64_t using std::bit_cast.
 * This macro is used to cast a value to std::int64_t type, which is a 64-bit signed integer.
 * @param x The value to be casted.
 * @return The value casted to std::int64_t.
 * Usage example: @code{.cpp} std::int64_t i64 = BC_I64T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_I64T(x) std::bit_cast<std::int64_t>(x)

/**
 * @brief Macro to cast a value to std::uint8_t using std::bit_cast.
 * This macro is used to cast a value to std::uint8_t type, which is an 8-bit unsigned integer.
 * @param x The value to be casted.
 * @return The value casted to std::uint8_t.
 * Usage example: @code{.cpp} std::uint8_t ui8 = BC_UI8T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_UI8T(x) std::bit_cast<std::uint8_t>(x)

/**
 * @brief Macro to cast a value to std::uint16_t using std::bit_cast.
 * This macro is used to cast a value to std::uint16_t type, which is a 16-bit unsigned integer.
 * @param x The value to be casted.
 * @return The value casted to std::uint16_t.
 * Usage example: @code{.cpp} std::uint16_t ui16 = BC_UI16T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_UI16T(x) std::bit_cast<std::uint16_t>(x)

/**
 * @brief Macro to cast a value to std::uint32_t using std::bit_cast.
 * This macro is used to cast a value to std::uint32_t type, which is a 32-bit unsigned integer.
 * @param x The value to be casted.
 * @return The value casted to std::uint32_t.
 * Usage example: @code{.cpp} std::uint32_t ui32 = BC_UI32T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_UI32T(x) std::bit_cast<std::uint32_t>(x)

/**
 * @brief Macro to cast a value to std::uint64_t using std::bit_cast.
 * This macro is used to cast a value to std::uint64_t type, which is a 64-bit unsigned integer.
 * @param x The value to be casted.
 * @return The value casted to std::uint64_t.
 * Usage example: @code{.cpp} std::uint64_t ui64 = BC_UI64T(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_UI64T(x) std::bit_cast<std::uint64_t>(x)

/**
 * @brief Macro to cast a value to std::ptrdiff_t using std::bit_cast.
 * This macro is used to cast a value to std::ptrdiff_t type, which is an integer type capable of representing the difference between two
 * pointers.
 * @param x The value to be casted.
 * @return The value casted to std::ptrdiff_t.
 * Usage example: @code{.cpp} std::ptrdiff_t pDiff = BC_PTRDIFT(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_PTRDIFT(x) std::bit_cast<std::ptrdiff_t>(x)

/**
 * @brief Macro to cast a value to std::div_t using std::bit_cast.
 * This macro is used to cast a value to std::div_t type, which is a structure used to store the result of the div function, including both
 * the quotient and the remainder.
 * @param x The value to be casted.
 * @return The value casted to std::div_t.
 * Usage example: @code{.cpp} std::div_t divResult = BC_DIVT(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_DIVT(x) std::bit_cast<std::div_t>(x)

/**
 * @brief Macro to cast a value to std::ldiv_t using std::bit_cast.
 * This macro is used to cast a value to std::ldiv_t type, which is a structure used to store the result of the ldiv function, including
 * both the quotient and the remainder.
 * @param x The value to be casted.
 * @return The value casted to std::ldiv_t.
 * Usage example: @code{.cpp} std::ldiv_t ldivResult = BC_LDIVT(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_LDIVT(x) std::bit_cast<std::ldiv_t>(x)

/**
 * @brief Macro to cast a value to std::lldiv_t using std::bit_cast.
 * This macro is used to cast a value to std::lldiv_t type, which is a structure used to store the result of the lldiv function, including
 * both the quotient and the remainder.
 * @param x The value to be casted.
 * @return The value casted to std::lldiv_t.
 * Usage example: @code{.cpp} std::lldiv_t lldivResult = BC_LLDIVT(someValue); @endcode
 * @see std::bit_cast
 */
#define BC_LLDIVT(x) std::bit_cast<std::lldiv_t>(x)

/** @} */  // end of TypeCastingMacros group
// NOLINTEND(*-include-cleaner, *-macro-usage)
