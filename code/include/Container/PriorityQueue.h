#pragma once

#include <functional>

#include "PageVector.h"
#include "../Sort.h"

namespace nxt::core {

template<typename T, typename Container = PageVector<T, 32>, typename Compare = std::less<>>
class PriorityQueue {
public:
    using value_type = typename Container::value_type;
    using value_compare = Compare;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using container_type = Container;

    PriorityQueue() noexcept(std::is_nothrow_default_constructible_v<container_type>&&
                                 std::is_nothrow_default_constructible_v<value_compare>)
        : container_()
        , comp_() {}

    PriorityQueue(const PriorityQueue& rhs)
        : container_(container_)
        , comp_(rhs.comp_) {}

    PriorityQueue(PriorityQueue&& rhs)
        : container_(rhs.container_)
        , comp_(rhs.comp_) {}

    explicit PriorityQueue(const container_type& container)
        : container_(container)
        , comp_() {
        makeHeap(container_.begin(), container_.end(), comp_);
    }

    explicit PriorityQueue(const value_compare& comp)
        : container_()
        , comp_(comp) {}

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    PriorityQueue(InputIter first, InputIter last)
        : container_(first, last)
        , comp_() {
        makeHeap(container_.begin(), container_.end(), comp_);
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    PriorityQueue(InputIter first, InputIter last, const value_compare& comp)
        : container_(first, last)
        , comp_(comp) {
        makeHeap(container_.begin(), container_.end(), comp_);
    }

    PriorityQueue(const value_compare& comp, const container_type& container)
        : container_(container)
        , comp_(comp) {
        makeHeap(container_.begin(), container_.end(), comp_);
    }

	PriorityQueue(std::initializer_list<T> values)
		: container_(values)
		, comp_() {
        makeHeap(container_.begin(), container_.end(), comp_);
	}

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

    [[nodiscard]] const_reference top() const noexcept(noexcept(container_.front())) {
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
    container_type container_;
    value_compare comp_;
};

template<typename InputIter,
         typename Container = PageVector<IteratorValueTypeT<InputIter>, 32>,
         typename Compare = std::less<>,
         typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
PriorityQueue(InputIter first, InputIter last)->PriorityQueue<IteratorValueTypeT<InputIter>, Container, Compare>;

}  // namespace nxt::core
