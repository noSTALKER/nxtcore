#pragma once
#include <iterator>
#include <list>
#include "../TypeTraits.h"

namespace nxt::core {

template<typename List>
class ListConstIterator {
public:
    using node_type = typename List::node_type;
    using node_pointer = typename List::node_pointer;
    using value_type = typename List::value_type;
    using difference_type = typename List::difference_type;
    using pointer = typename List::const_pointer;
    using reference = typename List::const_reference;
    using const_reference = typename List::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;

    ListConstIterator(List* list, node_pointer node)
        : list_(list)
        , node_(node) {}

    ListConstIterator& operator++() noexcept {
        node_ = node_->next;
        return *this;
    }

    ListConstIterator operator++(int) noexcept {
        ListConstIterator temp = ListConstIterator(list_, node_);
        node_ = node_->next;
        return temp;
    }

    ListConstIterator& operator--() noexcept {
        node_ = node_->previous;
        return *this;
    }

    ListConstIterator operator--(int) noexcept {
        ListConstIterator temp = ListConstIterator(list_, node_);
        node_ = node_->previous;
        return temp;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return node_->value;
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointerAt(node->value);
    }

    [[nodiscard]] bool operator==(const ListConstIterator& rhs) const noexcept {
        return node_ == rhs.node_;
    }

    [[nodiscard]] bool operator!=(const ListConstIterator& rhs) const noexcept {
        return node_ != rhs.node_;
    }

protected:
    List* list_;
    node_pointer node_;

    friend List;
};

template<typename List>
class ListIterator : public ListConstIterator<List> {
public:
    using node_type = typename List::node_type;
    using node_pointer = typename List::node_pointer;
    using value_type = typename List::value_type;
    using difference_type = typename List::difference_type;
    using pointer = typename List::pointer;
    using reference = typename List::reference;
    using iterator_category = std::bidirectional_iterator_tag;
	using base_class = ListConstIterator<List>;

    ListIterator(List* list, node_pointer node)
        : ListConstIterator<List>(list, node) {}

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
        return pointer_traits<pointer>::pointerAt(node_->value);
    }

protected:
	// needed as its a dependent name
	using base_class::node_;
};

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    struct Node;

    using node_type = Node;
    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer = typename node_allocator_traits::pointer;
    using node_const_pointer = typename node_allocator_traits::const_pointer;

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
    using const_iterator = ListConstIterator<List>;

    struct Node {
        node_pointer next;
        node_pointer previous;
        value_type value;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };

    List()
        : size_(0)
        , alloc_() {
        constructHeadNode();
    }

    explicit List(const Allocator& alloc)
        : size_(0)
        , alloc_(alloc) {
        constructHeadNode();
    }

    explicit List(size_type count, const Allocator& alloc = Allocator())
        : size_(0)
        , alloc_(alloc) {
        constructHeadNode();
        insertCountNode(head_->next, count);
    }

    List(size_type count, const value_type& value, const Allocator& alloc = Allocator())
        : size_(0)
        , alloc_(alloc) {
        constructHeadNode();
        insertCountNode(head_->next, count, value);
    }

    template<typename InputIter, typename = std::enable_if_t<IsIteratorV<InputIter>>>
    List(InputIter first, InputIter last, const Allocator& alloc = Allocator())
        : size_(0)
        , alloc_(alloc) {
        constructHeadNode();
        insertFromIterator(head_->next, first, last);
    }

	List(std::initializer_list<value_type> values)
        : List(values.begin(), values.end()) {}

    List(const List& list)
        : size_(0)
        , alloc_(node_allocator_traits::select_on_container_copy_construction(list.alloc_)) {
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
        std::swap(size_, list.size_);
    }

    List(List&& list, const Allocator& alloc)
        : alloc_(alloc)
        , size_(0) {
        constructHeadNode();

        // in case the allocators are not equal, move the elements from the list to the new list using move
        // iterators
        if constexpr (!node_allocator_traits::is_always_equal::value) {
            if (alloc_ != right.alloc_) {
                insertFromIterator(
                    head->next, std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
                return;
            }
        }

        std::swap(head_, list.head_);
        std::swap(size_, list.size_);
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] reference front() {
        return head_->next->value;
    }

    [[nodiscard]] const_reference front() const {
        return head_->next->value;
    }

    [[nodiscard]] reference back() {
        return head_->previous->value;
    }

    [[nodiscard]] const_reference back() const {
        return head_->previous->value;
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

    template<typename... Args>
    reference emplaceBack(Args&& ... args) {
        auto node = insertNode(head_, std::forward<Args>(args)...);
        return node->value;
    }

    void pushFront(const T& value) {
        insertNode(head_->next, value);
    }

    void pushFront(T&& value) {
        insertNode(head_->next, std::move(value));
    }

    template<typename... Args>
    reference emplaceFront(Args&&... args) {
        auto node = insertNode(head_->next, std::forward<Args>(args)...);
        return node->value;
    }

    iterator erase(const_iterator position) {
        auto node = position.node_;
        auto next_node = eraseNode(node);
        return iterator(this, next_node);
    }

    iterator erase(const_iterator first, const_iterator last) {
        auto next_node = eraseNodes(first.node_, last.node_);
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
            node_allocator_traits::destroy(alloc_, std::addressof(node->previous));
            node_allocator_traits::destroy(alloc_, std::addressof(node->next));
            node_allocator_traits::deallocate(alloc_, node, 1);

            node = next_node;
        }

        head_->next = head_;
        head_->previous = head_;
        size_ = 0;
    }

    void reverse() noexcept {
        // start from the head node
        auto node = head_;
        do {
            // reverse the links of the node
            auto next_node = node->next;
            node->next = node->previous;
            node->previous = next_node;

            // move to the next node
            node = next_node;
            // stop if reached the head
        } while (node != head_);
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    void assign(InputIter first, InputIter last) {
        auto old_node = head_->next;
        while (true) {
            // if we run out of old nodes, append the rest of the values in the end of the list
            if (old_node == head_) {
                insertFromIterator(old_node, first, last);
                return;
            }

            // if we run of values to append, erase remaining nodes
            if (first == last) {
                eraseNodes(old_node, head_);
                return;
            }

            // if value is assignable from the iterator value_type, use it
            old_node->value = *first;

            old_node = old_node->next;
            ++first;
        }
    }

    void assign(const T& value, size_type count) {
        auto old_node = head_->next;
        while (true) {
            // if we run out of old nodes, append the rest of the values in the end of the list
            if (old_node == head_) {
                insertCountNode(old_node, count, value);
                return;
            }

            // if we run of values to append, erase remaining nodes
            if (count == 0) {
                eraseNodes(old_node, head_);
                return;
            }

            // if value is copy assignable, use it
            old_node->value = value;
            old_node = old_node->next;
            --count;
        }
    }

    void assign(std::initializer_list<T> values) {
        assign(values.begin(), values.end());
    }

    void resize(size_type new_size) {
        if (new_size > size_) {
            insertCountNode(head_, new_size - size_);
        } else {
            auto node = head_->next;
            for (size_type i = 0; i < new_size; ++i) {
                node = node->next;
            }

            eraseNodes(node, head_);
        }
    }

    void resize(size_type new_size, const T& value) {
        if (new_size > size_) {
            insertCountNode(head_, new_size - size_, value);
        } else {
            auto node = head_->next;
            for (size_type i = 0; i < new_size; ++i) {
                node = node->next;
            }

            eraseNodes(node, head_);
        }
    }

    ~List() {
        clear();
        destroyHeadNode();
    }

private:
    void constructHeadNode() {
        head_ = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(head_->next), head_);
        node_allocator_traits::construct(alloc_, std::addressof(head_->previous), head_);
    }

    template<typename... Args>
    node_pointer insertNode(node_pointer node, Args&&... args) {
        auto next_node = node;
        auto previous_node = next_node->previous;

        auto new_node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->next), next_node);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->previous), previous_node);

        next_node->previous = new_node;
        previous_node->next = new_node;
        ++size_;

        return new_node;
    }

    template<typename... Args>
    node_pointer insertCountNode(node_pointer node, size_type count, Args&&... args) {
        auto next_node = node;
        auto previous_node = next_node->previous;
        bool added = false;

        for (size_type i = 0; i < count; ++i) {
            auto new_node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Args>(args)...);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->next));
            node_allocator_traits::construct(alloc_, std::addressof(new_node->previous), previous_node);

            previous_node->next = new_node;

            previous_node = new_node;
            added = true;

            ++size_;
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
    node_pointer insertFromIterator(node_pointer node, InputIter first, InputIter last) {
        auto next_node = node;
        auto previous_node = next_node->previous;
        auto iter = first;
        bool added = false;

        while (iter != last) {
            auto new_node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->value), *iter);
            node_allocator_traits::construct(alloc_, std::addressof(new_node->next));
            node_allocator_traits::construct(alloc_, std::addressof(new_node->previous), previous_node);

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

    node_pointer eraseNode(node_pointer node) noexcept {
        if (node != head_) {
            auto previous_node = node->previous;
            auto next_node = node->next;

            node_allocator_traits::destroy(alloc_, std::addressof(next_node->value));
            node_allocator_traits::destroy(alloc_, std::addressof(next_node->next));
            node_allocator_traits::destroy(alloc_, std::addressof(next_node->previous));
            node_allocator_traits::deallocate(alloc_, node, 1);

            previous_node->next = next_node;
            next_node->previous = previous_node;

            --size_;

            return next_node;
        }

        return node;
    }

    node_pointer eraseNodes(node_pointer first, node_pointer last) noexcept {
        auto previous_node = first->previous;
        while (first != last) {
            auto next_node = first->next;

            node_allocator_traits::destroy(alloc_, std::addressof(first->value));
            node_allocator_traits::destroy(alloc_, std::addressof(first->next));
            node_allocator_traits::destroy(alloc_, std::addressof(first->previous));
            node_allocator_traits::deallocate(alloc_, first, 1);

            --size_;

            first = next_node;
        }

        previous_node->next = last;
        last->previous = previous_node;
        return last;
    }

    void destroyHeadNode() noexcept {
        node_allocator_traits::destroy(alloc_, std::addressof(head_->next));
        node_allocator_traits::destroy(alloc_, std::addressof(head_->previous));
        node_allocator_traits::deallocate(alloc_, head_, 1);
    }

    node_pointer head_;
    size_type size_;
    node_allocator_type alloc_;

    friend iterator;
    friend const_iterator;
};

// template type deduction for input iterator constructor
template<class InputIter,
         class Allocator = std::allocator<IteratorValueTypeT<InputIter>>,
         typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
List(InputIter, InputIter, Allocator = Allocator())->List<IteratorValueTypeT<InputIter>, Allocator>;

}  // namespace nxt::core
