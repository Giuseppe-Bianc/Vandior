//
// Created by gbian on 01/06/2024.
//

#pragma once

/**
 * @brief Namespace containing sizes of various types.
 *
 * This namespace provides constants representing the sizes of different types in bytes.
 * It includes fundamental types, pointer types, character types, floating-point types,
 * and string types.
 *
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