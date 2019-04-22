#pragma once
#include <memory>

#include "Common.h"

namespace nxt::core {

template<typename T, std::size_t Size>
struct Buffer {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    constexpr static size_type total_size = Size * sizeof(T);
    constexpr static size_type type_size = sizeof(T);

    const_reference operator[](size_type index) const {
        return &reinterpret_cast<const_pointer>(&data[type_size * index]);
    }

    reference operator[](size_type index) {
        return *reinterpret_cast<pointer>(&data[type_size * index]);
    }

    const_pointer pointer_to(size_type index) const {
        return *reinterpret_cast<const_pointer>(&data[type_size * index]);
    }

    pointer pointer_to(size_type index) {
        return reinterpret_cast<pointer>(&data[type_size * index]);
    }

    template<typename... Args>
    void construct(size_type index, Args&&... args) {
        new (pointer_to(index)) T(std::forward<Args>(args)...);
    }

    void construct(size_type index, DefaultInitializeT) {
        new (pointer_to(index)) T;
    }

    void destroy(size_type index) {
        pointer_to(index)->~T();
    }

    std::byte data[total_size];
};
}  // namespace nxt::core