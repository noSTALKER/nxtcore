#include "catch2.h"

#include "../include/AVLTree.h"

TEST_CASE("AVL Tests", "[avl_tree]") {
    SECTION("sorting check") {
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

        avl.clear();

        REQUIRE(avl.size() == 0);
    }
}