#include "catch2.h"

#include "../include/Sort.h"

TEST_CASE("Palindrome Test", "[sort]") {
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
}