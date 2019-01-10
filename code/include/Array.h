#pragma once

#include <iterator>
#include <memory>

namespace nxt {
namespace core {

/**

*/
template<typename T, std::size_t Size>
class Array {
public:
    static_assert(Size > 0, "Can't create a array with size 0, it leads to undefined behaviour");

    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    constexpr size_type size() const noexcept {
        return Size;
    }

    constexpr size_type max_size() const noexcept {
        return Size;
    }

    constexpr reference operator[](size_type index) {
        return data_[index];
    }

    constexpr const_reference operator[](size_type index) const {
        return data_[index];
    }

    constexpr bool empty() const noexcept {
        return Size == 0;
    }

    constexpr const_reference front() const noexcept {
        return data_[0];
    }

    constexpr reference front() noexcept {
        return data_[0];
    }

    constexpr pointer data() noexcept {
        return data_;
    }

    constexpr const_pointer data() const noexcept {
        return data_;
    }

    constexpr const_reference back() const noexcept {
        return data_[Size - 1];
    }

    constexpr reference back() noexcept {
        return data_[Size - 1];
    }

    constexpr iterator begin() noexcept {
        return data_;
    }

    constexpr const_iterator begin() const noexcept {
        return data_;
    }

    constexpr const_iterator cbegin() const noexcept {
        return data_;
    }

    constexpr iterator end() const noexcept {
        return (data_ + Size);
    }

    constexpr const_iterator end() const noexcept {
        return (data_ + Size);
    }

    constexpr const_iterator cend() const noexcept {
        return (data_ + Size);
    }

    T data_[N];
};

}  // namespace core
}  // namespace nxt
