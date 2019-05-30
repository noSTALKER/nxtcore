#include "catch.hpp"

#include "../include/RingBuffer.h"

TEST_CASE("RingBuffer Tests", "[ring_buffer]") {
    SECTION("popFront() and pushBack() tests") {
        nxt::core::RingBuffer<int> ring_buffer;
        ring_buffer.pushBack(1);

        REQUIRE(ring_buffer.size() == 1);
        REQUIRE(ring_buffer.front() == 1);
        REQUIRE(ring_buffer.capacity() >= 1);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        ring_buffer.pushBack(2);
        ring_buffer.pushBack(3);
        ring_buffer.pushBack(4);

        REQUIRE(ring_buffer.size() == 4);
        REQUIRE(ring_buffer.front() == 1);
        REQUIRE(ring_buffer.capacity() >= 4);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        ring_buffer.pushBack(5);
        ring_buffer.pushBack(6);
        ring_buffer.pushBack(7);

        REQUIRE(ring_buffer.size() == 7);
        REQUIRE(ring_buffer.front() == 1);
        REQUIRE(ring_buffer.capacity() >= 7);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        REQUIRE(ring_buffer.popFrontAndExtract() == 1);
        REQUIRE(ring_buffer.size() == 6);
        REQUIRE(ring_buffer.front() == 2);
        REQUIRE(ring_buffer.capacity() >= 6);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        ring_buffer.popFront();
        ring_buffer.popFront();

        REQUIRE(ring_buffer.size() == 4);
        REQUIRE(ring_buffer.front() == 4);
        REQUIRE(ring_buffer.capacity() >= 4);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        ring_buffer.pushBack(8);
        ring_buffer.pushBack(9);
        ring_buffer.pushBack(10);
        ring_buffer.pushBack(11);
        ring_buffer.pushBack(12);

        REQUIRE(ring_buffer.size() == 9);
        REQUIRE(ring_buffer.front() == 4);
        REQUIRE(ring_buffer.capacity() >= 9);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));
    }

    SECTION("Copy Constructor tests") {
        nxt::core::RingBuffer<int> ring_buffer;

        ring_buffer.pushBack(1);
        ring_buffer.pushBack(2);
        ring_buffer.pushBack(3);
        ring_buffer.pushBack(4);
        ring_buffer.pushBack(5);
        ring_buffer.pushBack(6);
        ring_buffer.pushBack(7);

        REQUIRE(ring_buffer.size() == 7);
        REQUIRE(ring_buffer.front() == 1);
        REQUIRE(ring_buffer.capacity() >= 7);
        REQUIRE(nxt::core::isPowerOf2(ring_buffer.capacity()));

        nxt::core::RingBuffer copy_buffer = ring_buffer;

        REQUIRE(copy_buffer.size() == 7);
        REQUIRE(copy_buffer.front() == 1);
        REQUIRE(copy_buffer.capacity() >= 7);
        REQUIRE(nxt::core::isPowerOf2(copy_buffer.capacity()));

        copy_buffer.popFront();
        copy_buffer.popFront();
        copy_buffer.popFront();

        copy_buffer.pushBack(8);
        copy_buffer.pushBack(9);
        copy_buffer.pushBack(10);
        copy_buffer.pushBack(11);
        copy_buffer.pushBack(12);

        REQUIRE(copy_buffer.size() == 9);
        REQUIRE(copy_buffer.front() == 4);
        REQUIRE(copy_buffer.capacity() >= 9);
        REQUIRE(nxt::core::isPowerOf2(copy_buffer.capacity()));

        nxt::core::RingBuffer copy_buffer_2 = copy_buffer;

        REQUIRE(copy_buffer_2.size() == 9);
        REQUIRE(copy_buffer_2.front() == 4);
        REQUIRE(copy_buffer_2.capacity() >= 9);
        REQUIRE(nxt::core::isPowerOf2(copy_buffer_2.capacity()));

        nxt::core::RingBuffer move_buffer = std::move(copy_buffer_2);

        REQUIRE(move_buffer.size() == 9);
        REQUIRE(move_buffer.front() == 4);
        REQUIRE(move_buffer.capacity() >= 9);
        REQUIRE(nxt::core::isPowerOf2(move_buffer.capacity()));

        REQUIRE(copy_buffer_2.size() == 0);
        REQUIRE(copy_buffer_2.capacity() == 0);
    }
}