#pragma once

#include <iostream>
#include <set>

namespace nxt {
namespace core {

template <typename T, typename Comparator = std::less<T>, typename Allocator = std::allocator<T>>
class BinaryTree {
public:
    struct Node;

    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using node_type = Node;
    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;
    using size_type = typename allocator_traits::size_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using difference_type = typename allocator_traits::difference_type;

    BinaryTree();
    BinaryTree(Tree&&);
    BinaryTree(const Tree&);

    bool insert(const T& value);
    bool insert(T&& value);

    template<typename Args...>
	bool emplace(Args&& ... args) {

	}

private:
    struct Node {
        T value;
        Node* parent;
        Node* left_child;
        Node* right_child;

    };

    Node* root_node_;
    Node* leftmost_node_;
    Node* rightmost_node_;
    size_type size_;
    node_allocator_type alloc_;
};

}
}