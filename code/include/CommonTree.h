#pragma once

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
        return this->node_->value;
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

}