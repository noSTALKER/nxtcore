#pragma once

#include <atomic>

namespace nxt::core {

class SpinLock {
public:
    void lock() noexcept {
        while (flag_.test_and_set(std::memory_order_acquire)) {
        }
    }

    void unlock() noexcept {
        flag_.clear(std::memory_order_release);
    }

    void try_lock() noexcept {
        return !flag_.test_and_set(std::memory_order_acquire);
    }

	SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

}  // namespace nxt::core
