#include "catch2.h"

#include "../include/Vector.h"

TEST_CASE("Vector Tests", "[vector]") {
    SECTION("Creating empty Vector") {
        nxt::core::Vector<int> vector;

        REQUIRE(vector.size() == 0);
        REQUIRE(vector.capacity() == 0);
    }

    SECTION("Creating Vector with value initialization") {
        nxt::core::Vector<int> vector(10);

        REQUIRE(vector.size() == 10);
        REQUIRE(vector.capacity() >= 10);
        
		//check if everything is initialized to zero
        bool is_zero = true;
        for (auto value : vector) {
            if (value != 0)
                is_zero = false;
        }

		REQUIRE(is_zero);
    }

	SECTION("Creating Vector with value initialization") {
        constexpr int value = 100;
        nxt::core::Vector<int> vector(20, value);

        REQUIRE(vector.size() == 20);
        REQUIRE(vector.capacity() >= 20);

        // check if everything is initialized to zero
        bool is_same = true;
        for (auto item : vector) {
            if (item != value)
                is_same = false;
        }

        REQUIRE(is_same);
    }
}