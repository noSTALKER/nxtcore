#include "catch.hpp"

#include "../include/List.h"
#include "../include/Vector.h"
#include "../include/Sort.h"

TEST_CASE("List Tests", "[priority_queue]") {
    SECTION("List Constructors") {
        nxt::core::Vector values = {5, 1, 2, 4, 6, 7, 8, 5, 2};

        nxt::core::List list(values.begin(), values.end());

        REQUIRE(values.size() == list.size());
        REQUIRE(list.front() == 5);
        REQUIRE(list.back() == 2);
        REQUIRE(nxt::core::isEqual(values.begin(), values.end(), list.begin(), list.end()));
        
    }

    SECTION("List Insert and Erase Operations") {
        nxt::core::List<int> list;

        REQUIRE(list.size() == 0);
        REQUIRE(list.empty());

        list.pushBack(1);
        REQUIRE(list.size() == 1);
        REQUIRE(list.front() == 1);
        REQUIRE(list.back() == 1);

        list.pushBack(2);
        REQUIRE(list.size() == 2);
        REQUIRE(list.front() == 1);
        REQUIRE(list.back() == 2);

        list.pushFront(0);
        REQUIRE(list.size() == 3);
        REQUIRE(list.front() == 0);
        REQUIRE(list.back() == 2);
    }
}
