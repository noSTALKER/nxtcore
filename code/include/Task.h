#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace nxt::core {
class TaskQueue;
/**
 * @brief Base Class for all task classes
 *
 */
class Task {
public:
    /**
     * @brief
     *
     */
    enum class Status : uint8_t { kNotQueued, kQueued, kRunning, kError, kFinished };

    /**
     * @brief Construct a new Task object
     *
     */
    Task()
        : status_(Status::kNotQueued) {}

    /**
     * @brief
     *
     */
    void execute() noexcept {
        if (getStatus() == Status::kQueued) {
            setStatus(Status::kRunning);
            try {
                if (run()) {
                    setStatus(Status::kFinished);
                } else {
                    setStatus(Status::kError);
                }
                
            } catch (...) {
                setStatus(Status::kError);
            }
        }
    }

    /**
     * @brief Get the Status object
     *
     * @return Status
     */
    Status getStatus() const noexcept {
        return status_.load(std::memory_order::memory_order_acquire);
    }

    /**
     * @brief
     *
     */
    void wait() const {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_variable_.wait(lock, [this]() {
            auto status = getStatus();
            return status == Status::kError || status == Status::kFinished;
        });
    }

    template<typename Rep, typename Period>
    void waitFor(const std::chrono::duration<Rep, Period>& duration) const {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_variable_.wait_for(lock, duration, [this]() {
            auto status = GetStatus();
            return status == Status::kError || status == Status::kFinished;
        });
    }

    /**
     * @brief Destroy the Task object
     *
     */
    virtual ~Task() = default;

protected:
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    virtual bool run() = 0;

    /**
     * @brief Set the Status object
     *
     * @param status
     */
    void setStatus(Status status) noexcept {
        if (status != getStatus()) {
            std::unique_lock lock(mutex_);
            status_.store(status, std::memory_order::memory_order_release);
            if (status == Status::kError || status == Status::kFinished) {
                cond_variable_.notify_all();
            }
        }
    }

private:
    std::atomic<Status> status_;
    mutable std::mutex mutex_;
    mutable std::condition_variable cond_variable_;

    // Add TaskQueue as a friend class so that it can call SetStatus function
    friend class TaskQueue;
};
}  // namespace nxt::core
