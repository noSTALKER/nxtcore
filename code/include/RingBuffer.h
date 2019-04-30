#pragma once

#include <iterator>

namespace nxt::core {

template<typename T, typename Allocator = std::allocator<T>>
class RingBuffer {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;

    RingBuffer() noexcept(std::is_nothrow_default_constructible_v<allocator_type>)
		:alloc_() {}



private:
    allocator_type alloc_;
};
}  // namespace nxt::core