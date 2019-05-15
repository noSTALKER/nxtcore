#include "catch2.h"

#include "../include/Math.h"

TEST_CASE("Math Tests", "[math]") {
    SECTION("Power of 2 Test") {
        STATIC_REQUIRE(nxt::core::isPowerOf2(1u));
        STATIC_REQUIRE(nxt::core::isPowerOf2(2u));
        STATIC_REQUIRE(nxt::core::isPowerOf2(4u));
        STATIC_REQUIRE(nxt::core::isPowerOf2(8u));
        STATIC_REQUIRE(nxt::core::isPowerOf2(16u));

        REQUIRE(nxt::core::isPowerOf2(1u));
        REQUIRE(nxt::core::isPowerOf2(2u));
        REQUIRE(nxt::core::isPowerOf2(4u));
        REQUIRE(nxt::core::isPowerOf2(8u));
        REQUIRE(nxt::core::isPowerOf2(16u));

        STATIC_REQUIRE_FALSE(nxt::core::isPowerOf2(0u));
        STATIC_REQUIRE_FALSE(nxt::core::isPowerOf2(3u));
        STATIC_REQUIRE_FALSE(nxt::core::isPowerOf2(6u));
        STATIC_REQUIRE_FALSE(nxt::core::isPowerOf2(10u));
        STATIC_REQUIRE_FALSE(nxt::core::isPowerOf2(30u));

        REQUIRE_FALSE(nxt::core::isPowerOf2(0u));
        REQUIRE_FALSE(nxt::core::isPowerOf2(3u));
        REQUIRE_FALSE(nxt::core::isPowerOf2(6u));
        REQUIRE_FALSE(nxt::core::isPowerOf2(10u));
        REQUIRE_FALSE(nxt::core::isPowerOf2(30u));
    }

	SECTION("IsEven Test") {
        STATIC_REQUIRE(nxt::core::isEven(2u));
        STATIC_REQUIRE(nxt::core::isEven(4u));
        STATIC_REQUIRE(nxt::core::isEven(6u));
        STATIC_REQUIRE(nxt::core::isEven(8u));
        STATIC_REQUIRE(nxt::core::isEven(10u));

        REQUIRE(nxt::core::isEven(2u));
        REQUIRE(nxt::core::isEven(4u));
        REQUIRE(nxt::core::isEven(6u));
        REQUIRE(nxt::core::isEven(8u));
        REQUIRE(nxt::core::isEven(10u));

        STATIC_REQUIRE_FALSE(nxt::core::isEven(1u));
        STATIC_REQUIRE_FALSE(nxt::core::isEven(3u));
        STATIC_REQUIRE_FALSE(nxt::core::isEven(5u));
        STATIC_REQUIRE_FALSE(nxt::core::isEven(7u));
        STATIC_REQUIRE_FALSE(nxt::core::isEven(9u));

        REQUIRE_FALSE(nxt::core::isEven(1u));
        REQUIRE_FALSE(nxt::core::isEven(3u));
        REQUIRE_FALSE(nxt::core::isEven(5u));
        REQUIRE_FALSE(nxt::core::isEven(7u));
        REQUIRE_FALSE(nxt::core::isEven(9u));

	}
}