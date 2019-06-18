#include "catch.hpp"

#include "../include/Container/SkipList.h"
#include "../include/Container/CommonTree.h"

TEST_CASE("SkipList Tests", "[skip_list]") {
    SECTION("sorting check") {
        int values[] = {5, 8, 0, 1, 12, -5, 6};
        int sorted_values[] = {-5, 0, 1, 5, 6, 8, 12};

        nxt::core::SkipList<nxt::core::SimpleTraits<int>> skip_list;

        for (auto value : values) {
            skip_list.insert(value);
        }

        REQUIRE(skip_list.size() == 7);

        std::size_t i = 0;
        for (const auto& value : skip_list) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(skip_list.erase(-5) == 1);
        REQUIRE(skip_list.erase(-10) == 0);
        REQUIRE(skip_list.size() == 6);
        REQUIRE(*skip_list.begin() == 0);

        skip_list.clear();

        REQUIRE(skip_list.size() == 0);
    }

    SECTION("more sorting check") {
        int values[] = {1, 2, 3, 4, 5, 6, 7};
        int sorted_values[] = {1, 2, 3, 4, 5, 6, 7};

        nxt::core::SkipList<nxt::core::SimpleTraits<int>> skip_list;

        for (auto value : values) {
            skip_list.insert(value);
        }

        REQUIRE(skip_list.size() == 7);

        std::size_t i = 0;
        for (const auto& value : skip_list) {
            REQUIRE(value == sorted_values[i]);
            ++i;
        }

        REQUIRE(skip_list.erase(4) == 1);
        REQUIRE(skip_list.erase(-10) == 0);
        REQUIRE(skip_list.size() == 6);
        REQUIRE(*skip_list.begin() == 1);

        REQUIRE(skip_list.erase(1) == 1);
        REQUIRE(skip_list.erase(4) == 0);
        REQUIRE(skip_list.size() == 5);
        REQUIRE(*skip_list.begin() == 2);

        skip_list.clear();

        REQUIRE(skip_list.size() == 0);
    }

}
