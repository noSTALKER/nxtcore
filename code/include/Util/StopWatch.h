#pragma once

#include <chrono>
#include <string>

namespace nxt::core {

class StopWatch {
public:
    StopWatch(const std::string& name)
        : name_(name)
        , duration_(0)
        , is_running_(false) {}

    void start() {
        if (!is_running_) {
            start_point_ = std::chrono::high_resolution_clock::now();
            is_running_ = true;
        }
    }

    void stop() {
        if (is_running_) {
            auto end_point = std::chrono::high_resolution_clock::now();
            is_running_ = false;
            auto current_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point_);
            duration_ += current_duration;
        }
    }
    void reset() {
        is_running_ = false;
        duration_ = std::chrono::nanoseconds::zero();
    }

    [[nodiscard]] isRunning() const noexcept {
        return is_running_;
    }

    template<typename Duration>
    [[nodiscard]] Duration getDuration() const noexcept {
        return std::chrono::duration_cast<Duration>(duration_);
    }

    StopWatch(const StopWatch&) = delete;
    StopWatch& operator=(const StopWatch&) = delete;

private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_point_;
    std::chrono::nanoseconds duration_;
    bool is_running_;
};

}  // namespace nxt::core
