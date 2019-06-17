#pragma once

#include <allocators>

namespace nxt::core {

template<typename SkipList>
class SkipListConstIterator {
public:
    using value_type = typename SkipList::value_type;
    using reference = typename SkipList::const_reference;
    using pointer = typename SkipList::const_pointer;
    using node_type = typename SkipList::node_type;
    using node_pointer = typename SkipList::node_pointer;
    using iterator_category = std::forward_iterator_tag;

    SkipListConstIterator(SkipList* list, node_pointer node)
        : list_(list)
        , node_(node) {}

    SkipListConstIterator& operator++() {
        node_ = node_->links[0];
        return *this;
    }

    [[nodiscard]] SkipListConstIterator operator++(int) {
        SkipListConstIterator result(list, node);
        node_ = node->links[0];
        return result;
    }

    [[nodiscard]] reference operator*() const {
        return node->value;
    }

    [[nodiscard]] pointer operator->() const {
        return std::pointer_traits<pointer>::pointer_to(node->value);
    }

    [[nodiscard]] operator==(const ConstSkipListIterator& rhs) {
        return node_ == rhs.node_ && list_ == rhs.list_;
    }

    [[nodiscard]] operator!=(const ConstSkipListIterator& rhs) {
        return node_ != rhs.node_ || list_ != rhs.list_;
    }

protected:
    SkipList* list_;
    node_pointer node_;
};

template<typename SkipList>
class SkipListIterator : public SkipListConstIterator<SkipList> {
public:
    using value_type = typename SkipList::value_type;
    using reference = typename SkipList::reference;
    using pointer = typename SkipList::pointer;
    using node_type = typename SkipList::node_type;
    using node_pointer = typename SkipList::node_pointer;
    using iterator_category = std::forward_iterator_tag;
    using base_class = SkipListConstIterator<SkipList>

    SkipListIterator(SkipList* list, node_pointer node)
        : list_(list)
    , node_(node) {}

    SkipListIterator& operator++() {
        node_ = node_->links[0];
        return *this;
    }

    [[nodiscard]] SkipListIterator operator++(int) {
        SkipListIterator result(list, node);
        node_ = node->links[0];
        return result;
    }

    [[nodiscard]] reference operator*() const {
        return node_->value;
    }

    [[nodiscard]] pointer operator->() const {
        return std::pointer_traits<pointer>::pointer_to(node_->value);
    }

protected:
    using base_class::list_;
    using base_class::node_;
};

class RandomGenerator {
public:
    std::size_t operator()(std::size_t max_value) {
        return max_value;
    }

private:
};

template<typename Traits, typename Random = RandomGenerator>
class SkipList {
public:
    using value_type = typename Traits::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using allocator_type = typename Traits::allocator_type;
    using key_type = typename Traits::key_type;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using random_type = Random;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using compare_type = typename Traits::Compare;
    using iterator = SkipListIterator<SkipList>;
    using const_iterator = SkipListConstIterator<SkipList>;

    SkipList()
        : size_(0)
        , compare_() I
        , alloc_() {
        createHeadNode();
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        if (size_ > 0)
            return const_iterator(this, head_node_->links[0]);
        else
            return const_iterator(this, nullptr);
    }

    [[nodiscard]] iterator begin() const noexcept {
        if (size_ > 0)
            return iterator(this, head_node_->links[0]);
        else
            return iterator(this, nullptr);
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        if (size_ > 0)
            return const_iterator(this, head_node_->links[0]);
        else
            return const_iterator(this, nullptr);
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this, nullptr);
    }

    [[nodiscard]] iterator end() noexcept {
        return iterator(this, nullptr);
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator(this, nullptr);
    }

    [[nodiscard]] iterator find(const key_type& value) {
        return iterator(this, findNode(value));
    }

    [[nodiscard]] const_iterator find(const key_type& value) {
        return const_iterator(this, findNode(value));
    }

    template<typename Key, typename = typename compare_type::is_transparent>
    [[nodiscard]] iterator find(const Key& value) {
        return iterator(this, findNode(value));
    }

    template<typename Key, typename = typename compare_type::is_transparent>
    [[nodiscard]] const_iterator find(const Key& value) const {
        return const_iterator(this, findNode(value));
    }

private:
    struct SkipNode;

    using node_type = SkipNode;
    using node_allocator_type = std::allocator_traits<allocator_type>::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer = typename node_allocator_traits::pointer;
    using node_const_pointer = typename node_allocator_traits::const_pointer;

    struct SkipNode {
        Vector<node_pointer> links;
        value_type value;
    };

    void createHeadNode() {
        head_node_ = node_allocator_traits(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(head_node_.links));
    }

    template<typename U>
    node_pointer insertNode(U&& value) {
        auto 

    }

    template<typename Key>
    node_pointer findNode(const Key& value) {
        auto height = head_node_.links.size();
        auto current_node = head_node_;
        for (difference_type i = height - 1; i >= 0; --i) {
            while (current_node.links[i] != nullptr) {
                if (comp_(value, current_node.links[i].value)) {
                    break;
                } else if (comp_(current_node.links[i].value, value)) {
                    current_node = current_node.links[i];
                } else {
                    return current_node.links[i];
                }
            }
        }
        return nullptr;
    }

    node_pointer head_node_;
    size_type size_;
    compare comp_;
    node_allocator_type alloc_;
};
}  // namespace nxt::core
