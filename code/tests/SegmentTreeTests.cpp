#include "catch.hpp"

#include "../include/Container/SegmentTree.h"

TEST_CASE("Segment Tree Tests", "[segment_tree]") {
    SECTION("Segment Tree Tests") {
        nxt::core::SegmentTree<int, std::plus<>> segment_tree = {1, 1, 1, 1, 1};

        REQUIRE(segment_tree.size() == 9);
        REQUIRE(segment_tree.inputSize() == 5);
        REQUIRE(segment_tree.result() == 5);

        segment_tree.update(0, 2);
        REQUIRE(segment_tree.result() == 6);

        segment_tree.update(2, 2);
        REQUIRE(segment_tree.result() == 7);

        REQUIRE(segment_tree.query(0, 0) == 2);
        REQUIRE(segment_tree.query(0, 1) == 3);
        REQUIRE(segment_tree.query(0, 2) == 5);
        REQUIRE(segment_tree.query(0, 3) == 6);
        REQUIRE(segment_tree.query(0, 4) == 7);
        REQUIRE(segment_tree.query(1, 3) == 4);
        REQUIRE(segment_tree.query(2, 3) == 3);
        REQUIRE(segment_tree.query(3, 3) == 1);
        REQUIRE(segment_tree.query(2, 4) == 4);
        REQUIRE(segment_tree.query(3, 4) == 2);
        REQUIRE(segment_tree.query(4, 4) == 1);

    }
}
