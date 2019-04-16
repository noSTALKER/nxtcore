#pragma once

#include "TypeTraits.h"

namespace nxt {
namespace core {

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
void
insertionSort(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    using std::swap;
    auto start = first;
    while (start != last) {
        for (auto iter = start; iter != first; --iter) {
            auto previous = iter;
            --previous;
            if (comp(*iter, *previous)) {
                swap(*iter, *previous);
            } else {
                break;
            }
        }
        ++start;
    }
}

template<typename BiDirectionalIter,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
void
insertionSort(BiDirectionalIter first, BiDirectionalIter last) {
    insertionSort(first, last, std::less<>());
}
}  // namespace core
}  // namespace nxt