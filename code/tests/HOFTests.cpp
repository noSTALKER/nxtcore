#include "catch2.h"

#include "../include/HOF.h"

TEST_CASE("HOF Tests", "[hof]") {
    SECTION("equalsTo Tests") {
        auto int_equals_to_0 = nxt::core::equalsTo(0);

        REQUIRE(int_equals_to_0(0));
        REQUIRE_FALSE(int_equals_to_0(-1));
        REQUIRE_FALSE(int_equals_to_0(1));
    }

    SECTION("notEqualsTo Tests") {
        auto int_not_equals_to_0 = nxt::core::notEqualsTo(0);

        REQUIRE_FALSE(int_not_equals_to_0(0));
        REQUIRE(int_not_equals_to_0(-1));
        REQUIRE(int_not_equals_to_0(1));
    }

    SECTION("lessThan Tests") {
        auto int_less_than_0 = nxt::core::lessThan(0);

        REQUIRE_FALSE(int_less_than_0(0));
        REQUIRE_FALSE(int_less_than_0(1));
        REQUIRE(int_less_than_0(-1));
    }

    SECTION("greaterThan Tests") {
        auto int_greater_than_0 = nxt::core::greaterThan(0);

        REQUIRE_FALSE(int_greater_than_0(0));
        REQUIRE_FALSE(int_greater_than_0(-1));
        REQUIRE(int_greater_than_0(1));
    }
}
