#pragma once

#include <functional>

#include "PageVector.h"

namespace nxt::core {

template<typename T, typename Container = PageVector<T, 32>, typename Compare = std::less<>>
class PriorityQueue {
public:
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using compare_type = Compare;

    PriorityQueue() noexcept(
        std::is_nothrow_default_constructible_v<Container>&& std::is_nothrow_default_constructible_v<Compare>)
        : container_()
        , comp_() {}

    [[nodiscard]] bool empty() const noexcept(noexcept(container_.empty())) {
        return container_.empty();
    }

    [[nodiscard]] size_type size() const noexcept(noexcept(container_.size())) {
        return container_.size();
    }

    void push(const T& value) {
        container_.pushBack(value);
        pushHeap(container.begin(), container.end(), comp_);
    }

    void push(T&& value) {
        container_.pushBack(std::move(value));
        pushHeap(container_.begin(), container_.end(), comp_);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        container_.emplaceBack(std::forward<Args>(args)...);
        pushHeap(container_.begin(), container_.end(), comp_);
    }

    [[nodiscard]] const_reference top() noexcept(container_.front()) {
        return container_.front();
    }

    void pop() {
        popHeap(container_.begin(), container_.end(), comp_);
        container_.popBack();
    }

    [[nodiscard]] value_type popAndExtract() {
        popHeap(container_.begin(), container_.end(), comp_);
        value_type result = std::move(container_.back());
        container_.popBack();
        return result;
    }

private:
    Container container_;
    Compare comp_;
};

}  // namespace nxt::core
