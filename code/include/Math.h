#pragma once

#include "TypeTraits.h"

namespace nxt::core {

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr bool
isPowerOf2(T value) {
    return (value > 0u) && ((value & (value - 1u)) == 0u);
}

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
[[nodiscard]] constexpr T
getNextPowerOf2(T value) {
    T result = 1;
    while (result <= value)
		result *= 2;
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

}  // namespace nxt::core