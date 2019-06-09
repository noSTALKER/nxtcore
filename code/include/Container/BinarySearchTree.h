#pragma once

#include "CommonTree.h"
#include <xtree>

namespace nxt::core {

template<typename TreeTraits>
class BinarySearchTree {
private:
    struct Node;

public:
    using value_type = typename TreeTraits::value_type;
    using key_type = typename TreeTraits::key_type;
    using compare_type = typename TreeTraits::compare_type;
    using allocator_type = typename TreeTraits::allocator_type;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using node_type = Node;
    using size_type = typename allocator_traits::size_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using difference_type = typename allocator_traits::difference_type;
    using const_iterator = TreeConstIterator<BinarySearchTree<TreeTraits>>;
    using iterator = TreeIterator<BinarySearchTree<TreeTraits>>;

protected:
    using node_allocator_type = typename allocator_traits::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer = typename node_allocator_traits::pointer;
    using node_const_pointer = typename node_allocator_traits::const_pointer;
    using tree_traits = TreeTraits;

public:
    BinarySearchTree() noexcept(std::is_nothrow_default_constructible_v<node_allocator_type>&&
                                    std::is_nothrow_default_constructible_v<compare_type>)
        : head_node_()
        , size_(0)
        , compare_()
        , alloc_() {
        createHeadNode();
    }

    BinarySearchTree(BinarySearchTree&& rhs)
        : head_node_()
        , size_(0)
        , compare_(rhs.compare_)
        , alloc_(std::move(rhs.alloc_)) {
        createHeadNode();
        using std::swap;
        swap(head_node_, rhs.head_node_);
        size_ = rhs.size_;
        rhs.size_ = 0;
    }

    BinarySearchTree(const BinarySearchTree& rhs)
        : head_node_()
        , size_(0)
        , compare_(rhs.compare_)
        , alloc_(node_allocator_traits::select_on_container_copy_construction(rhs.alloc_)) {
        createHeadNode();
        if (rhs.size_ > 0) {
            auto root_node = copyTree(rhs.head_node_->parent);
            head_node_->parent = root_node;
            root_node->parent = head_node_;

            auto min_node = root_node;
            while (min_node->left_child != nullptr) {
                min_node = min_node->left_child;
            }
            head_node_->left_child = min_node;

            auto max_node = root_node;
            while (max_node->right_child != nullptr) {
                max_node = max_node->right_child;
            }
            head_node_->right_child = max_node;
        }

        size_ = rhs.size_;
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        return insert(head_node_->parent, head_node_, true, value);
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        return insert(head_node_->parent, head_node_, true, std::move(value));
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(node->value), std::forward<Args>(args)...);
        node_allocator_traits::construct(alloc_, std::addressof(node->parent));
        node_allocator_traits::construct(alloc_, std::addressof(node->left_child));
        node_allocator_traits::construct(alloc_, std::addressof(node->right_child));

        return emplace(head_node_->parent, head_node_, true, node);
    }

    size_type erase(const key_type& key) {
        auto node = findNode(key);
        if (node == head_node_) {
            return 0;
        } else {
            eraseNode(node);
            return 1;
        }
    }

    void clear() noexcept {
        if (head_node_->parent != head_node_) {
            freeTree(head_node_->parent);

            head_node_->parent = head_node_;
            head_node_->left_child = nullptr;
            head_node_->right_child = nullptr;

            size_ = 0;
        }
    }

    ~BinarySearchTree() {
        clear();

        node_allocator_traits::destroy(alloc_, std::addressof(head_node_->parent));
        node_allocator_traits::destroy(alloc_, std::addressof(head_node_->left_child));
        node_allocator_traits::destroy(alloc_, std::addressof(head_node_->right_child));
        node_allocator_traits::deallocate(alloc_, head_node_, 1);
    }

    template<typename Key, typename = typename compare_type::is_transparent>
    iterator erase(const Key& key) {}

    [[nodiscard]] const_iterator find(const key_type& value) const {
        return const_iterator(this, findNode(value));
    }

    [[nodiscard]] iterator find(const key_type& value) {
        return iterator(this, findNode(value));
    }

    template<typename Key, typename = typename compare_type::is_transparent>
    [[nodiscard]] const_iterator find(const Key& value) const {
        return const_iterator(this, findNode(value));
    }

    template<typename Key, typename = typename compare_type::is_transparent>
    [[nodiscard]] iterator find(const Key& value) {
        return iterator(this, findNode(value));
    }

    [[nodiscard]] iterator begin() {
        return iterator(this, head_node_->left_child);
    }

    [[nodiscard]] const_iterator begin() const {
        return const_iterator(this, head_node_->left_child);
    }

    [[nodiscard]] const_iterator cbegin() const {
        return const_iterator(this, head_node_->left_child);
    }

    [[nodiscard]] iterator end() {
        return iterator(this, head_node_);
    }

    [[nodiscard]] const_iterator end() const {
        return const_iterator(this, head_node_);
    }

    [[nodiscard]] const_iterator cend() const {
        return const_iterator(this, head_node_);
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

private:
    struct Node {
        value_type value;
        node_pointer parent;
        node_pointer left_child;
        node_pointer right_child;
    };

    template<typename Arg>
    std::pair<iterator, bool> insert(node_pointer node, node_pointer parent, bool is_left, Arg&& arg) {
        if (node == head_node_ || node == nullptr) {
            auto node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(node->value), std::forward<Arg>(arg));

            node_allocator_traits::construct(alloc_, std::addressof(node->parent), parent);
            node_allocator_traits::construct(alloc_, std::addressof(node->left_child));
            node_allocator_traits::construct(alloc_, std::addressof(node->right_child));

            if (parent == head_node_) {
                head_node_->parent = node;
                head_node_->left_child = node;
                head_node_->right_child = node;
            } else if (is_left) {
                parent->left_child = node;
                if (parent == head_node_->left_child)
                    head_node_->left_child = node;
            } else {
                parent->right_child = node;
                if (parent == head_node_->right_child)
                    head_node_->right_child = node;
            }

            ++size_;

            return {iterator(this, node), true};
        }

        if (compare_(arg, tree_traits::key(node->value))) {
            return insert(node->left_child, node, true, std::forward<Arg>(arg));
        } else if (compare_(tree_traits::key(node->value), arg)) {
            return insert(node->right_child, node, false, std::forward<Arg>(arg));
        } else {
            return {iterator(this, head_node_), false};
        }
    }

    std::pair<iterator, bool> emplace(node_pointer node, node_pointer parent, bool is_left, node_pointer node_to_add) {
        if (node == head_node_ || node == nullptr) {
            node_to_add->parent = parent;
            node_to_add->left_child = nullptr;
            node_to_add->right_child = nullptr;
            node_to_add->parent = parent;

            if (parent == head_node_) {
                head_node_->parent = node_to_add;
                head_node_->left_child = node_to_add;
                head_node_->right_child = node_to_add;
            } else if (is_left) {
                parent->left_child = node_to_add;
                if (parent == head_node_->left_child)
                    head_node_->left_child = node_to_add;
            } else {
                parent->right_child = node_to_add;
                if (parent == head_node_->right_child)
                    head_node_->right_child = node_to_add;
            }

            ++size_;

            return {iterator(this, node_to_add), true};
        }

        if (compare_(tree_traits::key(node_to_add->value), tree_traits::key(node->value))) {
            return emplace(node->left_child, node, true, node_to_add);
        } else if (compare_(tree_traits::key(node->value), tree_traits::key(node_to_add->value))) {
            return emplace(node->right_child, node, false, node_to_add);
        } else {
            node_allocator_traits::destroy(alloc_, std::addressof(node_to_add->value));
            node_allocator_traits::destroy(alloc_, std::addressof(node_to_add->parent));
            node_allocator_traits::destroy(alloc_, std::addressof(node_to_add->left_child));
            node_allocator_traits::destroy(alloc_, std::addressof(node_to_add->right_child));

            node_allocator_traits::deallocate(alloc_, node_to_add, 1);
            return (iterator(this, head_node_), false);
        }
    }

    template<typename Key>
    node_pointer lowerBoundNode(const Key& key) const {
        // intialize with end node
        node_pointer result = head_node_;
        node_pointer node = head_node_->parent;

        while (node != nullptr) {
            if (compare_(tree_traits::key(node->value), key)) {
                node = node->left_child;
            } else {
                // if compare fails, then we update the result with the node
                // as it points to a value not less than key
                result = node;
                node = node->right_child;
            }
        }

        return result;
    }

    template<typename Key>
    node_pointer upperBoundNode(const Key& key) const {
        // intialize with end node
        node_pointer result = head_node_;
        node_pointer node = head_node_->parent;

        while (node != nullptr) {
            if (compare_(key, tree_traits::key(node->value))) {
                result = node;
                node = node->left_child;
            } else {
                node = node->right_child;
            }
        }

        return result;
    }

    template<typename Key>
    node_pointer findNode(const Key& key) const {
        node_pointer node = head_node_->parent;

        while (node != nullptr) {
            if (compare_(key, tree_traits::key(node->value))) {
                node = node->left_child;
            } else if (compare_(tree_traits::key(node->value), key)) {
                node = node->right_child;
            } else {
                return node;
            }
        }

        return head_node_;
    }

    void createHeadNode() {
        head_node_ = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(head_node_->parent), head_node_);
        node_allocator_traits::construct(alloc_, std::addressof(head_node_->left_child));
        node_allocator_traits::construct(alloc_, std::addressof(head_node_->right_child));
    }

    void eraseNode(node_pointer node) {
        auto parent_node = node->parent;

        if (node->left_child == nullptr && node->right_child == nullptr) {
            // if the node being removed has no childs, then just remove the link
            // in the parent node
            if (parent_node == head_node_) {
                head_node_->left_child = nullptr;
                head_node_->right_child = nullptr;
                head_node_->parent = head_node_;
            } else if (parent_node->left_child == node) {
                parent_node->left_child = nullptr;
            } else {
                parent_node->right_child = nullptr;
            }
        } else if (node->left_child != nullptr && node->right_child == nullptr) {
            // if node being removed has a left child only, move up the left child so that it
            // it is child of parent node now
            auto replace_node = node->left_child;
            replace_node->parent = parent_node;
            if (parent_node == head_node_) {
                head_node_->parent = replace_node;
            } else if (parent_node->left_child == node) {
                parent_node->left_child = replace_node;
            } else {
                parent_node->right_child = replace_node;
            }
        } else if (node->right_child != nullptr && node->left_child == nullptr) {
            // if node being removed has a right child only, move up the right child so that it
            // it is child of parent node now
            auto replace_node = node->right_child;
            replace_node->parent = parent_node;
            if (parent_node == head_node_) {
                head_node_->parent = replace_node;
            } else if (parent_node->left_child == node) {
                parent_node->left_child = replace_node;
            } else {
                parent_node->right_child = replace_node;
            }
        } else {
            // if both child are present, find the max element in the left subtree
            auto replace_node = node->left_child;
            while (replace_node->right_child != nullptr) {
                replace_node = replace_node->right_child;
            }

            auto replace_parent = replace_node->parent;
            if (replace_parent->left_child == replace_node) {
                replace_parent->left_child = replace_node->left_child;
                if (replace_parent->left_child != nullptr)
                    replace_parent->left_child->parent = replace_parent;
            } else {
                replace_parent->right_child = replace_node->left_child;
                if (replace_parent->right_child != nullptr)
                    replace_parent->right_child->parent = replace_parent;
            }

            replace_node->left_child = node->left_child;
            if (replace_node->left_child != nullptr)
                replace_node->left_child->parent = replace_node;

            replace_node->right_child = node->right_child;
            if (replace_node->right_child != nullptr)
                replace_node->right_child->parent = replace_node;

            replace_node->parent = parent_node;
            if (parent_node == head_node_) {
                head_node_->parent = replace_node;
            } else if (parent_node->left_child == node) {
                parent_node->left_child = replace_node;
            } else {
                parent_node->left_child = replace_node;
            }
        }

        if (node == head_node_->left_child) {
            if (node->parent != head_node_) {
                head_node_->left_child = node->parent;
            } else {
                head_node_->left_child = head_node_->parent;
            }
        }

        if (node == head_node_->right_child) {
            if (node->parent != head_node_) {
                head_node_->right_child = node->parent;
            } else {
                head_node_->right_child = head_node_->parent;
            }
        }

        --size_;

        node_allocator_traits::destroy(alloc_, std::addressof(node->value));
        node_allocator_traits::destroy(alloc_, std::addressof(node->parent));
        node_allocator_traits::destroy(alloc_, std::addressof(node->left_child));
        node_allocator_traits::destroy(alloc_, std::addressof(node->right_child));

        node_allocator_traits::deallocate(alloc_, node, 1);
    }

    void freeTree(node_pointer node) noexcept {
        if (node != nullptr) {
            freeTree(node->left_child);
            freeTree(node->right_child);

            node_allocator_traits::destroy(alloc_, std::addressof(node->value));
            node_allocator_traits::destroy(alloc_, std::addressof(node->parent));
            node_allocator_traits::destroy(alloc_, std::addressof(node->left_child));
            node_allocator_traits::destroy(alloc_, std::addressof(node->right_child));

            node_allocator_traits::deallocate(alloc_, node, 1);
        }
    }

    node_pointer copyTree(node_pointer node) {
        auto copy_node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(copy_node->value), node->value);
        node_allocator_traits::construct(alloc_, std::addressof(copy_node->parent));
        node_allocator_traits::construct(alloc_, std::addressof(copy_node->left_child));
        node_allocator_traits::construct(alloc_, std::addressof(copy_node->right_child));

        if (node->left_child != nullptr) {
            auto left_child = copyTree(node->left_child);
            copy_node->left_child = left_child;
            left_child->parent = copy_node;
        }

        if (node->right_child != nullptr) {
            auto right_child = copyTree(node->right_child);
            copy_node->right_child = right_child;
            right_child->parent = copy_node;
        }

        return copy_node;
    }

    node_pointer head_node_;
    size_type size_;
    compare_type compare_;
    node_allocator_type alloc_;

    friend iterator;
    friend const_iterator;
};

}  // namespace nxt::core