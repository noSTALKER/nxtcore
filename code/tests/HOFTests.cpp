#include "catch.hpp"

#include "../include/Algorithm/HOF.h"
#include "../include/Maths.h"

TEST_CASE("HOF Tests", "[hof]") {
    SECTION("equalTo Tests") {
        auto int_equal_to_0 = nxt::core::equalTo(0);

        REQUIRE(int_equal_to_0(0));
        REQUIRE_FALSE(int_equal_to_0(-1));
        REQUIRE_FALSE(int_equal_to_0(1));
    }

    SECTION("notEqualTo Tests") {
        auto int_not_equal_to_0 = nxt::core::notEqualTo(0);

        REQUIRE_FALSE(int_not_equal_to_0(0));
        REQUIRE(int_not_equal_to_0(-1));
        REQUIRE(int_not_equal_to_0(1));
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

    SECTION("whenAll Tests") {
        auto when_all =
            nxt::core::whenAll(nxt::core::isEven<uint32_t>, nxt::core::lessThan(10u), nxt::core::greaterThan(2u));

        REQUIRE(when_all(4));
        REQUIRE(when_all(6));
        REQUIRE(when_all(8));

        REQUIRE_FALSE(when_all(0));
        REQUIRE_FALSE(when_all(2));
        REQUIRE_FALSE(when_all(3));
        REQUIRE_FALSE(when_all(10));
        REQUIRE_FALSE(when_all(11));
    }

    SECTION("whenNone Tests") {
        auto when_none =
            nxt::core::whenNone(nxt::core::isEven<uint32_t>, nxt::core::lessThan(10u), nxt::core::greaterThan(12u));

        REQUIRE_FALSE(when_none(4));
        REQUIRE_FALSE(when_none(6));
        REQUIRE_FALSE(when_none(8));
        REQUIRE_FALSE(when_none(0));
        REQUIRE_FALSE(when_none(2));
        REQUIRE_FALSE(when_none(3));
        REQUIRE_FALSE(when_none(10));

        REQUIRE(when_none(11));
    }

    SECTION("whenAny Tests") {
        auto when_any =
            nxt::core::whenAny(nxt::core::isEven<uint32_t>, nxt::core::isPowerOf2<uint32_t>, nxt::core::lessThan(10u));

        REQUIRE(when_any(0));
        REQUIRE(when_any(2));
        REQUIRE(when_any(3));
        REQUIRE(when_any(4));
        REQUIRE(when_any(6));
        REQUIRE(when_any(8));
        REQUIRE(when_any(10));
        REQUIRE(when_any(16));

        REQUIRE_FALSE(when_any(11));
        REQUIRE_FALSE(when_any(13));
        REQUIRE_FALSE(when_any(15));
    }

    SECTION("compose Tests") {
        auto single_compose = nxt::core::compose(nxt::core::getNextPowerOf2<uint32_t>);

        REQUIRE(single_compose(2u) == 4);
        REQUIRE(single_compose(3u) == 4);
        REQUIRE(single_compose(4u) == 8);
        REQUIRE(single_compose(5u) == 8);

        auto multiple_compose =
            nxt::core::compose(nxt::core::isPowerOf2<uint32_t>, nxt::core::getNextPowerOf2<uint32_t>);

        REQUIRE(multiple_compose(2u));
        REQUIRE(multiple_compose(3u));
        REQUIRE(multiple_compose(4u));
        REQUIRE(multiple_compose(5u));

        auto compose_including_when_all =
            nxt::core::compose(nxt::core::whenAll(nxt::core::isEven<uint32_t>, nxt::core::lessThan(10u)),
                               nxt::core::getNextPowerOf2<uint32_t>);

        REQUIRE(compose_including_when_all(2u));
        REQUIRE(compose_including_when_all(3u));
        REQUIRE(compose_including_when_all(4u));
        REQUIRE(compose_including_when_all(5u));

        REQUIRE_FALSE(compose_including_when_all(8u));
        REQUIRE_FALSE(compose_including_when_all(9u));
        REQUIRE_FALSE(compose_including_when_all(10u));

        auto increment = [](const auto& value) { return value + 1; };
        auto compose_including_compose =
            nxt::core::compose(nxt::core::compose(increment, nxt::core::getNextPowerOf2<uint32_t>),
                               increment,
                               nxt::core::getNextPowerOf2<uint32_t>);

        REQUIRE(compose_including_compose(1u) == 5);
        REQUIRE(compose_including_compose(2u) == 9);
        REQUIRE(compose_including_compose(3u) == 9);
        REQUIRE(compose_including_compose(4u) == 17);
        REQUIRE(compose_including_compose(5u) == 17);
    }
}
