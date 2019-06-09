#include "catch.hpp"

#include "../include/Container/AVLTree.h"

TEST_CASE("AVLTree Tests", "[avl_tree]") {
    SECTION("sorting check") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::AVLTree<nxt::core::SimpleTreeTraits<int>> avl;

        for (auto value : values) {
            avl.insert(value);
        }

        REQUIRE(avl.size() == 7);

        std::size_t i = 0;
        for (const auto& value : avl) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(avl.erase(-5) == 1);
        REQUIRE(avl.erase(-10) == 0);
        REQUIRE(avl.size() == 6);
        REQUIRE(*avl.begin() == 0);

        avl.clear();

        REQUIRE(avl.size() == 0);
    }

    SECTION("more sorting check") {
        int values[] = {1, 2, 3, 4, 5, 6, 7};
        int sorted_values[] = {1, 2, 3, 4, 5, 6, 7};

        nxt::core::AVLTree<nxt::core::SimpleTreeTraits<int>> avl;

        for (auto value : values) {
            avl.insert(value);
        }

        REQUIRE(avl.size() == 7);

        std::size_t i = 0;
        for (const auto& value : avl) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(avl.erase(4) == 1);
        REQUIRE(avl.erase(-10) == 0);
        REQUIRE(avl.size() == 6);
        REQUIRE(*avl.begin() == 1);

        REQUIRE(avl.erase(1) == 1);
        REQUIRE(avl.erase(4) == 0);
        REQUIRE(avl.size() == 5);
        REQUIRE(*avl.begin() == 2);

        avl.clear();

        REQUIRE(avl.size() == 0);
    }

    SECTION("copy constructor check") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::AVLTree<nxt::core::SimpleTreeTraits<int>> avl;

        for (auto value : values) {
            avl.insert(value);
        }

        REQUIRE(avl.size() == 7);

        std::size_t i = 0;
        for (const auto& value : avl) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        auto copy_avl = avl;

        REQUIRE(copy_avl.size() == 7);

        i = 0;
        for (const auto& value : copy_avl) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }
    }
}