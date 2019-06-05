#include "../include/TaskQueue.h"

#include <thread>

namespace nxt::core {
TaskQueue::TaskQueue(const std::string& name)
    : name_(name)
    , current_threads_(0)
    , max_threads_(std::max(std::thread::hardware_concurrency(), 2u) - 1)
    , queue_running_(false) {}

const std::string&
TaskQueue::getName() const noexcept {
    return name_;
}

void
TaskQueue::setMaxThreads(uint32_t num_of_threads) {
    max_threads_ = num_of_threads;
    if (queue_running_.load(std::memory_order_acquire)) {
        spawnWorkerThreads();
    }
}

uint32_t
TaskQueue::getMaxThreads() const noexcept {
    return max_threads_;
}

bool
TaskQueue::addTask(const std::shared_ptr<Task>& task) {
    if (task->getStatus() == Task::Status::kNotQueued) {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(task);
        task->setStatus(Task::Status::kQueued);
        conditional_variable_.notify_one();
        return true;
    }

    return false;
}

std::size_t
TaskQueue::addTasks(const Vector<std::shared_ptr<Task>>& tasks) {
    uint32_t count = 0;
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& task : tasks) {
        if (task->getStatus() == Task::Status::kNotQueued) {
            tasks_.push(task);
            task->setStatus(Task::Status::kQueued);
            ++count;
        }
    }
    conditional_variable_.notify_all();

    return count;
}

void
TaskQueue::start() {
    queue_running_.store(true, std::memory_order_release);
    spawnWorkerThreads();
}

void
TaskQueue::stop() {
    std::lock_guard lock(mutex_);
    queue_running_.store(false, std::memory_order_release);
    conditional_variable_.notify_all();
}

void
TaskQueue::spawnWorkerThreads() {
    while (current_threads_.load(std::memory_order_acquire) < max_threads_) {
        std::lock_guard lock(mutex_);
        current_threads_.fetch_add(1, std::memory_order_release);
        std::thread thread(&TaskQueue::workerLogic, this);
        threads_[thread.get_id()] = std::move(thread);
    }
}

void
TaskQueue::workerLogic() {
    while (queue_running_.load(std::memory_order_acquire) &&
           current_threads_.load(std::memory_order_acquire) <= max_threads_) {
        std::unique_lock<std::mutex> lock(mutex_);
        conditional_variable_.wait(
            lock, [this]() { return !queue_running_.load(std::memory_order_acquire) || !tasks_.empty(); });

        if (!queue_running_.load(std::memory_order_acquire))
            break;

        std::shared_ptr<Task> task = tasks_.popAndExtract();
        lock.unlock();

        if (task->getStatus() == Task::Status::kQueued) {
            task->execute();
        }
    }
    current_threads_.fetch_sub(1, std::memory_order_release);
}

TaskQueue::~TaskQueue() {
    stop();

    for (auto& pair : threads_)
        if (pair.second.joinable())
            pair.second.join();

}

}  // namespace nxt::core
