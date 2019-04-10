#pragma once
#include <iterator>
#include <type_traits>

namespace nxt {
namespace core {

template<typename List>
class ConstListIterator {
public:
    using node_type = typename List::node_type;
    using value_type = typename List::value_type;
    using difference_type = typename List::difference_type;
    using pointer = typename List::const_pointer;
    using reference = typename List::const_reference;
    using const_reference = typename List::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;

    ConstListIterator(List* list, node_type* node)
        : list_(list)
        , node_(node) {}

    ConstListIterator& operator++() noexcept {
        node_ = node_->next;
        return *this;
    }

    ConstListIterator operator++(int) noexcept {
        ConstListIterator temp = ConstListIterator(list_, node_);
        node_ = node_->next;
        return temp;
    }

    ConstListIterator& operator--() noexcept {
        node_ = node_->previous;
        return *this;
    }

    ConstListIterator operator--(int) noexcept {
        ConstListIterator temp = ConstListIterator(list_, node_);
        node_ = node_->previous;
        return temp;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return node_->value;
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(node->value);
    }

    [[nodiscard]] bool operator==(const ConstListIterator& rhs) const noexcept {
        return node_ == rhs->node_;
    }

    [[nodiscard]] bool operator!=(const ConstListIterator& rhs) const noexcept {
        return node_ != rhs->node_;
    }

protected:
    List* list_;
    node_type* node_;

    friend class List;
};

template<typename List>
class ListIterator : public ConstListIterator<List> {
public:
    using node_type = typename List::node_type;
    using value_type = typename List::value_type;
    using difference_type = typename List::difference_type;
    using pointer = typename List::pointer;
    using reference = typename List::reference;
    using iterator_category = std::bidirectional_iterator_tag;

    ListIterator(List* list, node_type* node)
        : ConstListIterator<List>(list, node) {}

    ListIterator& operator++() noexcept {
        node_ = node_->next;
        return *this;
    }

    ListIterator operator++(int) noexcept {
        ListIterator temp = ListIterator(list_, node_);
        node_ = node_->next;
        return temp;
    }

    ListIterator& operator--() noexcept {
        node_ = node_->previous;
        return *this;
    }

    ListIterator operator--(int) noexcept {
        ListIterator temp = ListIterator(list_, node_);
        node_ = node_->previous;
        return temp;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return node_->value;
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(node_->value);
    }
};

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    struct Node;

    using node_type = Node;
    using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    using value_type = T;
    using allocator_type = std::allocator_traits<Allocator>::template rebind<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = std::allocator_traits<allocator_type>::size_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    using iterator = ListIterator<List>;
    using const_iterator = ConstListIterator<List>;

    struct Node {
        Node* next;
        Node* previous;
        value_type value;
    };

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

    iterator erase(const_iterator position) {
        auto node = position.ptr_;
        auto next_node = eraseNode(node);
        return iterator(this, next_node);
	}

private:
    constructHeadNode() {
        head_ = node_allocator_traits::allocate(alloc_, 1);
        head_->next = head_;
        head_->previous = head_;
    }

    template<typename... Args>
    Node* insertNode(Node* node, Args&&... args) {
        auto previous_node = node;
        auto next_node = previous_node->next;

        auto new_node = node_allocator_traits::allocate(alloc_, 1);
        allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);

        new_node->next = next_node;
        new_node->previous = previous_node;

        next_node->previous = new_node;
        previous_node->next = new_node;
        ++size_;

        return new_node;
    }

    Node* eraseNode(Node* node) {
        if (node != head_) {
            auto previous_node = node->previous;
            auto next_node = node->next;

            allocator_traits::destroy(alloc_, std::addressof(next_node->value));
            node_allocator_traits::deallocate(alloc_, node, 1);

			previous_node->next = next_node;
            next_node->previous = previous_node;

			return next_node;
        }

		return node;

    }

    Node* head_;
    size_type size_;
    node_allocator_type alloc_;

    friend class iterator;
    friend class const_iterator;
};
}  // namespace core
}  // namespace nxt