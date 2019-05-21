#pragma once

#include "PageVector.h"

namespace nxt::core {

template<typename T, typename Container = PageVector<T, 32>>
class Stack {
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;

    Stack() noexcept(std::is_nothrow_default_constructible_v<Container>)
        : container_() {}

    explicit Stack(const Container& container) noexcept(std::is_nothrow_copy_constructible_v<Container>)
        : container_(container) {}

    explicit Stack(Container&& container) noexcept(std::is_nothrow_move_constructible_v<Container>)
        : container_(std::move(container)) {}

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    Stack(InputIter first, InputIter last)
        : container_(first, last) {}

    [[nodiscard]] reference top() noexcept {
        return container_.back();
    }

    [[nodiscard]] const_reference top() const noexcept {
        return container_.back();
    }

    void pop() {
        container_.popBack();
    }

    [[nodiscard]] value_type popAndExtract() {
        value_type result = std::move(container_.back());
        container_.popBack();
        return result;
    }

    void push(const T& value) {
        container_.pushBack(value);
    }

    void push(T&& value) {
        container_.pushBack(std::move(value));
    }

    template<typename... Args>
    decltype(auto) emplace(Args&&... args) {
        return container_.emplaceBack(std::forward<Args>(args)...);
    }

    [[nodiscard]] bool empty() const noexcept {
        return container_.empty();
    }

    [[nodiscard]] size_type size() const noexcept {
        return container_.size();
    }

private:
    container_type container_;
};

template<typename InputIter,
         typename Container = PageVector<IteratorValueTypeT<InputIter>, 32>,
         typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
Stack(InputIter, InputIter)->Stack<IteratorValueTypeT<InputIter>, Container>;

}  // namespace nxt::core