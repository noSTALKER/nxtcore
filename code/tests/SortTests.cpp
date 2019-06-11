#include "catch.hpp"

#include "../include/Algorithm/Sort.h"

TEST_CASE("Sort Test", "[sort]") {
    SECTION("isPalindrome Tests") {
        int true_case_1[] = {0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0};
        int true_case_2[] = {0};
        int true_case_3[] = {0, 1, 2, 3, 4, 4, 3, 2, 1, 0};

        int false_case_1[] = {0, 1, 2, 3, 2, 2, 1, 0};
        int false_case_2[] = {0, 1, 3, 4};

        REQUIRE(nxt::core::isPalindrome(std::begin(true_case_1), std::end(true_case_1)));
        REQUIRE(nxt::core::isPalindrome(std::begin(true_case_2), std::end(true_case_2)));
        REQUIRE(nxt::core::isPalindrome(std::begin(true_case_3), std::end(true_case_3)));

        REQUIRE_FALSE(nxt::core::isPalindrome(std::begin(false_case_1), std::end(false_case_1)));
        REQUIRE_FALSE(nxt::core::isPalindrome(std::begin(false_case_2), std::end(false_case_2)));
    }

    SECTION("shiftLeft Tests") {
        int array_1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        nxt::core::shiftLeft(std::begin(array_1), std::end(array_1), 0);

        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::shiftLeft(std::begin(array_1), std::end(array_1), 10);

        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::shiftLeft(std::begin(array_1), std::end(array_1), 4);
        REQUIRE(array_1[0] == 4);
        REQUIRE(array_1[5] == 9);
    }

    SECTION("shiftRight Tests") {
        int array_1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        nxt::core::shiftRight(std::begin(array_1), std::end(array_1), 0);

        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::shiftRight(std::begin(array_1), std::end(array_1), 10);

        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::shiftRight(std::begin(array_1), std::end(array_1), 4);
        REQUIRE(array_1[4] == 0);
        REQUIRE(array_1[9] == 5);
    }

    SECTION("rotateLeft Tests") {
        int array_1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        nxt::core::rotateLeft(std::begin(array_1), std::begin(array_1), std::end(array_1));
        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::rotateLeft(std::begin(array_1), std::end(array_1), std::end(array_1));
        REQUIRE(array_1[0] == 0);
        REQUIRE(array_1[9] == 9);

        nxt::core::rotateLeft(std::begin(array_1), std::begin(array_1) + 3, std::end(array_1));
        REQUIRE(array_1[0] == 3);
        REQUIRE(array_1[6] == 9);
        REQUIRE(array_1[7] == 0);
        REQUIRE(array_1[9] == 2);
    }
}


