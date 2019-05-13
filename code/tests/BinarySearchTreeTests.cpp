#include "catch2.h"

#include "../include/BinarySearchTree.h"

TEST_CASE("BinarySeachTree Tests", "[binary_search_tree]") {
    SECTION("sorting check") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::BinarySearchTree<nxt::core::SimpleTreeTraits<int>> bst;

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

    }
}