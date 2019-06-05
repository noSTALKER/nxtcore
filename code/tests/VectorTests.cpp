#include "catch.hpp"

#include "../include/Vector.h"

TEST_CASE("Vector Tests", "[vector]") {
    SECTION("sizeof test for vector") {
        STATIC_REQUIRE(sizeof(nxt::core::Vector<int>) == 32);
    }

    SECTION("Creating empty Vector") {
        nxt::core::Vector<int> vector;

        REQUIRE(vector.size() == 0);
        REQUIRE(vector.capacity() == 0);
    }

    SECTION("Creating Vector with value initialization") {
        nxt::core::Vector<int> vector(10);

        REQUIRE(vector.size() == 10);
        REQUIRE(vector.capacity() >= 10);

        // check if everything is initialized to zero
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

    SECTION("Inserting Items") {
        nxt::core::Vector<int> vector;

        vector.insert(vector.end(), 0);

        REQUIRE(vector.size() == 1);
        REQUIRE(vector[0] == 0);

        vector.insert(vector.begin(), -2);

        REQUIRE(vector.size() == 2);
        REQUIRE(vector[0] == -2);

        vector.insert(vector.begin() + 1, -1);

        REQUIRE(vector.size() == 3);
        REQUIRE(vector[0] == -2);
        REQUIRE(vector[1] == -1);

        vector.insert(vector.end(), 7);
        REQUIRE(vector.size() == 4);
        REQUIRE(vector[3] == 7);

        vector.insert(vector.begin() + 3, 6);
        REQUIRE(vector.size() == 5);
        REQUIRE(vector[3] == 6);

        vector.insert(vector.begin() + 3, 1);
        REQUIRE(vector.size() == 6);
        REQUIRE(vector[3] == 1);

        vector.insert(vector.begin() + 4, 2);
        REQUIRE(vector.size() == 7);
        REQUIRE(vector[4] == 2);

        vector.insert(vector.begin() + 5, 3);
        REQUIRE(vector.size() == 8);
        REQUIRE(vector[5] == 3);

        vector.insert(vector.begin() + 6, 4);
        REQUIRE(vector.size() == 9);
        REQUIRE(vector[6] == 4);
    }

    SECTION("Erasing Items") {
        nxt::core::Vector<int> vector = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(vector.size() == 10);

        vector.erase(vector.begin());
        REQUIRE(vector.size() == 9);
        REQUIRE(vector[0] == 0);

        auto next_value = vector.erase(vector.begin() + 1, vector.begin() + 4);
        REQUIRE(vector.size() == 6);
        REQUIRE(*next_value == 4);

        vector.erase(vector.begin(), vector.end());
        REQUIRE(vector.size() == 0);
    }

    SECTION("Comparision operators tests") {
        nxt::core::Vector<int> vector_1 = {1, 2, 3, 4, 5};
        nxt::core::Vector<int> vector_2 = {1, 2, 3, 4, 5};
        nxt::core::Vector<int> vector_3 = {0, 1, 2, 3, 4};
        nxt::core::Vector<int> vector_4 = {0, 1, 2, 3, 4, 5};

        REQUIRE(vector_1 == vector_2);
        REQUIRE(vector_1 != vector_3);
        REQUIRE(vector_1 != vector_4);

        REQUIRE_FALSE(vector_1 != vector_2);
        REQUIRE_FALSE(vector_1 == vector_3);
        REQUIRE_FALSE(vector_1 == vector_4);

        REQUIRE_FALSE(vector_1 < vector_2);
        REQUIRE_FALSE(vector_1 < vector_3);
        REQUIRE_FALSE(vector_1 < vector_4);

        REQUIRE(vector_1 <= vector_2);
        REQUIRE_FALSE(vector_1 <= vector_3);
        REQUIRE_FALSE(vector_1 <= vector_4);

        REQUIRE_FALSE(vector_1 > vector_2);
        REQUIRE(vector_1 > vector_3);
        REQUIRE(vector_1 > vector_4);

        REQUIRE(vector_1 >= vector_2);
        REQUIRE(vector_1 >= vector_3);
        REQUIRE(vector_1 >= vector_4);
    }

    SECTION("Copy Constructor Test") {
        nxt::core::Vector vector = {0, 1, 2, 3, 4};
        nxt::core::Vector copy_vector = vector;

        REQUIRE(vector == copy_vector);
    }
}