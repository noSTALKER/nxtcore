#pragma once
#include <iterator>
#include <list>
#include <type_traits>
#include "TypeTraits.h"

namespace nxt::core {

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
        return node_ == rhs.node_;
    }

    [[nodiscard]] bool operator!=(const ConstListIterator& rhs) const noexcept {
        return node_ != rhs.node_;
    }

protected:
    List* list_;
    node_type* node_;

    friend typename List;
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
    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = typename allocator_traits::size_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using difference_type = typename allocator_traits::difference_type;

    using iterator = ListIterator<List>;
    using const_iterator = ConstListIterator<List>;

    struct Node {
        Node* next;
        Node* previous;
        value_type value;

        Node(const Node*) = delete;
        Node& operator=(const Node*) = delete;
    };

    List()
        : alloc_()
        , size_(0) {
        constructHeadNode();
    }

    explicit List(const Allocator& alloc)
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
    }

    explicit List(size_type count, const Allocator& alloc = Allocator())
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
        insertCountNode(head_->next, count);
    }

    List(size_type count, const value_type& value, const Allocator& alloc = Allocator())
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
        insertCountNode(head_->next, count, value);
    }

    template<typename InputIter, typename = std::enable_if_t<IsIteratorV<InputIter>>>
    List(InputIter first, InputIter last, const Allocator& alloc = Allocator())
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
        insertFromIterator(head_->next, first, last);
    }

    List(const List& list)
        : alloc_(node_allocator_traits::select_on_container_copy_construction(list.alloc_))
        , size_(0) {
        constructHeadNode();
        insertFromIterator(head_->next, list.begin(), list.end());
    }

    List(const List& list, const Allocator& alloc)
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
        insertFromIterator(head_->next, list.begin(), list.end());
    }

    List(std::initializer_list<T> values, const Allocator& alloc = Allocator())
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();
        insertFromIterator(head->next, values.begin(), values.end());
    }

    List(List&& list)
        : alloc_(std::move(list.alloc_))
        , size_(0) {
        constructHeadNode();
        std::swap(head_, list.head_);
    }

    List(List&& list, const Allocator& alloc)
        : alloc_(alloc)
        , size_(0) {
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
        auto new_node = insertNode(head_, std::forward<Args>(args)...);
        return iterator(this, new_node);
    }

    template<typename... Args>
    iterator emplaceFront(Args&&... args) {
        auto new_node = insertNode(head_->next, std::forward<Args>(args)...);
        return iterator(this, new_node);
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        auto new_node = insertNode(position.node_, std::forward<Args>(args)...);
        return iterator(this, new_node);
    }

    iterator insert(const_iterator position, const T& value) {
        auto new_node = insertNode(position.node_, value);
        return iterator(this, new_node);
    }

    iterator insert(const_iterator position, T&& value) {
        auto new_node = insertNode(position.node_, std::move(value));
        return iterator(this, new_node);
    }

    iterator insert(const_iterator position, std::initializer_list<T> list) {
        auto new_node = insertFromIterator(position.node_, list.begin(), list.end());
        return iterator(this, new_node);
    }

    template<typename InputIter, typename = std::enable_if_t<IsIteratorV<InputIter>>>
    iterator insert(const_iterator position, InputIter first, InputIter last) {
        auto new_node = insertFromIterator(position.node_, first, last);
        return iterator(this, new_node);
    }

    void pushBack(const T& value) {
        insertNode(head_, value);
    }

    void pushBack(T&& value) {
        insertNode(head_, std::move(value));
    }

    void pushFront(const T& value) {
        insertNode(head_->next, value);
    }

    void pushFront(T&& value) {
        insertNode(head_->next, std::move(value));
    }

    iterator erase(const_iterator position) {
        auto node = position.node_;
        auto next_node = eraseNode(node);
        return iterator(this, next_node);
    }

    void popBack() {
        eraseNode(head_->previous);
    }

    void popFront() {
        eraseNode(head_->next);
    }

    void clear() noexcept {
        auto node = head_->next;
        while (node != head_) {
            auto next_node = node->next;
            node_allocator_traits::destroy(alloc_, std::addressof(node->value));
            node_allocator_traits::deallocate(alloc_, node, 1);

            node = next_node;
        }

        head_->next = head_;
        head_->previous = head_;
        size_ = 0;
    }

    ~List() {
        clear();
        destroyHeadNode();
    }

private:
    void constructHeadNode() {
        head_ = node_allocator_traits::allocate(alloc_, 1);
        head_->next = head_;
        head_->previous = head_;
    }

    template<typename... Args>
    Node* insertNode(Node* node, Args&&... args) {
        auto next_node = node;
        auto previous_node = next_node->previous;

        auto new_node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);

        new_node->next = next_node;
        new_node->previous = previous_node;

        next_node->previous = new_node;
        previous_node->next = new_node;
        ++size_;

        return new_node;
    }

    template<typename... Args>
    Node* insertCountNode(Node* node, size_type count, Args&&... args) {
        auto next_node = node;
        auto previous_node = next_node->previous;
        bool added = false;

        size_type i = count;
        while (i) {
            auto new_node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);

            new_node->previous = previous_node;
            previous_node->next = new_node;

            previous_node = new_node;
            added = true;

            ++size_;
            --i;
        }

        if (added) {
            previous_node->next = next_node;
            next_node->previous = previous_node;
            return node->next;
        } else {
            return node;
        }
    }

    template<typename InputIter>
    Node* insertFromIterator(Node* node, InputIter first, InputIter last) {
        auto next_node = node;
        auto previous_node = next_node->previous;
        auto iter = first;
        bool added = false;

        while (iter != last) {
            auto new_node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->value), *iter);

            new_node->previous = previous_node;
            previous_node->next = new_node;

            previous_node = new_node;
            added = true;

            ++size_;
            ++iter;
        }

        if (added) {
            previous_node->next = next_node;
            next_node->previous = previous_node;
            return node->next;
        } else {
            return node;
        }
    }

    Node* eraseNode(Node* node) noexcept {
        if (node != head_) {
            auto previous_node = node->previous;
            auto next_node = node->next;

            node_allocator_traits::destroy(alloc_, std::addressof(next_node->value));
            node_allocator_traits::deallocate(alloc_, node, 1);

            previous_node->next = next_node;
            next_node->previous = previous_node;

            --size_;

            return next_node;
        }

        return node;
    }

    void clearList() noexcept {}

    void destroyHeadNode() noexcept {
        node_allocator_traits::deallocate(alloc_, head_, 1);
    }

    Node* head_;
    size_type size_;
    node_allocator_type alloc_;

    friend class iterator;
    friend class const_iterator;
};
}  // namespace nxt::core