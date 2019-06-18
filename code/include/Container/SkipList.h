#pragma once

#include "Vector.h"

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

    [[nodiscard]] bool operator==(const SkipListConstIterator& rhs) {
        return node_ == rhs.node_ && list_ == rhs.list_;
    }

    [[nodiscard]] bool operator!=(const SkipListConstIterator& rhs) {
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
    using base_class = SkipListConstIterator<SkipList>;

    SkipListIterator(SkipList* list, node_pointer node)
        : base_class(list, node) {}

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
    using compare_type = typename Traits::compare_type;
    using iterator = SkipListIterator<SkipList>;
    using const_iterator = SkipListConstIterator<SkipList>;
    using traits = Traits;

private:
    struct SkipNode;

    using node_type = SkipNode;
    using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer = typename node_allocator_traits::pointer;
    using node_const_pointer = typename node_allocator_traits::const_pointer;

    struct SkipNode {
        Vector<node_pointer> links;
        value_type value;
    };

public:
    SkipList()
        : size_(0)
        , comp_()
        , random_()
        , alloc_() {
        createHeadNode();
    }

    SkipList(SkipList&& rhs)
        : size_(0)
        , comp_(rhs.comp_)
        , random_(rhs.random_)
        , alloc_(std::move(rhs.alloc_)) {
        createHeadNode();
        using std::swap;
        swap(head_node_, rhs.head_node_);
        size_ = rhs.size_;
        rhs.size_ = 0;
    }

    SkipList(const SkipList& rhs)
        : size_(0)
        , comp_(rhs.comp_)
        , random_(rhs.random_)
        , alloc_(node_allocator_traits::select_on_container_copy_construction(rhs.alloc_)) {
        createHeadNode();
        using std::swap;
        for (const value& : rhs) {
            insert(value);
        }
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_type height() const noexcept {
        return head_node_.links.size();
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        if (size_ > 0)
            return const_iterator(this, head_node_->links[0]);
        else
            return const_iterator(this, nullptr);
    }

    [[nodiscard]] iterator begin() noexcept {
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

    [[nodiscard]] const_iterator find(const key_type& value) const {
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

    std::pair<iterator, bool> insert(const value_type& value) {
        auto result = insertNode(value);
        return {iterator(this, result), result != nullptr};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        auto result = insertNode(std::move(value));
        return {iterator(this, result), result != nullptr};
    }

    size_type erase(const key_type& value) {
        return eraseNode(value);
    }

    size_type erase(const_iterator position) {
        return erase(traits::key(*position));
    }

    void clear() {
        if (size_ > 0) {
            auto current_node = head_node_->links[0];
            while (current_node != nullptr) {
                auto next_node = current_node->links[0];
                node_allocator_traits::destroy(alloc_, std::addressof(current_node->value));
                node_allocator_traits::destroy(alloc_, std::addressof(current_node->links));
                node_allocator_traits::deallocate(alloc_, current_node, 1);

                current_node = next_node;
            } 
            size_ = 0;
            head_node_->links.clear();
        }
    }

    ~SkipList() {
        clear();

        node_allocator_traits::destroy(alloc_, std::addressof(head_node_->links));
        node_allocator_traits::deallocate(alloc_, head_node_, 1);
    }

private:
    void createHeadNode() {
        head_node_ = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(head_node_->links));
    }

    template<typename Arg>
    node_pointer insertNode(Arg&& value) {
        Vector<node_pointer> update_links(head_node_->links.size(), nullptr);
        auto current_node = head_node_;
        for (difference_type i = head_node_->links.size() - 1; i >= 0; --i) {
            while (current_node->links[i] != nullptr) {
                if (comp_(traits::key(current_node->links[i]->value), traits::key(value))) {
                    current_node = current_node->links[i];
                } else if (comp_(traits::key(value), traits::key(current_node->links[i]->value))) {
                    break;
                } else {
                    // found a duplicate
                    return nullptr;
                }
            }
            update_links[i] = current_node;
        }

        difference_type height = random_(head_node_->links.size() + 1);

        auto new_node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->links), height, nullptr);
        node_allocator_traits::construct(alloc_, std::addressof(new_node->value), std::forward<Arg>(value));

        if (height > head_node_->links.size()) {
            head_node_->links.resize(height);
            head_node_->links[height - 1] = new_node;
            new_node->links[height - 1] = nullptr;
            --height;
        }

        for (difference_type i = height - 1; i >= 0; --i) {
            new_node->links[i] = update_links[i]->links[i];
            update_links[i]->links[i] = new_node;
        }
        ++size_;
        return new_node;
    }

    template<typename Key>
    node_pointer findNode(const Key& value) {
        auto height = head_node_->links.size();
        auto current_node = head_node_;
        for (difference_type i = height - 1; i >= 0; --i) {
            while (current_node->links[i] != nullptr) {
                if (comp_(traits::key(value), traits::key(current_node->links[i]->value))) {
                    break;
                } else if (comp_(traits::key(current_node->links[i]->value), traits::key(value))) {
                    current_node = current_node->links[i];
                } else {
                    return current_node->links[i];
                }
            }
        }
        return nullptr;
    }

    size_t eraseNode(const key_type& value) {
        node_pointer node = nullptr;
        Vector<node_pointer> update_links(head_node_->links.size(), nullptr);
        auto current_node = head_node_;
        for (difference_type i = head_node_->links.size() - 1; i >= 0; --i) {
            while (current_node->links[i] != nullptr) {
                if (comp_(traits::key(current_node->links[i]->value), value)) {
                    current_node = current_node->links[i];
                } else if (comp_(value, traits::key(current_node->links[i]->value))) {
                    break;
                } else {
                    // found a duplicate
                    node = current_node->links[i];
                    break;
                }
            }
            update_links[i] = current_node;
        }

        if (node != nullptr) {
            auto height = node->links.size();
            for (difference_type i = height - 1; i >= 0; --i) {
                update_links[i]->links[i] = node->links[i];
            }

            node_allocator_traits::destroy(alloc_, std::addressof(node->value));
            node_allocator_traits::destroy(alloc_, std::addressof(node->links));
            node_allocator_traits::deallocate(alloc_, node, 1);
            --size_;
            return 1;
        } else {
            return 0;
        }
    }

    node_pointer head_node_;
    size_type size_;
    compare_type comp_;
    random_type random_;
    node_allocator_type alloc_;

    friend iterator;
    friend const_iterator;
};
}  // namespace nxt::core
