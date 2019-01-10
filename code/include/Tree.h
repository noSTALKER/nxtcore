#pragma once

#include <iostream>

namespace nxt {
namespace core {

template <typename T, typename Comparator = std::less<T>, typename Allocator = std::allocator<T>>
class BinaryTree {
public:
    BinaryTree();
    BinaryTree(Tree&&);
    BinaryTree(const Tree&);

    bool insert(const T& value);
    bool insert(T&& value);

    template<Args args...>
    bool emplace(Args&&... args)

private:
    struct Node {
        T value;
        Node* left_child;
        Node* right_child;
    };

    Node* root_node_;
};

}
}