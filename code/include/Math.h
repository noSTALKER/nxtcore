#pragma once

#include "TypeTraits.h"

namespace nxt::core {

template<typename T, typename = std::enable_if_t<std::is_unsigned_v<RemoveCVRefT<T>>>>
constexpr bool
isPowerOf2(T value) {
    return (value > 0u) && ((value & (value - 1u)) == 0u);
}

}  // namespace nxt::core