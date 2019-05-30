#include "catch.hpp"

#include "../include/PriorityQueue.h"
#include "../include/Vector.h"

TEST_CASE("PriorityQueue Tests", "[priority_queue]") {
    SECTION("Priority Queue Constructors") {
        nxt::core::Vector values = {5, 1, 2, 4, 6, 7, 8, 5, 2};

		nxt::core::PriorityQueue queue(values.begin(), values.end());

		REQUIRE(values.size() == queue.size());
    }
}