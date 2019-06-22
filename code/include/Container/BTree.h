#pragma once

#include <memory>
#include "Buffer.h"

namespace nxt::core {

template<typename Key,
         typename Value,
         std::size_t BlockSize,
         typename Compare = std::less<>,
         typename Allocator = std::allocator<std::pair<const Key, Value>>>
class BTree {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, value_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using compare_type = Compare;

private:
    struct InternalNode;
    struct LeafNode;

    struct InternalNode {
        Buffer<key_type>
    };

    internal_node_allocator internal_node_alloc_;
    leaf_node_allocator leaf_node_alloc_;

};
}  // namespace nxt::core
