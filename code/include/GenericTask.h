#pragma once

#include <functional>
#include <future>
#include <thread>

#include "Task.h"

namespace nxt::core {

template<class Result, class Tuple>
class GenericTask : public Task {
public:
    template<class Func, class... Args>
    GenericTask(Func&& func, Args&&... args) {
        functor_ = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    Result getResult() const noexcept {
        wait();
        return result_;
    }

protected:
    bool run() override {
        result_ = functor_();
        return true;
    }

private:
    Tuple functor_;
    Result result_;
};

template<class Tuple>
class GenericTask<void> : public Task {
public:
    template<class Func, class... Args>
    GenericTask(Func&& func, Args&&... args) {
        functor_ = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    }

protected:
    bool run() override {
        functor_();
    }

private:
    Tuple functor_;
};
}  // namespace nxt::core
