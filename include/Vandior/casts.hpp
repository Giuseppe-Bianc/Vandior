#pragma once
/**
 * @file
 * @brief Macros and utilities for type casting
 */
#include <bit>
/**
 * @defgroup TypeCastingMacros Type Casting Macros
 * @{
 */

/**
 * @brief Macro to cast to bool.
 * This macro is used to cast a value to bool type.
 * @param x The value to cast.
 * @return The casted value.
 * Usage example: @code{.cpp} bool b = C_BOOL(1); @endcode
 */
#define C_BOOL(x) static_cast<bool>(x)
#define NC_BOOL(x) gsl::narrow_cast<bool>(x)
#define BC_BOOL(x) std::bit_cast<bool>(x)
/**
 * @brief Macro to cast to std::byte.
 * This macro is used to cast a value to std::byte type.
 * @param x The value to cast.
 * @return The casted value.
 * Usage example: @code{.cpp} std::byte b = C_B(255); @endcode
 */
#define C_B(x) static_cast<std::byte>(x)
#define NC_B(x) gsl::narrow_cast<std::byte>(x)
#define BC_B(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::intptr_t.
 * This macro is used to cast a value to std::intptr_t type.
 * @param x The value to be casted.
 * @return The value casted to std::intptr_t.
 * Usage example: @code{.cpp} std::intptr_t ptr = C_IPTR(somePointer);@endcode
 */
#define C_IPTR(x) static_cast<std::intptr_t>(x)
#define NC_IPTR(x) gsl::narrow_cast<std::intptr_t>(x)
#define BC_IPTR(x) std::bit_cast<std::intptr_t>(x)
/**
 * @brief Macro to cast to std::uintptr_t
 * This macro is used to cast a value to std::uintptr_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uintptr_t.
 * Usage example: @code{.cpp} std::intptr_t ptr = C_UIPTR(somePointer);@endcode
 */
#define C_UIPTR(x) static_cast<std::uintptr_t>(x)
#define NC_UIPTR(x) gsl::narrow_cast<std::uintptr_t>(x)
#define BC_UIPTR(x) std::bit_cast<std::uintptr_t>(x)
/**
 * @brief Macro to cast to std::int8_t.
 * This macro is used to cast a value to std::int8_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int8_t.
 * Usage example: @code{.cpp}  std::int8_t value = C_I8T(42);@endcode
 */
#define C_I8T(x) static_cast<std::int8_t>(x)
#define NC_I8T(x) gsl::narrow_cast<std::int8_t>(x)
#define BC_I8T(x) std::bit_cast<std::int8_t>(x)
/**
 * @brief Macro to cast to std::int16_t.
 * This macro is used to cast a value to std::int16_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int16_t.
 * Usage example: @code{.cpp} std::int16_t value = C_I16T(42);;@endcode
 */
#define C_I16T(x) static_cast<std::int16_t>(x)
#define NC_I16T(x) gsl::narrow_cast<std::int16_t>(x)
#define BC_I16T(x) std::bit_cast<std::int16_t>(x)
/**
 * @brief Macro to cast to std::int32_t.
 * This macro is used to cast a value to std::int32_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int32_t.
 * Usage example: @code{.cpp} std::int32_t value = C_I32T(42);@endcode
 */
#define C_I32T(x) static_cast<std::int32_t>(x)
#define NC_I32T(x) gsl::narrow_cast<std::int32_t>(x)
#define BC_I32T(x) std::bit_cast<std::int32_t>(x)
/**
 * @brief Macro to cast to std::int64_t.
 * This macro is used to cast a value to std::int64_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int64_t.
 * Usage example: @code{.cpp} std::int64_t value = C_I64T(42);@endcode
 */
#define C_I64T(x) static_cast<std::int64_t>(x)
#define NC_I64T(x) gsl::narrow_cast<std::int64_t>(x)
#define BC_I64T(x) std::bit_cast<std::int64_t>(x)
/**
 * @brief Macro to cast to  std::uint8_t.
 * This macro is used to cast a value to std::uint8_t type.
 * @param x The value to be casted.
 * @return The value casted to  std::uint8_t.
 * Usage example: @code{.cpp}  std::uint8_t value = C_UI8T(42);@endcode
 */
#define C_UI8T(x) static_cast<std::uint8_t>(x)
#define NC_UI8T(x) gsl::narrow_cast<std::uint8_t>(x)
#define BC_UI8T(x) std::bit_cast<std::uint8_t>(x)
/**
 * @brief Macro to cast to std::uint16_t.
 * This macro is used to cast a value to std::uint16_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint16_t.
 * Usage example: @code{.cpp}std::uint16_t value = C_UI16T(42);@endcode
 */
#define C_UI16T(x) static_cast<std::uint16_t>(x)
#define NC_UI16T(x) gsl::narrow_cast<std::uint16_t>(x)
#define BC_UI16T(x) std::bit_cast<std::unit16_t>(x)
/**
 * @brief Macro to cast to std::uint32_t.
 * This macro is used to cast a value to std::uint32_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint32_t.
 * Usage example: @code{.cpp} std::uint32_t value = C_UI32T(42);@endcode
 */
#define C_UI32T(x) static_cast<std::uint32_t>(x)
#define NC_UI32T(x) gsl::narrow_cast<std::uint32_t>(x)
#define BC_UI32T(x) std::bit_cast<std::uint32_t>(x)
/**
 * @brief Macro to cast to std::uint64_t.
 * This macro is used to cast a value to std::uint64_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint64_t.
 * Usage example: @code{.cpp} std::uint64_t value = C_UI64T(42);@endcode
 */
#define C_UI64T(x) static_cast<std::uint64_t>(x)
#define NC_UI64T(x) gsl::narrow_cast<std::uint64_t>(x)
#define BC_UI64T(x) std::bit_cast<std::ui64_t>(x)
/**
 * @brief Macro to cast to std::ptrdiff_t.
 * This macro is used to cast a value to std::ptrdiff_t type.
 * @param x The value to be casted.
 * @return The value casted to std::ptrdiff_t.
 * Usage example: @code{.cpp} std::ptrdiff_t diff = C_PTRDIFT(10)@endcode
 */
#define C_PTRDIFT(x) static_cast<std::ptrdiff_t>(x)
#define NC_PTRDIFT(x) gsl::narrow_cast<std::ptrdiff_t>(x)
#define BC_PTRDIFT(x) std::bit_cast<std::ptrdiff_t>(x)
/**
 * @brief Macro to cast to std::div_t.
 * This macro is used to cast a value to std::div_t type.
 * @param x The value to be casted.
 * @return The value casted to std::div_t.
 * Usage example: @code{.cpp} std::div_t result = C_DIVT(10);@endcode
 */
#define C_DIVT(x) static_cast<std::div_t>(x)
#define NC_DIVT(x) gsl::narrow_cast<std::div_t>(x)
#define BC_DIVT(x) std::bit_cast<std::div_t>(x)
/**
 * @brief Macro to cast to std::ldiv_t.
 * This macro is used to cast a value to std::ldiv_t type.
 * @param x The value to be casted.
 * @return The value casted to std::ldiv_t.
 * Usage example: @code{.cpp} std::ldiv_t result = C_LDIVT(10);@endcode
 */
#define C_LDIVT(x) static_cast<std::ldiv_t>(x)
#define NC_LDIVT(x) gsl::narrow_cast<std::ldiv_t>(x)
#define BC_LDIVT(x) std::bit_cast<std::ldiv_t>(x)
/**
 * @brief Macro to cast to char.
 * This macro is used to cast a value to char type.
 * @param x The value to be casted.
 * @return The value casted to char.
 * Usage example: @code{.cpp} char c = C_C(65);@endcode
 */
#define C_C(x) static_cast<char>(x)
#define NC_C(x) gsl::narrow_cast<char>(x)
#define BC_C(x) std::bit_cast<char>(x)
/**
 * @brief Macro to cast to char16_t.
 * This macro is used to cast a value to char16_t type.
 * @param x The value to be casted.
 * @return The value casted to char16_t.
 * Usage example: @code{.cpp} char16_t c = C_C16(65);@endcode
 */
#define C_C16(x) static_cast<char16_t>(x)
#define NC_C16(x) gsl::narrow_cast<char16_t>(x)
#define BC_C16(x) std::bit_cast<char16_t>(x)
/**
 * @brief Macro to cast to char132_t.
 * This macro is used to cast a value to char32_t type.
 * @param x The value to be casted.
 * @return The value casted to char32_t.
 * Usage example: @code{.cpp} char32_t c = C_C32(65);@endcode
 */
#define C_C32(x) static_cast<char32_t>(x)
#define NC_C32(x) gsl::narrow_cast<char32_t>(x)
#define BC_C32(x) std::bit_cast<char32_t>(x)
/**
 * @brief Macro to cast to char8_t.
 * This macro is used to cast a value to char8_t type.
 * @param x The value to be casted.
 * @return The value casted to char8_t.
 * Usage example: @code{.cpp} char8_t c = C_C8(65);@endcode
 */
#define C_C8(x) static_cast<char8_t>(x)
#define NC_C8(x) gsl::narrow_cast<char8_t>(x)
#define BC_C8(x) std::bit_cast<char8_t>(x)
/**
 * @brief Macro to cast to double.
 * This macro is used to cast a value to double type.
 * @param x The value to be casted.
 * @return The value casted to  double.
 * Usage example: @code{.cpp} double d = C_D(3.14f);@endcode
 */
#define C_D(x) static_cast<double>(x)
#define NC_D(x) gsl::narrow_cast<double>(x)
#define BC_D(x) std::bit_cast<double>(x)
/**
 * @brief Macro to cast to float.
 * This macro is used to cast a value to float type.
 * @param x The value to be casted.
 * @return The value casted to  float.
 * Usage example: @code{.cpp} float f = C_F(3.14);@endcode
 */
#define C_F(x) static_cast<float>(x)
#define NC_F(x) gsl::narrow_cast<float>(x)
#define BC_F(x) std::bit_cast<float>(x)
/**
 * @brief Macro to cast to int.
 * This macro is used to cast a value to int type.
 * @param x The value to be casted.
 * @return The value casted to  int.
 * Usage example: @code{.cpp} int i = C_I(3.14f);@endcode
 */
#define C_I(x) static_cast<int>(x)
#define NC_I(x) gsl::narrow_cast<int>(x)
#define BC_I(x) std::bit_cast<int>(x)
/**
 * @brief Macro to cast to long.
 * This macro is used to cast a value to long type.
 * @param x The value to be casted.
 * @return The value casted to  long.
 * Usage example: @code{.cpp} long l = C_L(42);@endcode
 */
#define C_L(x) static_cast<long>(x)
#define NC_L(x) gsl::narrow_cast<long>(x)
#define BC_L(x) std::bit_cast<long>(x)
/**
 * @brief Macro to cast to long double.
 * This macro is used to cast a value to long double type.
 * @param x The value to be casted.
 * @return The value casted to long double.
 * Usage example: @code{.cpp} long double ld = C_LD(3.14f);@endcode
 */
#define C_LD(x) static_cast<long double>(x)
#define NC_LD(x) gsl::narrow_cast<long double>(x)
#define BC_LD(x) std::bit_cast<long double>(x)
/**
 * @brief Macro to cast to long int.
 * This macro is used to cast a value to long int type.
 * @param x The value to be casted.
 * @return The value casted to long int
 * Usage example: @code{.cpp} long int li = C_LI(42);@endcode
 */
#define C_LI(x) static_cast<long int>(x)
#define NC_LI(x) gsl::narrow_cast<long int>(x)
#define BC_LI(x) std::bit_cast<long int>(x)
/**
 * @brief Macro to cast to long long.
 * This macro is used to cast a value to long long type.
 * @param x The value to be casted.
 * @return The value casted to long long
 * Usage example: @code{.cpp} long long ll = C_LL(42);@endcode
 */
#define C_LL(x) static_cast<long long>(x)
#define NC_LL(x) gsl::narrow_cast<long long>(x)
#define BC_LL(x) std::bit_cast<long long>(x)
/**
 * @brief Macro to cast to long long int.
 * This macro is used to cast a value to long long int type.
 * @param x The value to be casted.
 * @return The value casted to long long int
 * Usage example: @code{.cpp} long long int lli = C_LLI(42);@endcode
 */
#define C_LLI(x) static_cast<long long int>(x)
#define NC_LLI(x) gsl::narrow_cast<long long int>(x)
#define BC_LLI(x) std::bit_cast<long long int>(x)
/**
 * @brief Macro to cast to short.
 * This macro is used to cast a value to short type.
 * @param x The value to be casted.
 * @return The value casted to short
 * Usage example: @code{.cpp} short s = C_S(42);@endcode
 */
#define C_S(x) static_cast<short>(x)
#define NC_S(x) gsl::narrow_cast<short>(x)
#define BC_S(x) std::bit_cast<short>(x)
/**
 * @brief Macro to cast to short int.
 * This macro is used to cast a value to short int type.
 * @param x The value to be casted.
 * @return The value casted to short int
 * Usage example: @code{.cpp} short si = C_SI(42);@endcode
 */
#define C_SI(x) static_cast<short int>(x)
#define NC_SI(x) gsl::narrow_cast<short int>(x)
#define BC_SI(x) std::bit_cast<short int>(x)
/**
 * @brief Macro to cast to unsigned char.
 * This macro is used to cast a value to unsigned char type.
 * @param x The value to be casted.
 * @return The value casted to unsigned char.
 * Usage example: @code{.cpp} unsigned char uc = C_UC(65);@endcode
 */
#define C_UC(x) static_cast<unsigned char>(x)
#define NC_UC(x) gsl::narrow_cast<unsigned char>(x)
#define BC_UC(x) std::bit_cast<unsigned char>(x)
/**
 * @brief Macro to cast to unsigned int.
 * This macro is used to cast a value to unsigned int type.
 * @param x The value to be casted.
 * @return The value casted to unsigned int.
 * Usage example: @code{.cpp} unsigned int ui = C_UI(42);@endcode
 */
#define C_UI(x) static_cast<unsigned int>(x)
#define NC_UI(x) gsl::narrow_cast<unsigned int>(x)
#define BC_UI(x) std::bit_cast<unsigned int>(x)
/**
 * @brief Macro to cast to unsigned long.
 * This macro is used to cast a value to unsigned long type.
 * @param x The value to be casted.
 * @return The value casted to unsigned char.
 * Usage example: @code{.cpp} unsigned long ul = C_UL(42);@endcode
 */
#define C_UL(x) static_cast<unsigned long>(x)
#define NC_UL(x) gsl::narrow_cast<unsigned long>(x)
#define BC_UL(x) std::bit_cast<unsigned long>(x)
/**
 * @brief Macro to cast to unsigned long int.
 * This macro is used to cast a value to unsigned long int type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long int.
 * Usage example: @code{.cpp} unsigned long int uli = C_ULI(65);@endcode
 */
#define C_ULI(x) static_cast<unsigned long int>(x)
#define NC_ULI(x) gsl::narrow_cast<unsigned long int>(x)
#define BC_ULI(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to unsigned long long.
 * This macro is used to cast a value to unsigned long long type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long long.
 * Usage example: @code{.cpp} unsigned long long ull = C_ULL(42);@endcode
 */
#define C_ULL(x) static_cast<unsigned long long>(x)
#define NC_ULL(x) gsl::narrow_cast<unsigned long long>(x)
#define BC_ULL(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to unsigned long long int.
 * This macro is used to cast a value to unsigned long long int char type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long long int.
 * Usage example: @code{.cpp} unsigned char ulli = C_ULLI(42);@endcode
 */
#define C_ULLI(x) static_cast<unsigned long long int>(x)
#define NC_ULLI(x) gsl::narrow_cast<unsigned long long int>(x)
#define BC_ULLI(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::string.
 * This macro is used to cast a value to std::string type.
 * @param x The value to be casted.
 * @return The value casted to std::string.
 * Usage example: @code{.cpp} std::string str = C_STR("Hello");@endcode
 */
#define C_STR(x) static_cast<std::string>(x)
#define NC_STR(x) gsl::narrow_cast<std::string>(x)
#define BC_STR(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::wstring.
 * This macro is used to cast a value to std::wstring type.
 * @param x The value to be casted.
 * @return The value casted to std::wstring.
 * Usage example: @code{.cpp} std::wstring wstr = C_WSTR("Hello");@endcode
 */
#define C_WSTR(x) static_cast<std::wstring>(x)
#define NC_WSTR(x) gsl::narrow_cast<std::wstring>(x)
#define BC_WSTR(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::u8string.
 * This macro is used to cast a value to std::u8string type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string.
 * Usage example: @code{.cpp} std::u8string u8str = C_U8STR(u8"Hello");@endcode
 */
#define C_U8STR(x) static_cast<std::u8string>(x)
#define NC_U8STR(x) gsl::narrow_cast<std::u8string>(x)
#define BC_U8STR(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::u16string.
 * This macro is used to cast a value to std::u16string type.
 * @param x The value to be casted.
 * @return The value casted to std::u16string.
 * Usage example: @code{.cpp} std::u16string u16str = C_U16STR(u"Hello");@endcode
 */
#define C_U16STR(x) static_cast<std::u16string>(x)
#define NC_U16STR(x) gsl::narrow_cast<std::u16string>(x)
#define BC_U16STR(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::u32string.
 * This macro is used to cast a value to std::u32string type.
 * @param x The value to be casted.
 * @return The value casted to std::u32string.
 * Usage example: @code{.cpp} std::u32string u32str = C_U32STR(U"Hello");@endcode
 */
#define C_U32STR(x) static_cast<std::u32string>(x)
#define NC_U32STR(x) gsl::narrow_cast<std::u32string>(x)
#define BC_U32STR(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::string_view.
 * This macro is used to cast a value to std::string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::string_view.
 * Usage example: @code{.cpp} std::string_view strView = C_STRV("Hello, World!");@endcode
 */
#define C_STRV(x) static_cast<std::string_view>(x)
#define NC_STRV(x) gsl::narrow_cast<std::string_view>(x)
#define BC_STRV(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::wstring_view.
 * This macro is used to cast a value to std::wstring_view type.
 * @param x The value to be casted.
 * @return The value casted to std::wstring_view.
 * Usage example: @code{.cpp} std::wstring_view wstrView = C_WSTRV(L"Hello, World!");@endcode
 */
#define C_WSTRV(x) static_cast<std::wstring_view>(x)
#define NC_WSTRV(x) gsl::narrow_cast<std::wstring_view>(x)
#define BC_WSTRV(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::u8string_view.
 * This macro is used to cast a value to std::u8string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string_view.
 * Usage example: @code{.cpp} std::u8string_view u8strView = C_U8STRV(u8"Hello, World!");@endcode
 */
#define C_U8STRV(x) static_cast<std::u8string_view>(x)
#define NC_U8STRV(x) gsl::narrow_cast<std::u8string_view>(x)
#define BC_U8STRV(x) std::bit_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::u16string_view.
 * This macro is used to cast a value to std::u16string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string_view.
 * Usage example: @code{.cpp} std::u16string_view u16strView = C_U16STRV(u8"Hello, World!");@endcode
 */
#define C_U16STRV(x) static_cast<std::u16string_view>(x)
#define NC_U16STRV(x) gsl::narrow_cast<std::u16string_view>(x)
#define BC_U16STRV(x) std::bit_cast<std::u16string_view>(x)
/**
 * @brief Macro to cast to std::u32string_view.
 * This macro is used to cast a value to std::u32string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u32string_view.
 * Usage example: @code{.cpp} std::u32string_view u32strView = C_U32STRV(u8"Hello, World!");@endcode
 */
#define C_U32STRV(x) static_cast<std::u32string_view>(x)
#define NC_U32STRV(x) gsl::narrow_cast<std::u32string_view>(x)
#define BC_U32STRV(x) std::bit_cast<std::u32string_view>(x)

/**
 * @brief Macro to cast to std::size_t.
 * This macro is used to cast a value to std::size_t type.
 * @param x The value to be casted.
 * @return The value casted to std::size_t.
 * Usage example: @code{.cpp} std::size_t st = C_ST(42);@endcode
 */
#define C_ST(x) static_cast<std::size_t>(x)
#define NC_ST(x) gsl::narrow_cast<std::size_t>(x)
#define BC_ST(x) std::bit_cast<std::size_t>(x)

/**
 * @brief Macro to cast to const uint32_t *.
 * This macro is used to cast a value to const uint32_t * type.
 * @param x The value to be casted.
 * @return The value casted to const uint32_t *.
 * Usage example: @code{.cpp} const uint32_t * cpu32t = C_CPCU32T(&42);@endcode
 */
#define C_CPCU32T(x) static_cast<const uint32_t *>(static_cast<const void *>(x))
#define NC_CPCU32T(x) gsl::narrow_cast<const uint32_t *>(static_cast<const void *>(x))
#define BC_CPCU32T(x) std::bit_cast<const uint32_t *>(x)

/** @} */  // end of TypeCastingMacros group
/**
 * @brief Namespace containing sizes of various types.
 * @namespace TypeSizes
 * @{
 */
namespace TypeSizes {
    /// Size of a boolean type.
    static inline constexpr std::size_t sizeOfBool = sizeof(bool);
    /// Size of a byte type.
    static inline constexpr std::size_t sizeOfByte = sizeof(std::byte);
    /// Size of a pointer to an integer type.
    static inline constexpr std::size_t sizeOfIntPtr = sizeof(std::intptr_t);
    /// Size of a pointer to an unsigned integer type.
    static inline constexpr std::size_t sizeOfUintPtr = sizeof(std::uintptr_t);
    /// Size of a signed 8-bit integer type.
    static inline constexpr std::size_t sizeOfInt8T = sizeof(std::int8_t);
    /// Size of a signed 16-bit integer type.
    static inline constexpr std::size_t sizeOfInt16T = sizeof(std::int16_t);
    /// Size of a signed 32-bit integer type.
    static inline constexpr std::size_t sizeOfInt32T = sizeof(std::int32_t);
    /// Size of a signed 64-bit integer type.
    static inline constexpr std::size_t sizeOfInt64T = sizeof(std::int64_t);
    /// Size of an unsigned 8-bit integer type.
    static inline constexpr std::size_t sizeOfUint8T = sizeof(std::uint8_t);
    /// Size of an unsigned 16-bit integer type.
    static inline constexpr std::size_t sizeOfUint16T = sizeof(std::uint16_t);
    /// Size of an unsigned 32-bit integer type.
    static inline constexpr std::size_t sizeOfUint32T = sizeof(std::uint32_t);
    /// Size of an unsigned 64-bit integer type.
    static inline constexpr std::size_t sizeOfUint64T = sizeof(std::uint64_t);
    /// Size of a pointer difference type.
    static inline constexpr std::size_t sizeOfPtrdiffT = sizeof(std::ptrdiff_t);
    /// Size of a structure holding the result of dividing two integers.
    static inline constexpr std::size_t sizeOfDivT = sizeof(std::div_t);
    /// Size of a structure holding the result of dividing two long integers.
    static inline constexpr std::size_t sizeOfLdivT = sizeof(std::ldiv_t);
    /// Size of a character type.
    static inline constexpr std::size_t sizeOfChar = sizeof(char);
    /// Size of a 16-bit character type.
    static inline constexpr std::size_t sizeOfChar16T = sizeof(char16_t);
    /// Size of a 32-bit character type.
    static inline constexpr std::size_t sizeOfChar32T = sizeof(char32_t);
    /// Size of an 8-bit character type.
    static inline constexpr std::size_t sizeOfChar8T = sizeof(char8_t);
    /// Size of a double-precision floating-point type.
    static inline constexpr std::size_t sizeOfDouble = sizeof(double);
    /// Size of a single-precision floating-point type.
    static inline constexpr std::size_t sizeOfFloat = sizeof(float);
    /// Size of an integer type.
    static inline constexpr std::size_t sizeOfInt = sizeof(int);
    /// Size of a long integer type.
    static inline constexpr std::size_t sizeOfLong = sizeof(long);
    /// Size of a long double-precision floating-point type.
    static inline constexpr std::size_t sizeOfLongDouble = sizeof(long double);
    /// Size of a long integer type.
    static inline constexpr std::size_t sizeOfLongInt = sizeof(long int);
    /// Size of a long long integer type.
    static inline constexpr std::size_t sizeOfLongLong = sizeof(long long);
    /// Size of a long long integer type.
    static inline constexpr std::size_t sizeOfLongLongInt = sizeof(long long int);
    /// Size of a short integer type.
    static inline constexpr std::size_t sizeOfShort = sizeof(short);
    /// Size of a short integer type.
    static inline constexpr std::size_t sizeOfShortInt = sizeof(short int);
    /// Size of an unsigned character type.
    static inline constexpr std::size_t sizeOfUChar = sizeof(unsigned char);
    /// Size of an unsigned integer type.
    static inline constexpr std::size_t sizeOfUInt = sizeof(unsigned int);
    /// Size of an unsigned long integer type.
    static inline constexpr std::size_t sizeOfULong = sizeof(unsigned long);
    /// Size of an unsigned long integer type.
    static inline constexpr std::size_t sizeOfULongInt = sizeof(unsigned long int);
    /// Size of an unsigned long long integer type.
    static inline constexpr std::size_t sizeOfULongLong = sizeof(unsigned long long);
    /// Size of an unsigned long long integer type.
    static inline constexpr std::size_t sizeOfULongLongInt = sizeof(unsigned long long int);
    /// Size of a C++ standard string.
    static inline constexpr std::size_t sizeOfString = sizeof(std::string);
    /// Size of a wide character string.
    static inline constexpr std::size_t sizeOfWString = sizeof(std::wstring);
    /// Size of a UTF-8 encoded string.
    static inline constexpr std::size_t sizeOfU8String = sizeof(std::u8string);
    /// Size of a UTF-16 encoded string.
    static inline constexpr std::size_t sizeOfU16String = sizeof(std::u16string);
    /// Size of a UTF-32 encoded string.
    static inline constexpr std::size_t sizeOfU32String = sizeof(std::u32string);
    /// Size of a C++ standard string view.
    static inline constexpr std::size_t sizeOfStringView = sizeof(std::string_view);
    /// Size of a wide character string view.
    static inline constexpr std::size_t sizeOfWStringView = sizeof(std::wstring_view);
    /// Size of a UTF-8 encoded string view.
    static inline constexpr std::size_t sizeOfU8StringView = sizeof(std::u8string_view);
    /// Size of a UTF-16 encoded string view.
    static inline constexpr std::size_t sizeOfU16StringView = sizeof(std::u16string_view);
    /// Size of a UTF-32 encoded string view.
    static inline constexpr std::size_t sizeOfU32StringView = sizeof(std::u32string_view);
}  // namespace TypeSizes
/** @} */  // end of TypeSizes namespace
