#pragma once

namespace nxt::core {

template<typename T, std::size_t MaxDepth, typename Compare = std::less<>, typename Allocator = std::allocator<T>>
class SkipList {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;

    SkipList() 
    {
        createHeadNode();
    }

private:
    struct SkipNode;

    using node_type = SkipNode;
    using node_allocator_type = std::allocator_traits<allocator_type>::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer = typename node_allocator_traits::pointer;
    using node_const_pointer = typename node_allocator_traits::const_pointer;

    struct SkipNode {
        node_pointer links;
        size_type height;
        value_type value;
    };

    void createHeadNode() {
        head_node_ = node_allocator_traits(alloc_, 1);
        node_allocator_traits::
    }
    
    node_pointer head_node_;
    node_allocator_type alloc_;

};
}  // namespace nxt::core
