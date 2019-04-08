#pragma once
#include <list>

namespace nxt {
namespace core {

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = std::allocator_traits<allocator_type>::size_type;
    using reference = T&;
    using const_reference = const T&;
    using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

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

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        auto new_node = node_allocator_traits::allocate(alloc_, 1);
        auto previous_node = head_->previous;

        allocator_traits::construct(alloc_, std::addressof(new_node.value), std::forward<Args>(args)...);
        new_node->next = head_;
        new_node->previous = previous_node;
        previous_node->next = new_node;
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

    Node* head_;
    size_type size_;
    node_allocator_type alloc_;
};
}  // namespace core
}  // namespace nxt