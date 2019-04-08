#pragma once
#include <list>

namespace nxt {
namespace core {

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    class ListIterator;
    class ConstListIterator;

    using value_type = T;
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = std::allocator_traits<allocator_type>::size_type;
    using reference = T&;
    using const_reference = const T&;
    using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    using iterator = ListIterator;
    using const_iterator = ConstListIterator;

    List()
        : alloc_()
        , size_type_(0) {
        constructHeadNode();
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] iterator end() noexcept {
        return iterator(this, head_);
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this, head_);
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator(this, head_);
    }

    [[nodiscard]] iterator begin() noexcept {
        return iterator(this, head_->next);
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this, head_->next);
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        return iterator(this, head_->next);
    }

    template<typename... Args>
    iterator emplaceBack(Args&&... args) {
        auto previous_node = head_->previous;
        auto new_node = insertNode(previous_node, std::forward<Args>(args)...);
        return iterator(this, new_node);
    }

    iterator insert(const_iterator position, const T& value) {
        auto previous_node = position.ptr_;
        auto new_node = insertNode(previous_node, value);
        return iterator(this, new_node);
    }

    iterator insert(const_iterator position, T&& value) {
        auto previous_node = position.ptr_;
        auto new_node = insertNode(previous_node, std::move(value));
        return iterator(this, new_node);
    }

private:
    struct Node {
        Node* next;
        Node* previous;
        value_type value;
    };

    constructHeadNode() {
        head_ = node_allocator_traits::allocate(alloc_, 1);
        head_->next = head_;
        head_->previous = head_;
    }

    template<typename... Args>
    Node* insertNode(Node* node, Args&&... args) {
        auto previous_node = node;
        auto next_node = previous_node->next;

        auto new_node = node_allocator::allocate(alloc_, 1);
        allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);

        new_node->next = next_node;
        new_node->previous = previous_node;

        next_node->previous = new_node;
        previous_node->next = new_node;

        return new_node;
    }

    Node* head_;
    size_type size_;
    node_allocator_type alloc_;
};
}  // namespace core
}  // namespace nxt