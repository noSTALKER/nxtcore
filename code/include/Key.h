#pragma once

#include <cstddef>
#include <cstdint>
#include "Export.h"

namespace ess {
namespace data {
/**
 * @brief Key structure to be used in SlotMap
 *
 */
struct Key {
    static constexpr Key kInvalidKey;

	using index_type = uint32_t;
    using generation_type = uint32_t;

    Key() = default;

    /**
     * @brief
     *
     */
    Key(index_type index, generation_type generation)
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
}  // namespace data
}  // namespace ess
