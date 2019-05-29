#include "catch2.h"

#include "../include/GenericTask.h"
#include "../include/TaskQueue.h"
#include "../include/Task.h"

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

        auto task_1 = nxt::core::makeGenericTask(task, values, 0, 6);
        auto task_2 = nxt::core::makeGenericTask(task, values, 6, 11);

        task_1->execute();
        task_2->execute();

        REQUIRE(task_1->getResult() == 10);
        REQUIRE(task_2->getResult() == 34);

    }
}
