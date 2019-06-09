#pragma once

#include <iterator>

namespace nxt::core {

template<class T, std::size_t Size>
class Array {
public:
    static_assert(Size > 0, "Can't create Array with size == 0");

	// define all type definitions used by different traits like iterator_traits and pointer traits
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    [[nodiscard]] constexpr size_type size() const noexcept {
        return Size;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return Size;
    }

    [[nodiscard]] constexpr reference operator[](size_type index) {
        return data_[index];
    }

    [[nodiscard]] constexpr const_reference operator[](size_type index) const {
        return data_[index];
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return Size == 0;
    }

    [[nodiscard]] constexpr const_reference front() const noexcept {
        return data_[0];
    }

    [[nodiscard]] constexpr reference front() noexcept {
        return data_[0];
    }

    [[nodiscard]] constexpr pointer data() noexcept {
        return data_;
    }

    [[nodiscard]] constexpr const_pointer data() const noexcept {
        return data_;
    }

    [[nodiscard]] constexpr const_reference back() const noexcept {
        return data_[Size - 1];
    }

    [[nodiscard]] constexpr reference back() noexcept {
        return data_[Size - 1];
    }

    [[nodiscard]] constexpr iterator begin() noexcept {
        return data_;
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return data_;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return data_;
    }

    [[nodiscard]] constexpr iterator end() noexcept {
        return (data_ + Size);
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return (data_ + Size);
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return (data_ + Size);
    }

	// the field is public so that it can be used for aggregate initialization
    T data_[Size];
};

}  // namespace nxt::core
