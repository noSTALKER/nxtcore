#include "catch.hpp"

#include <thread>

#include "../include/Util/StopWatch.h"

using namespace std::chrono_literals;


TEST_CASE("StopWatch Tests", "[stop_watch]") {
    SECTION("StopWatch Tests") {
        std::string watch_name = "Test Watch";
        nxt::core::StopWatch test_watch(watch_name);

        REQUIRE_FALSE(test_watch.isRunning());
        REQUIRE(test_watch.getName() == watch_name);
        REQUIRE(test_watch.getDuration<std::chrono::nanoseconds>().count() == 0);

        test_watch.start();

        REQUIRE(test_watch.isRunning());
        std::this_thread::sleep_for(1000ns);

        test_watch.stop();

        auto first_duration = test_watch.getDuration<std::chrono::nanoseconds>().count();
        REQUIRE(first_duration > 0);
        REQUIRE_FALSE(test_watch.isRunning());

        test_watch.start();

        REQUIRE(test_watch.isRunning());
        std::this_thread::sleep_for(1000ns);

        test_watch.stop();

        auto second_duration = test_watch.getDuration<std::chrono::nanoseconds>().count();
        REQUIRE(second_duration > first_duration);
        REQUIRE_FALSE(test_watch.isRunning());

        test_watch.reset();

        REQUIRE_FALSE(test_watch.isRunning());
        REQUIRE(test_watch.getDuration<std::chrono::nanoseconds>().count() == 0);

    }
}
