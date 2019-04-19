#pragma once

#include "TypeTraits.h"

namespace nxt::core {

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

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
void
insertionSort(BiDirectionalIter first, BiDirectionalIter last) {
    insertionSort(first, last, std::less<>());
}

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
void
selectionSort(ForwardIter first, ForwardIter last, Compare comp) {
    auto iter = first;

    while (iter != last) {
        // find the min element in the subrange [iter, last)
        auto minIter = iter;

        if (minIter != last) {
            auto nextIter = std::next(iter);
            while (nextIter != last) {
                if (comp(*nextIter, *minIter)) {
                    minIter = nextIter;
                }
                ++nextIter;
            }
        }

        // swap the min element with first element in subrange
        using std::swap;
        swap(*minIter, *iter);

        // increment iter to reduce the subrange
        ++iter;
    }
}

template<typename ForwardIter, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
void
selectionSort(ForwardIter first, ForwardIter last) {
    insertionSort(first, last, std::less<>());
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
BiDirectionalIter
partition(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    auto lower = first;
    auto higher = last;

    while (true) {
        // skip elements which return false for predicate
        while (true) {
            if (lower == higher) {
                return lower;
            }

            if (comp(*lower)) {
                break;
            }

            ++lower;
        }

        // do always decreases higher so it avoids dereferencing last and does decrement after a swap
        do {
            --higher;
            if (lower == higher) {
                return lower;
            }
        } while (comp(*higher));

        using std::swap;

        // swap the element that are out of order
        swap(*lower, *higher);

        // increment lower to next element. note higher is decreased in start of do-while loop
        ++lower;
    }

    return lower;
}

template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare>
constexpr OutputIter
merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter outputIter, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        // order of comparision is important for stable merge
        if (comp(*first2, *first1)) {
            *outputIter = *first2;
            ++first2;
        } else {
            *outputIter = *first1;
            ++first1;
        }
        ++outputIter;
    }

    while (first1 != last1) {
        *outputIter = *first1;
        ++first1;
        ++outputIter;
    }

    while (first2 != last2) {
        *outputIter = *first2;
        ++first2;
        ++outputIter;
    }

    return outputIter;
}
}  // namespace nxt::core