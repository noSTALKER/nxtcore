#include "catch.hpp"

#include "../include/Container/BinarySearchTree.h"

TEST_CASE("BinarySeachTree Tests", "[binary_search_tree]") {
    SECTION("sorting check") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::BinarySearchTree<nxt::core::SimpleTraits<int>> bst;

        for (auto value : values) {
            bst.insert(value);
        }

        REQUIRE(bst.size() == 7);

        std::size_t i = 0;
        for (const auto& value : bst) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(bst.erase(-5) == 1);
        REQUIRE(bst.erase(-10) == 0);
        REQUIRE(bst.size() == 6);
        REQUIRE(*bst.begin() == 0);

		bst.clear();

		REQUIRE(bst.size() == 0);

    }

    SECTION("sorting check") {
        int values[] = {1, 2, 3, 4, 5, 6, 7};
        int sorted_values[] = {1, 2, 3, 4, 5, 6, 7};

        nxt::core::BinarySearchTree<nxt::core::SimpleTraits<int>> bst;

        for (auto value : values) {
            bst.insert(value);
        }

        REQUIRE(bst.size() == 7);

        std::size_t i = 0;
        for (const auto& value : bst) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(bst.erase(4) == 1);
        REQUIRE(bst.erase(-10) == 0);
        REQUIRE(bst.size() == 6);
        REQUIRE(*bst.begin() == 1);

        REQUIRE(bst.erase(1) == 1);
        REQUIRE(bst.erase(4) == 0);
        REQUIRE(bst.size() == 5);
        REQUIRE(*bst.begin() == 2);

        bst.clear();

        REQUIRE(bst.size() == 0);
    }

    SECTION("copy constructor test") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::BinarySearchTree<nxt::core::SimpleTraits<int>> bst;

        for (auto value : values) {
            bst.insert(value);
        }

        REQUIRE(bst.size() == 7);

        std::size_t i = 0;
        for (const auto& value : bst) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        auto copy_bst = bst;

        REQUIRE(copy_bst.size() == 7);
        i = 0;
        for (const auto& value : copy_bst) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }
    }
}