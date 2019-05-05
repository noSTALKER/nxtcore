#pragma once
#include <memory>

#include "Common.h"

namespace nxt::core {

template<typename T, std::size_t Size, std::size_t Alignment = alignof(T)>
struct Buffer {
    static_assert(Size > 0, "Can't create a Buffer of size == 0");

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = pointer;
    using const_iterator = const_pointer;

    constexpr static size_type total_size = Size * sizeof(T);
    constexpr static size_type type_size = sizeof(T);
    constexpr static size_type alignment = Alignment;
    constexpr static size_type size = Size;

    [[nodiscard]] const_reference operator[](size_type index) const {
        return data()[index];
    }

    [[nodiscard]] reference operator[](size_type index) {
        return data()[index];
    }

    [[nodiscard]] const_pointer pointerAt(size_type index) const noexcept {
        return data() + index;
    }

    [[nodiscard]] pointer pointerAt(size_type index) noexcept {
        return data() + index;
    }

    template<typename... Args>
    void construct(size_type index, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        new (pointerAt(index)) T(std::forward<Args>(args)...);
    }

    void construct(size_type index, DefaultInitializeTag) noexcept(std::is_nothrow_default_constructible_v<T>) {
        new (pointerAt(index)) T;
    }

    void destroy(size_type index) noexcept(std::is_nothrow_destructible_v<T>) {
        pointerAt(index)->~T();
    }

    [[nodiscard]] pointer data() noexcept {
        return reinterpret_cast<pointer>(data_);
    }

    [[nodiscard]] const_pointer data() const noexcept {
        return reinterpret_cast<const_pointer>(data_);
    }

    [[nodiscard]] iterator begin() noexcept {
        return data();
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return data();
    }


    [[nodiscard]] const_iterator cbegin() const noexcept {
        return data();
    }

    [[nodiscard]] const_iterator end() noexcept {
        return data() + size;
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return data() + size;
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return data() + size;
    }

	Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

private:
    alignas(alignment) std::byte data_[total_size];
};
}  // namespace nxt::core