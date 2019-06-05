#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>

#include "Task.h"
#include "Vector.h"
#include "Export.h"
#include "Queue.h"

namespace nxt::core {
/**
 * @brief
 *
 */
class TaskQueue {
public:
    /**
     * @brief Construct a new Task Queue object
     *
     * @param name
     */
    NXTCORE_DLL_EXPORT TaskQueue(const std::string& name);

    /**
     * @brief Get the Name object
     *
     * @return const std::string&
     */
    const std::string& getName() const noexcept;

    /**
     * @brief Set the Max Threads object
     *
     * @param num_of_threads
     */
    void setMaxThreads(uint32_t num_of_threads);

    /**
     * @brief Get the Max Threads object
     *
     * @return unsigned int
     */
    uint32_t getMaxThreads() const noexcept;

    /**
     * @brief
     *
     * @param task
     * @return true
     * @return false
     */
    bool addTask(const std::shared_ptr<Task>& task);

    std::size_t addTasks(const Vector<std::shared_ptr<Task>>& task);

    ///**
    // * @brief
    // *
    // * @param tasks
    // * @return unsigned int
    // */
    //std::size_t addTasks(const Vector<std::shared_ptr<Task>>& tasks);

    /**
     * @brief
     *
     */
    void start();

    /**
     * @brief
     *
     */
    void stop();

    ~TaskQueue();

private:
    void spawnWorkerThreads();
    void workerLogic();

    std::string name_;
    Queue<std::shared_ptr<Task>> tasks_;
    std::atomic<uint32_t> current_threads_;
    uint32_t max_threads_;
    std::atomic<bool> queue_running_;
    std::mutex mutex_;
    std::condition_variable conditional_variable_;
    std::unordered_map<std::thread::id, std::thread> threads_;
};
}  // namespace nxt::core
