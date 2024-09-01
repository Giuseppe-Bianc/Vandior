//
// Created by gbian on 01/06/2024.
//
// NOLINTBEGIN(*-macro-usage)
#pragma once

#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

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
/**
 * @brief Macro to cast to std::byte.
 * This macro is used to cast a value to std::byte type.
 * @param x The value to cast.
 * @return The casted value.
 * Usage example: @code{.cpp} std::byte b = C_B(255); @endcode
 */
#define C_B(x) static_cast<std::byte>(x)
/**
 * @brief Macro to cast to std::intptr_t.
 * This macro is used to cast a value to std::intptr_t type.
 * @param x The value to be casted.
 * @return The value casted to std::intptr_t.
 * Usage example: @code{.cpp} std::intptr_t ptr = C_IPTR(somePointer);@endcode
 */
#define C_IPTR(x) static_cast<std::intptr_t>(x)
/**
 * @brief Macro to cast to std::uintptr_t
 * This macro is used to cast a value to std::uintptr_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uintptr_t.
 * Usage example: @code{.cpp} std::intptr_t ptr = C_UIPTR(somePointer);@endcode
 */
#define C_UIPTR(x) static_cast<std::uintptr_t>(x)
/**
 * @brief Macro to cast to std::int8_t.
 * This macro is used to cast a value to std::int8_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int8_t.
 * Usage example: @code{.cpp}  std::int8_t value = C_I8T(42);@endcode
 */
#define C_I8T(x) static_cast<std::int8_t>(x)
/**
 * @brief Macro to cast to std::int16_t.
 * This macro is used to cast a value to std::int16_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int16_t.
 * Usage example: @code{.cpp} std::int16_t value = C_I16T(42);;@endcode
 */
#define C_I16T(x) static_cast<std::int16_t>(x)
/**
 * @brief Macro to cast to std::int32_t.
 * This macro is used to cast a value to std::int32_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int32_t.
 * Usage example: @code{.cpp} std::int32_t value = C_I32T(42);@endcode
 */
#define C_I32T(x) static_cast<std::int32_t>(x)
/**
 * @brief Macro to cast to std::int64_t.
 * This macro is used to cast a value to std::int64_t type.
 * @param x The value to be casted.
 * @return The value casted to std::int64_t.
 * Usage example: @code{.cpp} std::int64_t value = C_I64T(42);@endcode
 */
#define C_I64T(x) static_cast<std::int64_t>(x)
/**
 * @brief Macro to cast to  std::uint8_t.
 * This macro is used to cast a value to std::uint8_t type.
 * @param x The value to be casted.
 * @return The value casted to  std::uint8_t.
 * Usage example: @code{.cpp}  std::uint8_t value = C_UI8T(42);@endcode
 */
#define C_UI8T(x) static_cast<std::uint8_t>(x)
/**
 * @brief Macro to cast to std::uint16_t.
 * This macro is used to cast a value to std::uint16_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint16_t.
 * Usage example: @code{.cpp}std::uint16_t value = C_UI16T(42);@endcode
 */
#define C_UI16T(x) static_cast<std::uint16_t>(x)
/**
 * @brief Macro to cast to std::uint32_t.
 * This macro is used to cast a value to std::uint32_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint32_t.
 * Usage example: @code{.cpp} std::uint32_t value = C_UI32T(42);@endcode
 */
#define C_UI32T(x) static_cast<std::uint32_t>(x)
/**
 * @brief Macro to cast to std::uint64_t.
 * This macro is used to cast a value to std::uint64_t type.
 * @param x The value to be casted.
 * @return The value casted to std::uint64_t.
 * Usage example: @code{.cpp} std::uint64_t value = C_UI64T(42);@endcode
 */
#define C_UI64T(x) static_cast<std::uint64_t>(x)
/**
 * @brief Macro to cast to std::ptrdiff_t.
 * This macro is used to cast a value to std::ptrdiff_t type.
 * @param x The value to be casted.
 * @return The value casted to std::ptrdiff_t.
 * Usage example: @code{.cpp} std::ptrdiff_t diff = C_PTRDIFT(10)@endcode
 */
#define C_PTRDIFT(x) static_cast<std::ptrdiff_t>(x)
/**
 * @brief Macro to cast to std::div_t.
 * This macro is used to cast a value to std::div_t type.
 * @param x The value to be casted.
 * @return The value casted to std::div_t.
 * Usage example: @code{.cpp} std::div_t result = C_DIVT(10);@endcode
 */
#define C_DIVT(x) static_cast<std::div_t>(x)
/**
 * @brief Macro to cast to std::ldiv_t.
 * This macro is used to cast a value to std::ldiv_t type.
 * @param x The value to be casted.
 * @return The value casted to std::ldiv_t.
 * Usage example: @code{.cpp} std::ldiv_t result = C_LDIVT(10);@endcode
 */
#define C_LDIVT(x) static_cast<std::ldiv_t>(x)
/**
 * @brief Macro to cast to char.
 * This macro is used to cast a value to char type.
 * @param x The value to be casted.
 * @return The value casted to char.
 * Usage example: @code{.cpp} char c = C_C(65);@endcode
 */
#define C_C(x) static_cast<char>(x)
/**
 * @brief Macro to cast to char16_t.
 * This macro is used to cast a value to char16_t type.
 * @param x The value to be casted.
 * @return The value casted to char16_t.
 * Usage example: @code{.cpp} char16_t c = C_C16(65);@endcode
 */
#define C_C16(x) static_cast<char16_t>(x)
/**
 * @brief Macro to cast to char132_t.
 * This macro is used to cast a value to char32_t type.
 * @param x The value to be casted.
 * @return The value casted to char32_t.
 * Usage example: @code{.cpp} char32_t c = C_C32(65);@endcode
 */
#define C_C32(x) static_cast<char32_t>(x)
/**
 * @brief Macro to cast to char8_t.
 * This macro is used to cast a value to char8_t type.
 * @param x The value to be casted.
 * @return The value casted to char8_t.
 * Usage example: @code{.cpp} char8_t c = C_C8(65);@endcode
 */
#define C_C8(x) static_cast<char8_t>(x)
/**
 * @brief Macro to cast to double.
 * This macro is used to cast a value to double type.
 * @param x The value to be casted.
 * @return The value casted to  double.
 * Usage example: @code{.cpp} double d = C_D(3.14f);@endcode
 */
#define C_D(x) static_cast<double>(x)
/**
 * @brief Macro to cast to float.
 * This macro is used to cast a value to float type.
 * @param x The value to be casted.
 * @return The value casted to  float.
 * Usage example: @code{.cpp} float f = C_F(3.14);@endcode
 */
#define C_F(x) static_cast<float>(x)
/**
 * @brief Macro to cast to int.
 * This macro is used to cast a value to int type.
 * @param x The value to be casted.
 * @return The value casted to  int.
 * Usage example: @code{.cpp} int i = C_I(3.14f);@endcode
 */
#define C_I(x) static_cast<int>(x)
/**
 * @brief Macro to cast to long.
 * This macro is used to cast a value to long type.
 * @param x The value to be casted.
 * @return The value casted to  long.
 * Usage example: @code{.cpp} long l = C_L(42);@endcode
 */
#define C_L(x) static_cast<long>(x)
/**
 * @brief Macro to cast to long double.
 * This macro is used to cast a value to long double type.
 * @param x The value to be casted.
 * @return The value casted to long double.
 * Usage example: @code{.cpp} long double ld = C_LD(3.14f);@endcode
 */
#define C_LD(x) static_cast<long double>(x)
/**
 * @brief Macro to cast to long int.
 * This macro is used to cast a value to long int type.
 * @param x The value to be casted.
 * @return The value casted to long int
 * Usage example: @code{.cpp} long int li = C_LI(42);@endcode
 */
#define C_LI(x) static_cast<long int>(x)
/**
 * @brief Macro to cast to long long.
 * This macro is used to cast a value to long long type.
 * @param x The value to be casted.
 * @return The value casted to long long
 * Usage example: @code{.cpp} long long ll = C_LL(42);@endcode
 */
#define C_LL(x) static_cast<long long>(x)
/**
 * @brief Macro to cast to long long int.
 * This macro is used to cast a value to long long int type.
 * @param x The value to be casted.
 * @return The value casted to long long int
 * Usage example: @code{.cpp} long long int lli = C_LLI(42);@endcode
 */
#define C_LLI(x) static_cast<long long int>(x)
/**
 * @brief Macro to cast to short.
 * This macro is used to cast a value to short type.
 * @param x The value to be casted.
 * @return The value casted to short
 * Usage example: @code{.cpp} short s = C_S(42);@endcode
 */
#define C_S(x) static_cast<short>(x)
/**
 * @brief Macro to cast to short int.
 * This macro is used to cast a value to short int type.
 * @param x The value to be casted.
 * @return The value casted to short int
 * Usage example: @code{.cpp} short si = C_SI(42);@endcode
 */
#define C_SI(x) static_cast<short int>(x)
/**
 * @brief Macro to cast to unsigned char.
 * This macro is used to cast a value to unsigned char type.
 * @param x The value to be casted.
 * @return The value casted to unsigned char.
 * Usage example: @code{.cpp} unsigned char uc = C_UC(65);@endcode
 */
#define C_UC(x) static_cast<unsigned char>(x)
/**
 * @brief Macro to cast to unsigned int.
 * This macro is used to cast a value to unsigned int type.
 * @param x The value to be casted.
 * @return The value casted to unsigned int.
 * Usage example: @code{.cpp} unsigned int ui = C_UI(42);@endcode
 */
#define C_UI(x) static_cast<unsigned int>(x)
/**
 * @brief Macro to cast to unsigned long.
 * This macro is used to cast a value to unsigned long type.
 * @param x The value to be casted.
 * @return The value casted to unsigned char.
 * Usage example: @code{.cpp} unsigned long ul = C_UL(42);@endcode
 */
#define C_UL(x) static_cast<unsigned long>(x)
/**
 * @brief Macro to cast to unsigned long int.
 * This macro is used to cast a value to unsigned long int type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long int.
 * Usage example: @code{.cpp} unsigned long int uli = C_ULI(65);@endcode
 */
#define C_ULI(x) static_cast<unsigned long int>(x)
/**
 * @brief Macro to cast to unsigned long long.
 * This macro is used to cast a value to unsigned long long type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long long.
 * Usage example: @code{.cpp} unsigned long long ull = C_ULL(42);@endcode
 */
#define C_ULL(x) static_cast<unsigned long long>(x)
/**
 * @brief Macro to cast to unsigned long long int.
 * This macro is used to cast a value to unsigned long long int char type.
 * @param x The value to be casted.
 * @return The value casted to unsigned long long int.
 * Usage example: @code{.cpp} unsigned char ulli = C_ULLI(42);@endcode
 */
#define C_ULLI(x) static_cast<unsigned long long int>(x)
/**
 * @brief Macro to cast to std::string.
 * This macro is used to cast a value to std::string type.
 * @param x The value to be casted.
 * @return The value casted to std::string.
 * Usage example: @code{.cpp} std::string str = C_STR("Hello");@endcode
 */
#define C_STR(x) static_cast<std::string>(x)
/**
 * @brief Macro to cast to std::wstring.
 * This macro is used to cast a value to std::wstring type.
 * @param x The value to be casted.
 * @return The value casted to std::wstring.
 * Usage example: @code{.cpp} std::wstring wstr = C_WSTR("Hello");@endcode
 */
#define C_WSTR(x) static_cast<std::wstring>(x)
/**
 * @brief Macro to cast to std::u8string.
 * This macro is used to cast a value to std::u8string type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string.
 * Usage example: @code{.cpp} std::u8string u8str = C_U8STR(u8"Hello");@endcode
 */
#define C_U8STR(x) static_cast<std::u8string>(x)
/**
 * @brief Macro to cast to std::u16string.
 * This macro is used to cast a value to std::u16string type.
 * @param x The value to be casted.
 * @return The value casted to std::u16string.
 * Usage example: @code{.cpp} std::u16string u16str = C_U16STR(u"Hello");@endcode
 */
#define C_U16STR(x) static_cast<std::u16string>(x)
/**
 * @brief Macro to cast to std::u32string.
 * This macro is used to cast a value to std::u32string type.
 * @param x The value to be casted.
 * @return The value casted to std::u32string.
 * Usage example: @code{.cpp} std::u32string u32str = C_U32STR(U"Hello");@endcode
 */
#define C_U32STR(x) static_cast<std::u32string>(x)
/**
 * @brief Macro to cast to std::string_view.
 * This macro is used to cast a value to std::string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::string_view.
 * Usage example: @code{.cpp} std::string_view strView = C_STRV("Hello, World!");@endcode
 */
#define C_STRV(x) static_cast<std::string_view>(x)
/**
 * @brief Macro to cast to std::wstring_view.
 * This macro is used to cast a value to std::wstring_view type.
 * @param x The value to be casted.
 * @return The value casted to std::wstring_view.
 * Usage example: @code{.cpp} std::wstring_view wstrView = C_WSTRV(L"Hello, World!");@endcode
 */
#define C_WSTRV(x) static_cast<std::wstring_view>(x)
/**
 * @brief Macro to cast to std::u8string_view.
 * This macro is used to cast a value to std::u8string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string_view.
 * Usage example: @code{.cpp} std::u8string_view u8strView = C_U8STRV(u8"Hello, World!");@endcode
 */
#define C_U8STRV(x) static_cast<std::u8string_view>(x)
/**
 * @brief Macro to cast to std::u16string_view.
 * This macro is used to cast a value to std::u16string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u8string_view.
 * Usage example: @code{.cpp} std::u16string_view u16strView = C_U16STRV(u8"Hello, World!");@endcode
 */
#define C_U16STRV(x) static_cast<std::u16string_view>(x)
/**
 * @brief Macro to cast to std::u32string_view.
 * This macro is used to cast a value to std::u32string_view type.
 * @param x The value to be casted.
 * @return The value casted to std::u32string_view.
 * Usage example: @code{.cpp} std::u32string_view u32strView = C_U32STRV(u8"Hello, World!");@endcode
 */
#define C_U32STRV(x) static_cast<std::u32string_view>(x)

/**
 * @brief Macro to cast to std::size_t.
 * This macro is used to cast a value to std::size_t type.
 * @param x The value to be casted.
 * @return The value casted to std::size_t.
 * Usage example: @code{.cpp} std::size_t st = C_ST(42);@endcode
 */
#define C_ST(x) static_cast<std::size_t>(x)

/**
 * @brief Macro to cast to const uint32_t *.
 * This macro is used to cast a value to const uint32_t * type.
 * @param x The value to be casted.
 * @return The value casted to const uint32_t *.
 * Usage example: @code{.cpp} const uint32_t * cpu32t = C_CPCU32T(&42);@endcode
 */
#define C_CPCU32T(x) static_cast<const uint32_t *>(static_cast<const void *>(x))
/** @} */  // end of TypeCastingMacros group

// NOLINTEND(*-macro-usage)
