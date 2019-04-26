#pragma once
#include <memory>

#include "Common.h"

namespace nxt::core {

template<typename T, std::size_t Size>
struct Buffer {
    static_assert(Size > 0, "Can't create a Buffer of size == 0");

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    constexpr static size_type total_size = Size * sizeof(T);
    constexpr static size_type type_size = sizeof(T);

    [[nodiscard]] const_reference operator[](size_type index) const {
        return *reinterpret_cast<const_pointer>(&data_[type_size * index]);
    }

    [[nodiscard]] reference operator[](size_type index) {
        return *reinterpret_cast<pointer>(&data_[type_size * index]);
    }

    [[nodiscard]] const_pointer pointer_to(size_type index) const noexcept {
        return reinterpret_cast<const_pointer>(&data_[type_size * index]);
    }

    [[nodiscard]] pointer pointer_to(size_type index) noexcept {
        return reinterpret_cast<pointer>(&data_[type_size * index]);
    }

    template<typename... Args>
    void construct(size_type index, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        new (pointer_to(index)) T(std::forward<Args>(args)...);
    }

    void construct(size_type index, DefaultInitializeTag) noexcept(std::is_nothrow_default_constructible_v<T>) {
        new (pointer_to(index)) T;
    }

    void destroy(size_type index) noexcept(std::is_nothrow_destructible_v<T>) {
        pointer_to(index)->~T();
    }

    [[nodiscard]] pointer data() noexcept {
        return reinterpret_cast<pointer>(data_);
    }

    [[nodiscard]] const_pointer data() const noexcept {
        return reinterpret_cast<const_pointer>(data_);
    }

private:

    alignas(value_type) std::byte data_[total_size];
};
}  // namespace nxt::core