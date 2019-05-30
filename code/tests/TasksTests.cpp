#include "catch.hpp"

#include "../include/GenericTask.h"
#include "../include/Task.h"
#include "../include/TaskQueue.h"

TEST_CASE("Task System tests", "[task]") {
    nxt::core::TaskQueue task_queue("test_queue");

    SECTION("Generic Task Tests") {
        int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        auto task = [](int* values, int start_index, int end_index) {
            int sum = 0;
            for (size_t i = start_index; i < end_index; ++i) {
                sum += values[i];
            }
            return sum;
        };

        std::atomic<int> sum = 0;
        auto void_task = [&sum](int* values, int start_index, int end_index) {
            int counter = 0;
            for (size_t i = start_index; i < end_index; ++i) {
                counter += values[i];
            }
            sum.fetch_add(counter);
        };

        auto task_1 = nxt::core::makeGenericTask(task, values, 0, 5);
        auto task_2 = nxt::core::makeGenericTask(task, values, 5, 10);

        task_queue.start();

        int values_1[100];
        std::fill(std::begin(values_1), std::end(values_1), 1);

        auto task_3 = nxt::core::makeGenericTask(task, values_1, 0, 10);
        auto task_4 = nxt::core::makeGenericTask(task, values_1, 10, 20);
        auto task_5 = nxt::core::makeGenericTask(task, values_1, 20, 30);
        auto task_6 = nxt::core::makeGenericTask(task, values_1, 30, 40);
        auto task_7 = nxt::core::makeGenericTask(task, values_1, 40, 50);
        auto task_8 = nxt::core::makeGenericTask(task, values_1, 50, 60);
        auto task_9 = nxt::core::makeGenericTask(task, values_1, 60, 70);
        auto task_10 = nxt::core::makeGenericTask(task, values_1, 70, 80);
        auto task_11 = nxt::core::makeGenericTask(task, values_1, 80, 90);
        auto task_12 = nxt::core::makeGenericTask(task, values_1, 90, 100);

        task_queue.addTask(task_1);
        task_queue.addTask(task_2);
        task_queue.addTask(task_3);
        task_queue.addTask(task_4);
        task_queue.addTask(task_5);
        task_queue.addTask(task_6);
        task_queue.addTask(task_7);
        task_queue.addTask(task_8);
        task_queue.addTask(task_9);
        task_queue.addTask(task_10);
        task_queue.addTask(task_11);
        task_queue.addTask(task_12);

        REQUIRE(task_1->getResult() == 10);
        REQUIRE(task_2->getResult() == 35);
        REQUIRE(task_3->getResult() == 10);
        REQUIRE(task_4->getResult() == 10);
        REQUIRE(task_5->getResult() == 10);
        REQUIRE(task_6->getResult() == 10);
        REQUIRE(task_7->getResult() == 10);
        REQUIRE(task_8->getResult() == 10);
        REQUIRE(task_9->getResult() == 10);
        REQUIRE(task_10->getResult() == 10);
        REQUIRE(task_11->getResult() == 10);
        REQUIRE(task_12->getResult() == 10);

        auto void_task_1 = nxt::core::makeGenericTask(void_task, values, 0, 5);
        auto void_task_2 = nxt::core::makeGenericTask(void_task, values, 5, 10);

        task_queue.addTask(void_task_1);
        task_queue.addTask(void_task_2);

        void_task_1->wait();
        void_task_2->wait();

        REQUIRE(sum == 45);
    }
}
