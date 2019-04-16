#pragma once

#include "TypeTraits.h"

namespace nxt {
namespace core {

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
void
insertionSort(BiDirectionalIter first, BiDirectionalIter last) {
    using std::swap;
    auto start = first;
    while (start != last) {
        for (auto iter = start; iter != first; --iter) {
            auto previous = iter;
            --previous;
            if (*iter < *previous) {
                swap(*iter, *previous);
            } else {
                break;
            }
        }
        ++start;
    }
}
}  // namespace core
}  // namespace nxt