#include "catch.hpp"

#include "../include/Algorithm/Sort.h"
#include "../include/Container/Array.h"

TEST_CASE("Permutation Tests", "[sort]") {
    SECTION("Permutation Tests") {
        std::string values("abcde");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abced");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abdce");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abdec");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abecd");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abedc");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "acbde");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abedc");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abecd");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abdec");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abdce");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abced");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "abcde");

        REQUIRE_FALSE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "edcba");

        REQUIRE_FALSE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "abcde");
    }

    SECTION("Permutation Tests with duplicate values") {
        std::string values("11112");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "11121");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "11211");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "12111");

        REQUIRE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "21111");

        REQUIRE_FALSE(nxt::core::nextPermutation(values.begin(), values.end()));
        REQUIRE(values == "11112");

        values = "21111";
        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "12111");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "11211");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "11121");

        REQUIRE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "11112");

        REQUIRE_FALSE(nxt::core::previousPermutation(values.begin(), values.end()));
        REQUIRE(values == "21111");
    }

    SECTION("Corner cases for nextPermutation") {
        std::string values("11111");

        REQUIRE_FALSE(nxt::core::nextPermutation(values.begin(), values.end()));

        values = "1";
        REQUIRE_FALSE(nxt::core::nextPermutation(values.begin(), values.end()));

        values.clear();
        REQUIRE_FALSE(nxt::core::nextPermutation(values.begin(), values.end()));

    }

    SECTION("Is Permutation Tests") {
        std::string value1("abcde"), value2("adbce");

        REQUIRE(nxt::core::isPermutation(value1.begin(), value1.end(), value2.begin(), value2.end()));
    }
}
