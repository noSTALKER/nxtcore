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

    SkipList() {}

};
}  // namespace nxt::core
