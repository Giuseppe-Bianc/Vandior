#include <catch2/catch_test_macros.hpp>

#include <Vandior/vandior.hpp>

TEST_CASE("the constexpr size of types", "[TypeSizes]") {
    STATIC_REQUIRE(TypeSizes::sizeOfBool == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfByte == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfIntPtr == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfUintPtr == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfInt8T == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfInt16T == 2);
    STATIC_REQUIRE(TypeSizes::sizeOfInt32T == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfInt64T == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfUint8T == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfUint16T == 2);
    STATIC_REQUIRE(TypeSizes::sizeOfUint32T == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfUint64T == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfPtrdiffT == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfDivT == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfLdivT == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfChar == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfChar16T == 2);
    STATIC_REQUIRE(TypeSizes::sizeOfChar32T == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfChar8T == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfDouble == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfFloat == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfInt == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfLong == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfLongDouble == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfLongInt == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfLongLong == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfLongLongInt == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfShort == 2);
    STATIC_REQUIRE(TypeSizes::sizeOfShortInt == 2);
    STATIC_REQUIRE(TypeSizes::sizeOfUChar == 1);
    STATIC_REQUIRE(TypeSizes::sizeOfUInt == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfULong == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfULongInt == 4);
    STATIC_REQUIRE(TypeSizes::sizeOfULongLong == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfULongLongInt == 8);
    STATIC_REQUIRE(TypeSizes::sizeOfString == 40);
    STATIC_REQUIRE(TypeSizes::sizeOfWString == 40);
    STATIC_REQUIRE(TypeSizes::sizeOfU8String == 40);
    STATIC_REQUIRE(TypeSizes::sizeOfU16String == 40);
    STATIC_REQUIRE(TypeSizes::sizeOfU32String == 40);
    STATIC_REQUIRE(TypeSizes::sizeOfStringView == 16);
    STATIC_REQUIRE(TypeSizes::sizeOfWStringView == 16);
    STATIC_REQUIRE(TypeSizes::sizeOfU8StringView == 16);
    STATIC_REQUIRE(TypeSizes::sizeOfU16StringView == 16);
    STATIC_REQUIRE(TypeSizes::sizeOfU32StringView == 16);
}
