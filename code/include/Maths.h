#pragma once

#include "TypeTraits.h"

namespace nxt::core {

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr bool
isPowerOf2(T value) {
    return (value > 0u) && ((value & (value - 1u)) == 0u);
}

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr auto
getNextPowerOf2(T value) {
    T result = 1;
    while (result <= value) result *= 2;
    return result;
}

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr bool
isEven(T value) {
    return ((value & 1u) == 0);
}

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr bool
isOdd(T value) {
    return ((value & 1u) != 0);
}
template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr T
rotateLeft(T value, uint32_t rotate) {
    constexpr T bit_size = sizeof(T) * 8;
    uint32_t rotate_factor = rotate % bit_size;
    return (value >> rotate_factor) | (value << (bit_size - rotate_factor));
}

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr T
rotateRight(T value, uint32_t rotate) {
    constexpr T bit_size = sizeof(T) * 8;
    uint32_t rotate_factor = rotate % bit_size;
    return (value << rotate_factor) | (value >> (bit_size - rotate_factor));
}

}  // namespace nxt::core
