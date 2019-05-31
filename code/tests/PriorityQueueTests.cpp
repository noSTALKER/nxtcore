#include "catch.hpp"

#include "../include/PriorityQueue.h"
#include "../include/Vector.h"

TEST_CASE("PriorityQueue Tests", "[priority_queue]") {
    SECTION("Priority Queue Push and Pop Tests") {
        nxt::core::Vector values = {5, 1, 2, 4, 6, 7, 8, 5, 2};

        nxt::core::PriorityQueue queue(values.begin(), values.end());

        REQUIRE(values.size() == queue.size());
        REQUIRE(queue.top() == 8);

        REQUIRE(queue.popAndExtract() == 8);
        REQUIRE(queue.size() == 8);
        REQUIRE(queue.top() == 7);

        REQUIRE(queue.popAndExtract() == 7);
        REQUIRE(queue.size() == 7);
        REQUIRE(queue.top() == 6);

        REQUIRE(queue.popAndExtract() == 6);
        REQUIRE(queue.size() == 6);
        REQUIRE(queue.top() == 5);

        REQUIRE(queue.popAndExtract() == 5);
        REQUIRE(queue.size() == 5);
        REQUIRE(queue.top() == 5);

        queue.push(0);
        REQUIRE(queue.size() == 6);
        REQUIRE(queue.top() == 5);

        queue.push(1);
        REQUIRE(queue.size() == 7);
        REQUIRE(queue.top() == 5);

        queue.push(5);
        REQUIRE(queue.size() == 8);
        REQUIRE(queue.top() == 5);

        queue.emplace(6);
        REQUIRE(queue.size() == 9);
        REQUIRE(queue.top() == 6);

        REQUIRE(queue.popAndExtract() == 6);
        REQUIRE(queue.size() == 8);
        REQUIRE(queue.top() == 5);

        REQUIRE(queue.popAndExtract() == 5);
        REQUIRE(queue.size() == 7);
        REQUIRE(queue.top() == 5);

        REQUIRE(queue.popAndExtract() == 5);
        REQUIRE(queue.size() == 6);
        REQUIRE(queue.top() == 4);

        REQUIRE(queue.popAndExtract() == 4);
        REQUIRE(queue.size() == 5);
        REQUIRE(queue.top() == 2);

        REQUIRE(queue.popAndExtract() == 2);
        REQUIRE(queue.size() == 4);
        REQUIRE(queue.top() == 2);

        REQUIRE(queue.popAndExtract() == 2);
        REQUIRE(queue.size() == 3);
        REQUIRE(queue.top() == 1);

        REQUIRE(queue.popAndExtract() == 1);
        REQUIRE(queue.size() == 2);
        REQUIRE(queue.top() == 1);

        REQUIRE(queue.popAndExtract() == 1);
        REQUIRE(queue.size() == 1);
        REQUIRE(queue.top() == 0);

        REQUIRE(queue.popAndExtract() == 0);
        REQUIRE(queue.size() == 0);
    }
}