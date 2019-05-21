#pragma once

#include "CommonTree.h"

namespace nxt::core {

template<typename TreeTraits>
class AVLTree {
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
    using const_iterator = TreeConstIterator<AVLTree<TreeTraits>>;
    using iterator = TreeIterator<AVLTree<TreeTraits>>;

protected:
    using node_allocator_type = typename allocator_traits::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using tree_traits = TreeTraits;

public:
    AVLTree() noexcept(std::is_nothrow_default_constructible_v<node_allocator_type>&&
                           std::is_nothrow_default_constructible_v<compare_type>)
        : head_node_(nullptr)
        , size_(0)
        , compare_()
        , alloc_() {
        createHeadNode();
    }

    AVLTree(AVLTree&& rhs)
        : head_node_(nullptr)
        , size_(0)
        , compare_(rhs.compare_)
        , alloc_(std::move(rhs.alloc_)) {
        createHeadNode();
        using std::swap;
        swap(head_node_, rhs.head_node_);
        size_ = rhs.size_;
        rhs.size_ = 0;
    }

    AVLTree(const AVLTree&) {}

    std::pair<iterator, bool> insert(const value_type& value) {
        auto node = insert(head_node_->parent, head_node_, true, value);
        return {iterator(this, node), node != head_node_};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        auto node = insert(head_node_->parent, head_node_, true, std::move(value));
        return {iterator(this, node), node != head_node_};
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto node = node_allocator_traits::allocate(alloc_, 1);
        node_allocator_traits::construct(alloc_, std::addressof(node->value), std::forward<Args>(args)...);
        auto node = emplace(head_node_->parent, head_node_, true, node);
        return {iterator(this, node), node != head_node_};
    }

    size_type erase(const key_type& key) {
        auto node = findNode(key);
        if (node == head_node_) {
            return 0;
        } else {
            auto parent_node = node->parent;

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

    ~AVLTree() {
        clear();

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
        Node* parent;
        Node* left_child;
        Node* right_child;
        int32_t height;
    };

    template<typename Arg>
    Node* insert(Node* node, Node* parent, bool is_left, Arg&& arg) {
        if (node == head_node_ || node == nullptr) {
            auto node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(node->value), std::forward<Arg>(arg));

            node->parent = parent;
            node->left_child = nullptr;
            node->right_child = nullptr;
            node->height = 0;
            node->parent = parent;

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

            return node;
        }

        if (compare_(arg, tree_traits::key(node->value))) {
            auto result = insert(node->left_child, node, true, std::forward<Arg>(arg));
            balanceNode(node);
            return result;
        } else if (compare_(tree_traits::key(node->value), arg)) {
            auto result = insert(node->right_child, node, false, std::forward<Arg>(arg));
            balanceNode(node);
            return result;
        } else {
            return head_node_;
        }
    }

    Node* emplace(Node* node, Node* parent, bool is_left, Node* node_to_add) {
        if (node == head_node_ || node == nullptr) {
            node_to_add->parent = parent;
            node_to_add->height = 0;
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

            return node_to_add;
        }

        if (compare_(tree_traits::key(node_to_add->value), tree_traits::key(node->value))) {
            auto result = emplace(node->left_child, node, true, node_to_add);
            balanceNode(node);
            return result;
        } else if (compare_(tree_traits::key(node->value), tree_traits::key(node_to_add->value))) {
            auto result = emplace(node->right_child, node, false, node_to_add);
            balanceNode(node);
            return result;
        } else {
            node_allocator_traits::destroy(alloc_, std::addressof(node_to_add->value));
            node_allocator_traits::deallocate(alloc_, node_to_add, 1);
            return head_node_;
        }
    }

    template<typename Key>
    Node* lowerBoundNode(const Key& key) const {
        // intialize with end node
        Node* result = head_node_;
        Node* node = head_node_->parent;

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
    Node* upperBoundNode(const Key& key) const {
        // intialize with end node
        Node* result = head_node_;
        Node* node = head_node_->parent;

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
    Node* findNode(const Key& key) const {
        Node* node = head_node_->parent;

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
        head_node_->left_child = nullptr;
        head_node_->right_child = nullptr;
        head_node_->parent = head_node_;
        head_node_->height = -1;
    }

    void eraseNode(Node* node) {
        auto parent_node = node->parent;
        auto last_affected_node = head_node_;

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
            last_affected_node = parent_node;
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
            last_affected_node = parent_node;
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
            last_affected_node = parent_node;
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

            if (replace_parent == node) {
                last_affected_node = parent_node;
            } else {
                last_affected_node = replace_parent;
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

        while (last_affected_node != head_node_) {
            balanceNode(last_affected_node);
            last_affected_node = last_affected_node->parent;
        }

        node_allocator_traits::destroy(alloc_, std::addressof(node->value));
        node_allocator_traits::deallocate(alloc_, node, 1);
    }

    void freeTree(Node* node) noexcept {
        if (node != nullptr) {
            freeTree(node->left_child);
            freeTree(node->right_child);

            node_allocator_traits::destroy(alloc_, std::addressof(node->value));
            node_allocator_traits::deallocate(alloc_, node, 1);
        }
    }

    int32_t height(Node* node) const noexcept {
        if (node == nullptr) {
            return -1;
        } else {
            return node->height;
        }
    }

    void calculateHeight(Node* node) noexcept {
        node->height = std::max(height(node->left_child), height(node->right_child)) + 1;
    }

    void balanceNode(Node* node) {
        constexpr int32_t max_imbalance = 1;
        if (height(node->left_child) - height(node->right_child) > 1) {
            if (height(node->left_child->left_child) >= height(node->left_child->right_child)) {
                rotateOuterLeft(node);
            } else {
                rotateInnerLeft(node);
            }
        } else if (height(node->right_child) - height(node->left_child) > 1) {
            if (height(node->right_child->right_child) >= height(node->right_child->left_child)) {
                rotateOuterRight(node);
            } else {
                rotateInnerRight(node);
            }
        }

        calculateHeight(node);
    }

    void rotateOuterLeft(Node* node) {
        auto parent_node = node->parent;
        auto left_node = node->left_child;

        if (parent_node == head_node_) {
            parent_node->parent = left_node;
        } else {
            auto is_left = parent_node->left_child == node;
            if (is_left) {
                parent_node->left_child = left_node;
            } else {
                parent_node->right_child = left_node;
            }
        }
        left_node->parent = parent_node;

        node->left_child = left_node->right_child;
        if (node->left_child != nullptr) {
            node->left_child->parent = node;
        }

        left_node->right_child = node;
        node->parent = left_node;

        calculateHeight(node);
        calculateHeight(left_node);
    }

    void rotateOuterRight(Node* node) {
        auto parent_node = node->parent;
        auto right_node = node->right_child;

        if (parent_node == head_node_) {
            parent_node->parent = right_node;
        } else {
            auto is_left = parent_node->left_child == node;
            if (is_left) {
                parent_node->left_child = right_node;
            } else {
                parent_node->right_child = right_node;
            }
        }

        right_node->parent = parent_node;

        node->right_child = right_node->left_child;
        if (node->right_child != nullptr) {
            node->right_child->parent = node;
        }

        right_node->left_child = node;
        node->parent = right_node;

        calculateHeight(node);
        calculateHeight(right_node);
    }

    void rotateInnerLeft(Node* node) {
        auto parent_node = node->parent;
        auto left_node = node->left_child;
        auto inner_right_node = left_node->right_child;

        auto is_left = parent_node->left_child == node;

        if (is_left) {
            parent_node->left_child = inner_right_node;
        } else {
            parent_node->right_child = inner_right_node;
        }
        inner_right_node->parent = parent_node;

        left_node->right_child = inner_right_node->left_child;
        if (left_node->right_child != nullptr)
            left_node->right_child->parent = left_node;

        node->left_child = inner_right_node->right_child;
        if (node->left_child != nullptr)
            node->left_child->parent = node;

        inner_right_node->left_child = left_node;
        left_node->parent = inner_right_node;

        inner_right_node->right_child = node;
        node->parent = inner_right_node;

        calculateHeight(left_node);
        calculateHeight(node);
        calculateHeight(inner_right_node);
    }

    void rotateInnerRight(Node* node) {
        auto parent_node = node->parent;
        auto right_node = node->right_child;
        auto inner_left_node = right_node->left_child;

        auto is_left = parent_node->left_child == node;

        if (is_left) {
            parent_node->left_child = inner_left_node;
        } else {
            parent_node->right_child = inner_left_node;
        }
        inner_left_node->parent = parent_node;

        right_node->left_child = inner_left_node->right_child;
        if (right_node->right_child != nullptr)
            right_node->right_child->parent = right_node;

        node->right_child = inner_left_node->left_child;
        if (node->right_child != nullptr)
            node->right_child->parent = node;

        inner_left_node->left_child = node;
        node->parent = inner_left_node;

        inner_left_node->right_child = right_node;
        right_node->parent = inner_left_node;

        calculateHeight(right_node);
        calculateHeight(node);
        calculateHeight(inner_left_node);
    }

    Node* head_node_;
    size_type size_;
    compare_type compare_;
    node_allocator_type alloc_;

    friend iterator;
    friend const_iterator;
};

}  // namespace nxt::core