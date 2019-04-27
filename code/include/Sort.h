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

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
isSortedUntil(ForwardIter first, ForwardIter last, Compare comp) {
    auto next = first;
    if (next != last) {
        ++next;
        while (next != last) {
            if (comp(*next, *first)) {
                break;
            }
            ++next;
            ++first;
        }
    }

    return next;
}

template<typename ForwardIter, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr ForwardIter
isSortedUntil(ForwardIter first, ForwardIter last) {
    return isSortedUntil(first, last, std::less<>());
}

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr bool
isSorted(ForwardIter first, ForwardIter last, Compare comp) {
    return isSortedUntil(first, last, comp) == last;
}

template<typename ForwardIter, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr bool
isSorted(ForwardIter first, ForwardIter last) {
    return isSortedUntil(first, last, std::less<>()) == last;
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
pushHeap(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first >= 2) {
        auto hole = last;

        // point to the element to push
        --hole;

        auto distance = hole - first;

        // move the value to a temporary variable
        IteratorValueTypeT<RandomAccessIter> value = std::move(*hole);
        while (distance > 0) {
            // get the parent of the hole
            distance = (distance - 1) / 2;
            auto parent = first + distance;

            // move the value at parent to hole if predicate is true
            if (comp(*parent, value)) {
                *hole = std::move(*parent);

                // parent is the new hole
                hole = parent;
            } else {
                break;
            }
        }

        // move the stored value to the hole
        *hole = std::move(value);
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
pushHeap(RandomAccessIter first, RandomAccessIter last) {
    pushHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
popHeap(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first >= 2) {
        auto result = last;
        --result;

        // move the last value to a temporary
        IteratorValueTypeT<RandomAccessIter> value = std::move(*result);

        // pop the max value to the last place
        *result = std::move(*first);

        auto hole_index = 0;
        auto child_index = 2 * hole_index + 1;
        auto max_distance = result - first;
        while (child_index < max_distance) {
            // check if second child exist or not
            if (child_index + 1 < max_distance) {
                // select the greater child
                if (comp(first[child_index], first[child_index + 1])) {
                    ++child_index;
                }
            }

            if (comp(first[child_index], value)) {
                break;
            }

            first[hole_index] = std::move(first[child_index]);
            hole_index = child_index;
            child_index = 2 * hole_index + 1;
        }

        first[hole_index] = std::move(value);
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
popHeap(RandomAccessIter first, RandomAccessIter last) {
    popHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
makeHeap(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first >= 2) {
        auto max_distance = last - first;
        for (decltype(max_distance) i = max_distance / 2; i >= 0; --i) {
            // move the last value to a temporary
            auto hole_index = i;
            IteratorValueTypeT<RandomAccessIter> value = std::move(first[hole_index]);

            
            auto child_index = 2 * hole_index + 1;
            while (child_index < max_distance) {
                // check if second child exist or not
                if (child_index + 1 < max_distance) {
                    // select the greater child
                    if (comp(first[child_index], first[child_index + 1])) {
                        ++child_index;
                    }
                }

                if (comp(first[child_index], value)) {
                    break;
                }

                first[hole_index] = std::move(first[child_index]);
                hole_index = child_index;
                child_index = 2 * hole_index + 1;
            }

            first[hole_index] = std::move(value);
        }
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
makeHeap(RandomAccessIter first, RandomAccessIter last) {
    makeHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
heapSort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    makeHeap(first, last, comp);
    auto max_distance = last - first;
    while (max_distance > 0) {
        popHeap(first, first + max_distance, comp);
        --max_distance;
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
heapSort(RandomAccessIter first, RandomAccessIter last) {
    heapSort(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
quickSort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    static constexpr IteratorDifferenceTypeT<RandomAccessIter> kInsertionSortLimit = 16;
    if (kInsertionSortLimit >= last - first) {
        insertionSort(first, last, comp);
        return;
    }

    auto last_element = last;
    --last_element;

    auto distance = last - first;
    auto mid = first + (distance / 2);
    using std::swap;

    // do swaps to place the median of 3 in the middle
    if (comp(*mid, *first)) {
        swap(*mid, *first);
    }

    if (comp(*last_element, *mid)) {
        swap(*mid, *last_element);
    }

    if (comp(*mid, *first)) {
        swap(*mid, *first);
    }

    // replace the median to place it in the last place
    swap(*mid, *last_element);

    auto low = first;
    auto high = last_element;
    --high;

    while (true) {
        // swap lower and higher values when they don't satisfy the predicate
        while (comp(*low, *last_element)) {
            ++low;
        }

        while (comp(*last_element, *high)) {
            --high;
        }

        // check if lower is smaller than high, if yes, swap the values or else
        // we found the place for the pivot value
        if (low < high) {
            swap(*low, *high);
            ++low;
            --high;
        } else {
            break;
        }
    }

    // swap the pivot value to place it at its place
    swap(*low, *last_element);

    // recursively sort lower and higher partition now
    quickSort(first, low, comp);
    quickSort(low + 1, last, comp);
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
void
quickSort(RandomAccessIter first, RandomAccessIter last) {
    quickSort(first, last, std::less<>());
}

}  // namespace nxt::core