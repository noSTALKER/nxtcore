#pragma once

#include <iostream>
#include <map>
#include <set>

namespace nxt::core {

template<typename Tree>
class TreeConstIterator {
public:
    using value_type = typename Tree::value_type;
    using difference_type = typename Tree::difference_type;
    using pointer = typename Tree::const_pointer;
    using reference = typename Tree::const_reference;
    using difference_type = typename Tree::difference_type;
    using node_type = typename Tree::node_type;
    using tree_type = Tree;
    using iterator_category = std::bidirectional_iterator_tag;

    TreeConstIterator(tree_type* tree, node_type* node)
        : tree_(tree)
        , node_(node) {}

    [[nodiscard]] reference operator*() const {
        return node->value;
    }

    [[nodiscard]] pointer operator->() const {
        return std::pointer_traits<pointer>::pointer_to(node->value);
    }

    TreeConstIterator& operator++() {
        if (node_->right_child != nullptr) {
            // find the min element in right subtree
            auto min_node = node_->right_child;
            while (min_node->left_child != nullptr) {
                min_node = min_node->left_child;
            }
            node_ = min_node;
        } else {
            auto parent_node = node_->parent;
            while (parent_node->parent != nullptr && node_ == parent_node->right_child) {
                node_ = parent_node;
                parent_node = node_->parent;
            }

            node_ = parent_node;
        }

        return *this;
    }

    [[nodiscard]] TreeConstIterator operator++(int) {
        TreeConstIterator result(tree_, node_);
        this->operator++();
        return result;
    }

    TreeConstIterator& operator--() {
        if (node_ == tree_->head_node_) {
            node_ = head_node_->right_child;
        } else if (node_->left_child != nullptr) {
            auto max_node = node_->left_child;
            while (max_node->right_child != nullptr) {
                max_node = result->right_child;
            }

            node_ = max_node;
        } else {
            node_type* parent_node = node_->parent;
            while (parent_node != nullptr && node_ == parent_node->left_child) {
                node_ = parent_node;
                parent_node = node_->parent;
            }

            if (node_ != head_node_) {
                node_ = parent_node;
            }
        }
    }

    [[nodiscard]] TreeConstIterator operator--(int) {
        TreeConstIterator result(tree_, node_);
        this->operator--();
        return result;
    }

    [[nodiscard]] bool operator==(const TreeConstIterator& rhs) {
        return node_ == rhs.node_;
    }

    [[nodiscard]] bool operator!=(const TreeConstIterator& rhs) {
        return node_ != rhs.node_;
    }

protected:
    tree_type* tree_;
    node_type* node_;
};

template<typename Key, typename Compare = std::less<>, typename Allocator = std::allocator<Key>>
struct SimpleTreeTraits {
    using value_type = Key;
    using key_type = Key;
    using reference = value_type&;
    using const_reference = const value_type&;
	using compare_type = Compare;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;

	static const key_type& key(const value_type& value) {
        return value;
	}
};

template<typename Tree>
class TreeIterator : public TreeConstIterator<Tree> {
public:
    using value_type = typename Tree::value_type;
    using difference_type = typename Tree::difference_type;
    using pointer = typename Tree::pointer;
    using reference = typename Tree::reference;
    using difference_type = typename Tree::difference_type;
    using node_type = typename Tree::node_type;
    using tree_type = Tree;
    using iterator_category = std::bidirectional_iterator_tag;
    using Base = TreeConstIterator<Tree>;

    TreeIterator(tree_type* tree, node_type* node)
        : Base(tree, node) {}

    [[nodiscard]] reference operator*() const {
        return node_->value;
    }

    [[nodiscard]] pointer operator->() const {
        return std::pointer_traits<pointer>::pointer_to(node_->value);
    }

    TreeIterator& operator++() {
        ++(static_cast<Base&>(*this));
        return *this;
    }

    [[nodiscard]] TreeIterator operator++(int) {
        TreeIterator result(tree_, node_);
        this->operator++();
        return result;
    }

    TreeIterator& operator--() {
        --(static_cast<Base&>(*this));
        return *this;
    }

    [[nodiscard]] TreeIterator operator--(int) {
        TreeIterator result(tree_, node_);
        this->operator--();
        return result;
    }
};

template<typename TreeTraits>
class BinarySearchTree {
public:
    struct Node;

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
    using tree_traits = TreeTraits;

public:
    BinarySearchTree() noexcept(std::is_nothrow_default_constructible_v<node_allocator_type>)
        : head_node_(nullptr)
        , size_(0)
        , alloc_() {
        createHeadNode();
    }

    BinarySearchTree(BinarySearchTree&&) {}
    BinarySearchTree(const BinarySearchTree&) {}

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
        return emplace(head_node_->parent, head_node_, true, node);
    }

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
    };

    template<typename Arg>
    std::pair<iterator, bool> insert(Node* node, Node* parent, bool is_left, Arg&& arg) {
        if (node == nullptr) {
            auto node = node_allocator_traits::allocate(alloc_, 1);
            node_allocator_traits::construct(alloc_, std::addressof(node->value), std::forward<Arg>(arg));

            node->parent = parent;
            node->left_child = nullptr;
            node->right_child = nullptr;
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

    std::pair<iterator, bool> emplace(Node* node, Node* parent, bool is_left, Node* node_to_add) {
        if (node == nullptr) {
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
            node_allocator_traits::deallocate(alloc_, node_to_add, 1);
            return (iterator(this, head_node_), false);
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
        head_node_->parent = nullptr;
    }

    Node* head_node_;
    size_type size_;
    node_allocator_type alloc_;
    compare_type compare_;

    friend iterator;
    friend const_iterator;
};

}  // namespace nxt::core