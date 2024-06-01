//
// Created by gbian on 01/06/2024.
//

#pragma once

#include <bit>

/**
 * @defgroup BitTypeCastingMacros Type Casting Macros
 * @{
 */


#define BC_BOOL(x) std::bit_cast<bool>(x)
#define BC_B(x) std::bit_cast<std::byte>(x)
#define BC_IPTR(x) std::bit_cast<std::intptr_t>(x)
#define BC_UIPTR(x) std::bit_cast<std::uintptr_t>(x)
#define BC_I8T(x) std::bit_cast<std::int8_t>(x)
#define BC_I16T(x) std::bit_cast<std::int16_t>(x)
#define BC_I32T(x) std::bit_cast<std::int32_t>(x)
#define BC_I64T(x) std::bit_cast<std::int64_t>(x)
#define BC_UI8T(x) std::bit_cast<std::uint8_t>(x)
#define BC_UI16T(x) std::bit_cast<std::unit16_t>(x)
#define BC_UI32T(x) std::bit_cast<std::uint32_t>(x)
#define BC_UI64T(x) std::bit_cast<std::ui64_t>(x)
#define BC_PTRDIFT(x) std::bit_cast<std::ptrdiff_t>(x)
#define BC_DIVT(x) std::bit_cast<std::div_t>(x)
#define BC_LDIVT(x) std::bit_cast<std::ldiv_t>(x)
#define BC_C(x) std::bit_cast<char>(x)
#define BC_C16(x) std::bit_cast<char16_t>(x)
#define BC_C32(x) std::bit_cast<char32_t>(x)
#define BC_C8(x) std::bit_cast<char8_t>(x)
#define BC_D(x) std::bit_cast<double>(x)
#define BC_F(x) std::bit_cast<float>(x)
#define BC_I(x) std::bit_cast<int>(x)
#define BC_L(x) std::bit_cast<long>(x)
#define BC_LD(x) std::bit_cast<long double>(x)
#define BC_LI(x) std::bit_cast<long int>(x)
#define BC_LL(x) std::bit_cast<long long>(x)
#define BC_LLI(x) std::bit_cast<long long int>(x)
#define BC_S(x) std::bit_cast<short>(x)
#define BC_SI(x) std::bit_cast<short int>(x)
#define BC_UC(x) std::bit_cast<unsigned char>(x)
#define BC_UI(x) std::bit_cast<unsigned int>(x)
#define BC_UL(x) std::bit_cast<unsigned long>(x)
#define BC_ULI(x) std::bit_cast<unsigned long int>(x)
#define BC_ULLI(x) std::bit_cast<unsigned long long int>(x)
#define BC_STR(x) std::bit_cast<std::string>(x)
#define BC_WSTR(x) std::bit_cast<std::wtring>(x)
#define BC_U8STR(x) std::bit_cast<std::u8string>(x)
#define BC_U16STR(x) std::bit_cast<std::u16string>(x)
#define BC_U32STR(x) std::bit_cast<std::u32string>(x)
#define BC_STRV(x) std::bit_cast<std::string_view>(x)
#define BC_WSTRV(x) std::bit_cast<std::wstring_view>(x)
#define BC_U8STRV(x) std::bit_cast<std::u8string_view>(x)
#define BC_U16STRV(x) std::bit_cast<std::u16string_view>(x)
#define BC_U32STRV(x) std::bit_cast<std::u32string_view>(x)
#define NC_ST(x) gsl::narrow_cast<std::size_t>(x)
#define BC_ST(x) std::bit_cast<std::size_t>(x)
#define BC_CPCU32T(x) std::bit_cast<const uint32_t *>(x)

/** @} */  // end of TypeCastingMacros group
