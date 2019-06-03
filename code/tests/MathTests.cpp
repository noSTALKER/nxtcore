#include "catch.hpp"

#include "../include/Maths.h"

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

    SECTION("isEven tests") {
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

    SECTION("isOdd Tests") {
        STATIC_REQUIRE_FALSE(nxt::core::isOdd(2u));
        STATIC_REQUIRE_FALSE(nxt::core::isOdd(4u));
        STATIC_REQUIRE_FALSE(nxt::core::isOdd(6u));
        STATIC_REQUIRE_FALSE(nxt::core::isOdd(8u));
        STATIC_REQUIRE_FALSE(nxt::core::isOdd(10u));

        REQUIRE_FALSE(nxt::core::isOdd(2u));
        REQUIRE_FALSE(nxt::core::isOdd(4u));
        REQUIRE_FALSE(nxt::core::isOdd(6u));
        REQUIRE_FALSE(nxt::core::isOdd(8u));
        REQUIRE_FALSE(nxt::core::isOdd(10u));

        STATIC_REQUIRE(nxt::core::isOdd(1u));
        STATIC_REQUIRE(nxt::core::isOdd(3u));
        STATIC_REQUIRE(nxt::core::isOdd(5u));
        STATIC_REQUIRE(nxt::core::isOdd(7u));
        STATIC_REQUIRE(nxt::core::isOdd(9u));

        REQUIRE(nxt::core::isOdd(1u));
        REQUIRE(nxt::core::isOdd(3u));
        REQUIRE(nxt::core::isOdd(5u));
        REQUIRE(nxt::core::isOdd(7u));
        REQUIRE(nxt::core::isOdd(9u));
    }

    SECTION("getNextPowerOf2 tests") {
        REQUIRE(nxt::core::getNextPowerOf2(0u) == 1u);
        REQUIRE(nxt::core::getNextPowerOf2(1u) == 2u);
        REQUIRE(nxt::core::getNextPowerOf2(2u) == 4u);
        REQUIRE(nxt::core::getNextPowerOf2(3u) == 4u);
        REQUIRE(nxt::core::getNextPowerOf2(4u) == 8u);
        REQUIRE(nxt::core::getNextPowerOf2(5u) == 8u);
        REQUIRE(nxt::core::getNextPowerOf2(6u) == 8u);
        REQUIRE(nxt::core::getNextPowerOf2(7u) == 8u);
        REQUIRE(nxt::core::getNextPowerOf2(8u) == 16u);
        REQUIRE(nxt::core::getNextPowerOf2(9u) == 16u);
        REQUIRE(nxt::core::getNextPowerOf2(16u) == 32u);
        REQUIRE(nxt::core::getNextPowerOf2(17u) == 32u);
        REQUIRE(nxt::core::getNextPowerOf2(33u) == 64u);

        STATIC_REQUIRE(nxt::core::getNextPowerOf2(0u) == 1u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(1u) == 2u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(2u) == 4u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(3u) == 4u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(4u) == 8u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(5u) == 8u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(6u) == 8u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(7u) == 8u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(8u) == 16u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(9u) == 16u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(16u) == 32u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(17u) == 32u);
        STATIC_REQUIRE(nxt::core::getNextPowerOf2(33u) == 64u);
    }
}
