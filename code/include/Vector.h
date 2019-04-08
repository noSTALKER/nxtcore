#pragma once
#include <type_traits>

namespace nxt {
namespace core {
template<typename T, typename Allocator = std::allocator<T>>
class Vector {
    using alloc_traits = typename std::allocator_traits<Allocator>;

public:
    using value_type = T;
    using size_type = typename alloc_traits::size_type;
    using difference_type = typename alloc_traits::difference_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using allocator_type = Allocator;

    Vector() noexcept(std::is_noexcept_default_constructible_v<Allocator>)
        : size_(0)
        , capacity(0)
        , data_(nullptr)
        , allocator_() {}

    [[nodiscard]] iterator begin() noexcept {
        return data_;
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return data_;
    }

	[[nodiscard]] const_iterator cbegin() const noexcept {
        return data_;
    }

    [[nodiscard]] iterator end() noexcept {
        return data_ + size_;
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return data_ + size_;
    }

	[[nodiscard]] const_iterator cend() const noexcept {
        return data_ + size_;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size == 0;
    }

    [[nodiscard]] reference operator[](size_type position) {
        return data_[position];
    }

    [[nodiscard]] const_reference operator[](size_type position) {
        return data_[position];
    }

private:
    pointer data_;
    size_type size_;
    size_type capacity_;
    allocator_type allocator_;
};
}  // namespace core
}  // namespace nxt

