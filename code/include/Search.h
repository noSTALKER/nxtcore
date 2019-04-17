#pragma once

#include <iterator>
#include "TypeTraits.h"

namespace nxt::core {

template<typename ForwardIter,
         typename T,
         typename Compare,
         typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
lowerBound(ForwardIter first, ForwardIter last, const T& value, Compare comp) {
    auto distance = std::distance(first, last);

    while (distance > 0) {
        auto step = distance / 2;
        auto mid = std::next(first, step);
        if (comp(*mid, value)) {
            ++mid;
            first = mid;
            distance -= step + 1;
        } else {
            distance = step;
        }
    }

    return first;
}

template<typename ForwardIter, typename T, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
lowerBound(ForwardIter first, ForwardIter last, const T& value) {
    return lowerBound(first, last, value, std::less<>());
}

template<typename ForwardIter,
         typename T,
         typename Compare,
         typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
upperBound(ForwardIter first, ForwardIter last, const T& value, Compare comp) {
    auto distance = std::distance(first, last);

    while (distance > 0) {
        auto step = distance / 2;
        auto mid = std::next(first, step);
        if (comp(value, *mid)) {
            distance = step;
        } else {
            ++mid;
            first = mid;
            distance -= step + 1;
        }
    }

    return first;
}

template<typename ForwardIter, typename T, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
upperBound(ForwardIter first, ForwardIter last, const T& value) {
    return upperBound(first, last, value, std::less<>());
}

template<typename ForwardIter,
         typename T,
         typename Compare,
         typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr bool
binarySearch(ForwardIter first, ForwardIter last, const T& value, Compare comp) {
	// get the lower bound which is the first element not less than value
    auto result = lowerBound(first, last, value, comp);

	//check if the lower bound is not pointing to last value and 
    return result != last && (!comp(value, *result));
}

template<typename ForwardIter, typename T, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr bool
binarySearch(ForwardIter first, ForwardIter last, const T& value) {
    return binarySearch(first, last, value, std::less<>());
}

}  // namespace nxt::core