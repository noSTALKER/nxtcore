#pragma once

#include <cstddef>
#include <cstdint>

namespace nxt::core {
/**
 * @brief Key structure to be used in SlotMap
 *
 */
struct Key {

    using index_type = uint32_t;
    using generation_type = uint32_t;

    Key() = default;

    /**
     * @brief
     *
     */
    Key(index_type index, generation_type generation) noexcept
        : index(index)
        , generation(generation) {}

    /**
     * @brief
     *
     * @param rhs
     * @return true
     * @return false
     */
    constexpr bool operator==(const Key& rhs) const noexcept {
        return index == rhs.index && generation == rhs.generation;
    }

    index_type index;
    generation_type generation;
};
}  // namespace nxt::core
