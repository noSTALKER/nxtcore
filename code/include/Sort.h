#pragma once

#include "TypeTraits.h"

namespace nxt {
namespace core {

template<typename ForwardIter, typename = std::enable_if_t<IsRandomAccessIteratorV<ForwardIter>>>
void
insertionSort(ForwardIter first, ForwardIter last) {
    using std::swap;
    if (first != last) {
        auto index = first;
        

    }
}
}  // namespace core
}  // namespace nxt