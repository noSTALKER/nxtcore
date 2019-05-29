#pragma once

#include <functional>
#include <future>
#include <thread>

#include "Task.h"

namespace nxt::core {

template<typename Tuple, typename Result, typename Func, typename... Args>
class GenericTask : public Task {
public:
    GenericTask(std::unique_ptr<Tuple> tuple)
        : tuple_(std::move(tuple)) {}

    Result getResult() const noexcept {
        wait();
        return result_;
    }

protected:
    bool run() override {
        this->invokeHelper(std::make_index_sequence<1 + sizeof...(Args)>{});
        return true;
    }

private:
    template <size_t... Indices> 
    void invokeHelper(std::index_sequence<Indices...>) {
        result_ = std::invoke(std::move(std::get<Indices>(*tuple_))...);
    }

    std::unique_ptr<Tuple> tuple_;
    Result result_;
};

template<typename Func, typename... Args>
[[nodiscard]] auto
makeGenericTask(Func&& func, Args&& ... args) {
    using tuple_type = std::tuple<std::decay_t<Func>, std::decay_t<Args>...>;
    using func_type = std::decay_t<Func>;
    using result_type = std::invoke_result_t<Func, Args...>;
    using generic_task_type = GenericTask<tuple_type, result_type, func_type, Args...>;

    auto tuple = std::make_unique<tuple_type>(std::forward<Func>(func), std::forward<Args>(args)...);

    if constexpr (!std::is_void_v<result_type>) {
        return std::make_shared<GenericTask<tuple_type, result_type, Func, Args...>>(std::move(tuple));
    } else {

    }

}

}  // namespace nxt::core
