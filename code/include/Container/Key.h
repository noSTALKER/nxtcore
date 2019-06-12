#pragma once

#include <cstddef>
#include <cstdint>

namespace nxt::core {
/**
 * @brief Key structure to be used in SlotMap with 32 bit generation and 32 bit index 
 *
 */
struct Key {

    using index_type = uint32_t;
    using generation_type = uint32_t;

    Key() = default;

    /**
     * @brief Constructor
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

    constexpr bool operator!=(const Key& rhs) const noexcept{
        return index != rhs.index || generation != rhs.generation;
        }

    index_type index;
    generation_type generation;
};
}  // namespace nxt::core
