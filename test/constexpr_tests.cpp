// NOLINTBEGIN(*-err58-cpp, *-include-cleaner, *-use-anonymous-namespace, *-function-cognitive-complexity, *-avoid-do-while)
#include <catch2/catch_test_macros.hpp>

#include <Vandior/vandior.hpp>

TEST_CASE("the constexpr size of types", "[TypeSizes]") {
    STATIC_REQUIRE(TypeSizes::sizeOfBool == sizeof(bool));
    STATIC_REQUIRE(TypeSizes::sizeOfByte == sizeof(std::byte));
    STATIC_REQUIRE(TypeSizes::sizeOfIntPtr == sizeof(std::intptr_t));
    STATIC_REQUIRE(TypeSizes::sizeOfUintPtr == sizeof(std::uintptr_t));
    STATIC_REQUIRE(TypeSizes::sizeOfInt8T == sizeof(std::int8_t));
    STATIC_REQUIRE(TypeSizes::sizeOfInt16T == sizeof(std::int16_t));
    STATIC_REQUIRE(TypeSizes::sizeOfInt32T == sizeof(std::int32_t));
    STATIC_REQUIRE(TypeSizes::sizeOfInt64T == sizeof(std::int64_t));
    STATIC_REQUIRE(TypeSizes::sizeOfUint8T == sizeof(std::uint8_t));
    STATIC_REQUIRE(TypeSizes::sizeOfUint16T == sizeof(std::uint16_t));
    STATIC_REQUIRE(TypeSizes::sizeOfUint32T == sizeof(std::uint32_t));
    STATIC_REQUIRE(TypeSizes::sizeOfUint64T == sizeof(std::uint64_t));
    STATIC_REQUIRE(TypeSizes::sizeOfPtrdiffT == sizeof(std::ptrdiff_t));
    STATIC_REQUIRE(TypeSizes::sizeOfDivT == sizeof(std::div_t));
    STATIC_REQUIRE(TypeSizes::sizeOfLdivT == sizeof(std::ldiv_t));
    STATIC_REQUIRE(TypeSizes::sizeOfChar == sizeof(char));
    STATIC_REQUIRE(TypeSizes::sizeOfChar16T == sizeof(char16_t));
    STATIC_REQUIRE(TypeSizes::sizeOfChar32T == sizeof(char32_t));
    STATIC_REQUIRE(TypeSizes::sizeOfChar8T == sizeof(char8_t));
    STATIC_REQUIRE(TypeSizes::sizeOfDouble == sizeof(double));
    STATIC_REQUIRE(TypeSizes::sizeOfFloat == sizeof(float));
    STATIC_REQUIRE(TypeSizes::sizeOfInt == sizeof(int));
    STATIC_REQUIRE(TypeSizes::sizeOfLong == sizeof(long));
    STATIC_REQUIRE(TypeSizes::sizeOfLongDouble == sizeof(long double));
    STATIC_REQUIRE(TypeSizes::sizeOfLongInt == sizeof(long int));
    STATIC_REQUIRE(TypeSizes::sizeOfLongLong == sizeof(long long));
    STATIC_REQUIRE(TypeSizes::sizeOfLongLongInt == sizeof(long long int));
    STATIC_REQUIRE(TypeSizes::sizeOfShort == sizeof(short));
    STATIC_REQUIRE(TypeSizes::sizeOfShortInt == sizeof(short int));
    STATIC_REQUIRE(TypeSizes::sizeOfUChar == sizeof(unsigned char));
    STATIC_REQUIRE(TypeSizes::sizeOfUInt == sizeof(unsigned int));
    STATIC_REQUIRE(TypeSizes::sizeOfULong == sizeof(unsigned long));
    STATIC_REQUIRE(TypeSizes::sizeOfULongInt == sizeof(unsigned long int));
    STATIC_REQUIRE(TypeSizes::sizeOfULongLong == sizeof(unsigned long long));
    STATIC_REQUIRE(TypeSizes::sizeOfULongLongInt == sizeof(unsigned long long int));
    STATIC_REQUIRE(TypeSizes::sizeOfString == sizeof(std::string));
    STATIC_REQUIRE(TypeSizes::sizeOfWString == sizeof(std::wstring));
    STATIC_REQUIRE(TypeSizes::sizeOfU8String == sizeof(std::u8string));
    STATIC_REQUIRE(TypeSizes::sizeOfU16String == sizeof(std::u16string));
    STATIC_REQUIRE(TypeSizes::sizeOfU32String == sizeof(std::u32string));
    STATIC_REQUIRE(TypeSizes::sizeOfStringView == sizeof(std::string_view));
    STATIC_REQUIRE(TypeSizes::sizeOfWStringView == sizeof(std::wstring_view));
    STATIC_REQUIRE(TypeSizes::sizeOfU8StringView == sizeof(std::u8string_view));
    STATIC_REQUIRE(TypeSizes::sizeOfU16StringView == sizeof(std::u16string_view));
    STATIC_REQUIRE(TypeSizes::sizeOfU32StringView == sizeof(std::u32string_view));
}

TEST_CASE("the constexpr parentesis tokenType", "[tokenType]") {
    STATIC_REQUIRE(vnd::getBracketsType("(") == vnd::TokenType::OPEN_PARENTESIS);
    STATIC_REQUIRE(vnd::getBracketsType("[") == vnd::TokenType::OPEN_SQ_PARENTESIS);
    STATIC_REQUIRE(vnd::getBracketsType("{") == vnd::TokenType::OPEN_CUR_PARENTESIS);
    STATIC_REQUIRE(vnd::getBracketsType(")") == vnd::TokenType::CLOSE_PARENTESIS);
    STATIC_REQUIRE(vnd::getBracketsType("]") == vnd::TokenType::CLOSE_SQ_PARENTESIS);
    STATIC_REQUIRE(vnd::getBracketsType("}") == vnd::TokenType::CLOSE_CUR_PARENTESIS);

    STATIC_REQUIRE(vnd::getBracketsType("a") == vnd::TokenType::UNKNOWN);
    STATIC_REQUIRE(vnd::getBracketsType(" ") == vnd::TokenType::UNKNOWN);  // Space character
    STATIC_REQUIRE(vnd::getBracketsType("#") == vnd::TokenType::UNKNOWN);  // Unrecognized symbol
}

TEST_CASE("singleCharOp function tests", "[singleCharOp]") {
    // Test valid operators
    STATIC_REQUIRE(vnd::singoleCharOp('-') == vnd::TokenType::MINUS);
    STATIC_REQUIRE(vnd::singoleCharOp('=') == vnd::TokenType::EQUAL);
    STATIC_REQUIRE(vnd::singoleCharOp('<') == vnd::TokenType::LESS);
    STATIC_REQUIRE(vnd::singoleCharOp('>') == vnd::TokenType::GREATER);
    STATIC_REQUIRE(vnd::singoleCharOp('!') == vnd::TokenType::NOT);
    STATIC_REQUIRE(vnd::singoleCharOp('+') == vnd::TokenType::PLUS);
    STATIC_REQUIRE(vnd::singoleCharOp('*') == vnd::TokenType::STAR);
    STATIC_REQUIRE(vnd::singoleCharOp('/') == vnd::TokenType::DIVIDE);
    STATIC_REQUIRE(vnd::singoleCharOp('^') == vnd::TokenType::XOR);
    STATIC_REQUIRE(vnd::singoleCharOp('%') == vnd::TokenType::PERCENT);
    STATIC_REQUIRE(vnd::singoleCharOp('|') == vnd::TokenType::OR);
    STATIC_REQUIRE(vnd::singoleCharOp('&') == vnd::TokenType::AND);

    // Test an unknown operator
    STATIC_REQUIRE(vnd::singoleCharOp('a') == vnd::TokenType::UNKNOWN);
    STATIC_REQUIRE(vnd::singoleCharOp(' ') == vnd::TokenType::UNKNOWN);  // Space character
    STATIC_REQUIRE(vnd::singoleCharOp('#') == vnd::TokenType::UNKNOWN);  // Unrecognized symbol
}

// NOLINTEND(*-err58-cpp, *-include-cleaner, *-use-anonymous-namespace, *-function-cognitive-complexity, *-avoid-do-while)