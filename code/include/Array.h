#pragma once

#include <iterator>
#include <memory>

namespace nxt {
namespace core {

template<class T, std::size_t Size>
class Array {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    [[nodiscard]]
    constexpr size_type size() const noexcept {
        return Size;
    }

    [[nodiscard]]
    constexpr size_type max_size() const noexcept {
        return Size;
    }

    [[nodiscard]]
    constexpr reference operator[](size_type index) {
        return data_[index];
    }

    [[nodiscard]]
    constexpr const_reference operator[](size_type index) const {
        return data_[index];
    }

    [[nodiscard]]
    constexpr bool empty() const noexcept {
        return Size == 0;
    }

    [[nodiscard]]
    constexpr const_reference front() const noexcept {
        return data_[0];
    }

    [[nodiscard]]
    constexpr reference front() noexcept {
        return data_[0];
    }

    [[nodiscard]]
    constexpr pointer data() noexcept {
        return data_;
    }

    [[nodiscard]]
    constexpr const_pointer data() const noexcept {
        return data_;
    }

    [[nodiscard]]
    constexpr const_reference back() const noexcept {
        return data_[Size - 1];
    }

    [[nodiscard]]
    constexpr reference back() noexcept {
        return data_[Size - 1];
    }

    [[nodiscard]]
    constexpr iterator begin() noexcept {
        return data_;
    }

    [[nodiscard]]
    constexpr const_iterator begin() const noexcept {
        return data_;
    }

    [[nodiscard]]
    constexpr const_iterator cbegin() const noexcept {
        return data_;
    }

    [[nodiscard]]
    constexpr iterator end() const noexcept {
        return (data_ + Size);
    }

    [[nodiscard]]
    constexpr const_iterator end() const noexcept {
        return (data_ + Size);
    }

    [[nodiscard]]
    constexpr const_iterator cend() const noexcept {
        return (data_ + Size);
    }

    T data_[Size];
};

}  // namespace core
}  // namespace nxt
