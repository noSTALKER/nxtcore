#include "catch2.h"

#include "../include/RingBuffer.h"

TEST_CASE("RingBuffer Tests", "[ring_buffer]") {
    SECTION("capacity increase check") {
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
}