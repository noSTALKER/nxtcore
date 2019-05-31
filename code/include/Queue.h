#pragma once

#include <functional>
#include <queue>

#include "RingBuffer.h"

namespace nxt::core {

template<typename T, typename Container = RingBuffer<T>>
class Queue {
public:
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using container_type = Container;

    Queue() noexcept(std::is_nothrow_default_constructible_v<container_type>)
        : container_() {}

    explicit Queue(const container_type& container)
        : container_(container) {}

    template<typename Allocator, typename = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>>
    explicit Queue(const Allocator& alloc) noexcept(std::is_nothrow_constructible_v<container_type, const Allocator&>)
        : container_(alloc) {}

    template<typename Allocator, typename = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>>
    Queue(const container_type& container, const Allocator& alloc)
        : container_(container, alloc) {}

    template<typename Allocator, typename = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>>
    Queue(const Queue& rhs, const Allocator& alloc)
        : container_(rhs.container_, alloc) {}

    explicit Queue(container_type&& container) noexcept(std::is_nothrow_move_constructible_v<container_type>)
        : container_(std::move(container)) {}

    template<typename Allocator, typename = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>>
    Queue(container_type&& container, const Allocator& alloc)
        noexcept(std::is_nothrow_constructible_v<container_type, container_type&&, const Allocator&>)
        : container_(std::move(container), alloc) {
    }

    template<typename Allocator, typename = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>>
    Queue(Queue&& rhs, const Allocator& alloc)
        noexcept(std::is_nothrow_constructible_v<container_type, container_type&&, const Allocator&>)
        : container_(std::move(rhs.container), alloc) {
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    Queue(InputIter first, InputIter last)
        : container_(first, last) {
    }

    [[nodiscard]] bool empty() const noexcept(noexcept(container_.empty())) {
        return container_.empty();
    }

    [[nodiscard]] size_type size() const noexcept(noexcept(container_.size())) {
        return container_.size();
    }

    void push(const T& value) {
        container_.pushBack(value);
    }

    void push(T&& value) {
        container_.pushBack(std::move(value));
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        container_.emplaceBack(std::forward<Args>(args)...);
    }

    [[nodiscard]] const_reference front() const noexcept(noexcept(container_.front())) {
        return container_.front();
    }

    [[nodiscard]] reference front() noexcept(noexcept(container_.front())) {
        return container_.front();
    }

    [[nodiscard]] const_reference back() const noexcept(noexcept(container_.back())) {
        return container_.back();
    }

    [[nodiscard]] reference back() noexcept(noexcept(container_.back())) {
        return container_.back();
    }

    void pop() {
        container_.popFront();
    }

    [[nodiscard]] value_type popAndExtract() {
        return container_.popFrontAndExtract();
    }

private:
    Container container_;
};

template<typename InputIter,
         typename Container = RingBuffer<IteratorValueTypeT<InputIter>>,
         typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
Queue(InputIter first, InputIter last)->Queue<IteratorValueTypeT<InputIter>, Container>;

}  // namespace nxt::core