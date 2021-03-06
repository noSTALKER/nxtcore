#pragma once

#include <algorithm>
#include <tuple>
#include "../TypeTraits.h"
#include "Search.h"

namespace nxt::core {

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr void
insertionSort(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    using std::swap;
    auto start = first;
    while (start != last) {
        // select the last element in subrange [first, start]
        // every element in subrange [first, start) is sorted
        for (auto iter = start; iter != first; --iter) {
            auto previous = iter;
            --previous;
            // move the element to right if the predicate is true
            // else we found the position and move to the next item
            // to be inserted in the sorted subrange
            if (comp(*iter, *previous)) {
                swap(*iter, *previous);
            } else {
                break;
            }
        }
        // grow the subrange [first, start]
        ++start;
    }
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr void
insertionSort(BiDirectionalIter first, BiDirectionalIter last) {
    insertionSort(first, last, std::less<>());
}

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr void
selectionSort(ForwardIter first, ForwardIter last, Compare comp) {
    auto iter = first;

    while (iter != last) {
        // find the min element in the subrange [iter, last)
        auto min_iter = iter;

        if (min_iter != last) {
            auto next_iter = std::next(iter);
            while (next_iter != last) {
                if (comp(*next_iter, *min_iter)) {
                    min_iter = next_iter;
                }
                ++next_iter;
            }
        }

        // swap the min element with first element in subrange
        using std::swap;
        swap(*min_iter, *iter);

        // increment iter to reduce the subrange
        ++iter;
    }
}

template<typename ForwardIter, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
constexpr void
selectionSort(ForwardIter first, ForwardIter last) {
    insertionSort(first, last, std::less<>());
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr BiDirectionalIter
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
merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter output_iter, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        // order of comparison is important for stable merge
        if (comp(*first2, *first1)) {
            *output_iter = *first2;
            ++first2;
        } else {
            *output_iter = *first1;
            ++first1;
        }
        ++output_iter;
    }

    // copy the remaining items if left in first iterator to the output
    while (first1 != last1) {
        *output_iter = *first1;
        ++first1;
        ++output_iter;
    }

    // copy the remaining items if left in second iterator to the output
    while (first2 != last2) {
        *output_iter = *first2;
        ++first2;
        ++output_iter;
    }

    return output_iter;
}

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
[[nodiscard]] constexpr ForwardIter
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
[[nodiscard]] constexpr ForwardIter
isSortedUntil(ForwardIter first, ForwardIter last) {
    return isSortedUntil(first, last, std::less<>());
}

template<typename ForwardIter, typename Compare, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
[[nodiscard]] constexpr bool
isSorted(ForwardIter first, ForwardIter last, Compare comp) {
    return isSortedUntil(first, last, comp) == last;
}

template<typename ForwardIter, typename = std::enable_if_t<IsForwardIteratorV<ForwardIter>>>
[[nodiscard]] constexpr bool
isSorted(ForwardIter first, ForwardIter last) {
    return isSortedUntil(first, last, std::less<>()) == last;
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
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
constexpr void
pushHeap(RandomAccessIter first, RandomAccessIter last) {
    pushHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
popHeap(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    if (last - first >= 2) {
        auto result = last;
        --result;

        // move the last value to a temporary
        IteratorValueTypeT<RandomAccessIter> value = std::move(*result);

        // pop the max value to the last place
        *result = std::move(*first);

        IteratorDifferenceTypeT<RandomAccessIter> hole_index = 0;
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
constexpr void
popHeap(RandomAccessIter first, RandomAccessIter last) {
    popHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
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
constexpr void
makeHeap(RandomAccessIter first, RandomAccessIter last) {
    makeHeap(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
[[nodiscard]] constexpr RandomAccessIter
isHeapUntil(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    auto max_distance = last - first;
    // check if each element is
    for (decltype(max_distance) i = 1; i < max_distance; ++i) {
        auto parent = (i - 1) / 2;
        if (comp(first[parent], first[i])) {
            return (first + i);
        }
    }

    return last;
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
[[nodiscard]] constexpr RandomAccessIter
isHeapUntil(RandomAccessIter first, RandomAccessIter last) {
    isHeapUntil(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
[[nodiscard]] constexpr bool
isHeap(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    return isHeapUntil(first, last, comp) == last;
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
[[nodiscard]] constexpr bool
isHeap(RandomAccessIter first, RandomAccessIter last) {
    return isHeapUntil(first, last, std::less<>()) == last;
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
heapSort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    makeHeap(first, last, comp);
    auto max_distance = last - first;
    while (max_distance > 0) {
        popHeap(first, first + max_distance, comp);
        --max_distance;
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
heapSort(RandomAccessIter first, RandomAccessIter last) {
    heapSort(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
partialSort(RandomAccessIter first, RandomAccessIter mid, RandomAccessIter last, Compare comp) {
    makeHeap(first, mid, comp);

    auto iter = mid;
    while (iter != last) {
        if (comp(*iter, *first)) {
            IteratorValueTypeT<RandomAccessIter> value = std::move(*iter);
            *iter = std::move(*first);

            auto hole_index = 0;
            auto child_index = 2 * hole_index + 1;
            auto max_distance = mid - first;
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
        ++iter;
    }

    auto mid_distance = mid - first;
    while (mid_distance > 0) {
        popHeap(first, first + mid_distance, comp);
        --mid_distance;
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
partialSort(RandomAccessIter first, RandomAccessIter mid, RandomAccessIter last) {
    partialSort(first, mid, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
quickSort(RandomAccessIter first, RandomAccessIter last, Compare comp) {
    constexpr IteratorDifferenceTypeT<RandomAccessIter> kInsertionSortLimit = 16;
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
void constexpr quickSort(RandomAccessIter first, RandomAccessIter last) {
    quickSort(first, last, std::less<>());
}

template<typename RandomAccessIter,
         typename Compare,
         typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
nthElement(RandomAccessIter first, RandomAccessIter last, RandomAccessIter nth, Compare comp) {
    constexpr IteratorDifferenceTypeT<RandomAccessIter> kInsertionSortLimit = 16;
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

    // check nth position fall in which partition
    if (low > nth) {
        nthElement(first, low, nth, comp);
    } else if (nth > low) {
        nthElement(low + 1, last, nth, comp);
    } else {
        return;
    }
}

template<typename RandomAccessIter, typename = std::enable_if_t<IsRandomAccessIteratorV<RandomAccessIter>>>
constexpr void
nthElement(RandomAccessIter first, RandomAccessIter last, RandomAccessIter nth) {
    nthElement(first, last, nth, std::less<>());
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr void
reverse(BiDirectionalIter first, BiDirectionalIter last) {
    // this condition takes care that first never crosses last for all
    // even-odd length
    while (first != last && first != --last) {
        using std::swap;
        swap(*first, *last);
        ++first;
    }
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
[[nodiscard]] constexpr BiDirectionalIter
palindromeUntil(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    auto last_saved = last;
    while (first != last && first != --last) {
        if (!comp(*first, *last))
            return first;
        ++first;
    }

    return last_saved;
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
[[nodiscard]] constexpr BiDirectionalIter
palindromeUntil(BiDirectionalIter first, BiDirectionalIter last) {
    return palindromeUntil(first, last, std::equal_to<>());
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
[[nodiscard]] constexpr bool
isPalindrome(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    while (first != last && first != --last) {
        if (!comp(*first, *last))
            return false;
        ++first;
    }

    return true;
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
[[nodiscard]] constexpr bool
isPalindrome(BiDirectionalIter first, BiDirectionalIter last) {
    return isPalindrome(first, last, std::equal_to<>());
}

template<typename InputIter1,
         typename InputIter2,
         typename Compare,
         typename = std::enable_if_t<IsInputIteratorV<InputIter1> && IsInputIteratorV<InputIter2>>>
[[nodiscard]] constexpr bool
isEqual(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare comp) {
    if constexpr (IsRandomAccessIteratorV<InputIter1> && IsRandomAccessIteratorV<InputIter2>) {
        if (last1 - first1 != last2 - first2) {
            return false;
        }

        while (first1 != last1) {
            if (!comp(*first1, *first2))
                return false;
            ++first1;
            ++first2;
        }

        return true;
    } else {
        while (first1 != last1 && first2 != last2) {
            if (!comp(*first1, *first2))
                return false;
            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 == last2;
    }
}

template<typename InputIter1,
         typename InputIter2,
         typename = std::enable_if_t<IsInputIteratorV<InputIter1> && IsInputIteratorV<InputIter2>>>
[[nodiscard]] constexpr bool
isEqual(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2) {
    return isEqual(first1, last1, first2, last2, std::equal_to<>());
}

template<typename InputIter1,
         typename InputIter2,
         typename Compare,
         typename = std::enable_if_t<IsInputIteratorV<InputIter1> && IsInputIteratorV<InputIter2>>>
[[nodiscard]] constexpr bool
lexicographicalCompare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            return true;
        } else if (comp(*first2, *first1)) {
            return false;
        }
        ++first1;
        ++first2;
    }

    return first1 == last1 && first2 != last2;
}

template<typename InputIter1,
         typename InputIter2,
         typename = std::enable_if_t<IsInputIteratorV<InputIter1> && IsInputIteratorV<InputIter2>>>
[[nodiscard]] constexpr bool
lexicographicalCompare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2) {
    return lexicographicalCompare(first1, last1, first2, last2, std::less<>());
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr BiDirectionalIter
shiftLeft(BiDirectionalIter first, BiDirectionalIter last, IteratorDifferenceTypeT<BiDirectionalIter> shift_value) {
    if (shift_value <= 0)
        return last;

    auto start_position = first;

    if constexpr (IsRandomAccessIteratorV<BiDirectionalIter>) {
        if (shift_value >= last - first)
            return first;
        start_position += shift_value;
    } else {
        for (auto i = 0; i < shift_value; ++i) {
            if (start_position == last) {
                return first;
            }
            ++start_position;
        }
    }

    auto destination = first;
    while (start_position != last) {
        *destination = std::move(*start_position);
        ++start_position;
        ++destination;
    }

    return first;
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr BiDirectionalIter
shiftRight(BiDirectionalIter first, BiDirectionalIter last, IteratorDifferenceTypeT<BiDirectionalIter> shift_value) {
    if (shift_value <= 0)
        return last;

    auto start_position = last;
    if constexpr (IsRandomAccessIteratorV<BiDirectionalIter>) {
        if (shift_value >= last - first)
            return first;
        start_position -= shift_value;
    } else {
        for (auto i = 0; i < shift_value; ++i) {
            if (first == start_position) {
                return first;
            }
            --start_position;
        }
    }

    auto source = start_position;
    auto destination = last;
    while (source != first) {
        --destination;
        --source;
        *destination = std::move(*source);
    }

    return first;
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr void
rotateLeft(BiDirectionalIter first, BiDirectionalIter mid, BiDirectionalIter last) {
    if (first == mid || mid == last)
        return;

    reverse(first, mid);
    reverse(mid, last);
    reverse(first, last);
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr bool
nextPermutation(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    auto iter = last;
    if (first == iter || first == --iter)
        return false;

    while (true) {
        auto next = iter;
        // find first pair such that value[n-1] < value[n]
        if (comp(*(--iter), *next)) {
            auto greater_iter = last;

            // find the first element that is greater than the value[n-1]
            while (!comp(*iter, *(--greater_iter))) {
            }

            // swap the values
            using std::swap;
            swap(*iter, *greater_iter);

            // reverse the sequence [next, last]
            core::reverse(next, last);
            return true;
        }

        // if we reached the first element, the reached the last permutation which is the reversed sorted
        // sequence, so we just reverse the whole sequence and return false
        if (iter == first) {
            core::reverse(first, last);
            return false;
        }
    }
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr bool
nextPermutation(BiDirectionalIter first, BiDirectionalIter last) {
    return nextPermutation(first, last, std::less<>());
}

template<typename BiDirectionalIter,
         typename Compare,
         typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr bool
previousPermutation(BiDirectionalIter first, BiDirectionalIter last, Compare comp) {
    auto iter = last;
    if (first == iter || first == --iter)
        return false;

    while (true) {
        auto next = iter;
        // find first pair such that value[n-1] < value[n]
        if (comp(*next, *(--iter))) {
            auto less_iter = last;

            // find the first element that is greater than the value[n-1]
            while (!comp(*(--less_iter), (*iter))) {
            }

            // swap the values
            {
                using std::swap;
                swap(*iter, *less_iter);
            }

            // reverse the sequence [next, last]
            core::reverse(next, last);
            return true;
        }

        // if we reached the first element, the reached the last permutation which is the reversed sorted
        // sequence, so we just reverse the whole sequence and return false
        if (iter == first) {
            core::reverse(first, last);
            return false;
        }
    }
}

template<typename BiDirectionalIter, typename = std::enable_if_t<IsBidirectionalIteratorV<BiDirectionalIter>>>
constexpr bool
previousPermutation(BiDirectionalIter first, BiDirectionalIter last) {
    return previousPermutation(first, last, std::less<>());
}

template<typename ForwardIter1,
         typename ForwardIter2,
         typename Compare,
         typename = std::enable_if_t<IsForwardIteratorV<ForwardIter1> && IsForwardIteratorV<ForwardIter2>>>
constexpr bool
isPermutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2, Compare compare) {
    if constexpr (IsRandomAccessIteratorV<ForwardIter1> && IsBidirectionalIteratorV<ForwardIter2>) {
        if (last1 - first1 != last2 - first2) {
            return false;
        }
    }

    // remove the matching suffixes
    while (first1 != last1 || first2 != last2) {
        // if found a
        if (!compare(*first1, *first2)) {
            if constexpr (IsBidirectionalIteratorV<ForwardIter1> && IsBidirectionalIteratorV<ForwardIter2>) {
                while (compare(*--last1, *--last2)) {
                }
                ++last1;
                ++last2;
            }

            auto iter1 = first1;
            while (iter1 != last1) {
                // see if we already encountered such element
                if (core::find(first1, iter1, *iter1) == iter1) {
                    // if not count the elements in the second sequence
                    auto count2 = core::count(first2, last2, *iter1);

                    // if count is zero for second sequence then return false
                    if (count2 == 0) {
                        return false;
                    }

                    // count the number of elements in first sequence and compare it
                    // with number of elemnts in second sequence
                    auto count1 = core::count(iter1, last1, *iter1);
                    if (count1 != count2)
                        return false;
                }
                ++iter1;
            }
            return true;
        }
        ++first1;
        ++first2;
    }

    return first1 == last1 && first2 == last2;
}

template<typename ForwardIter1,
         typename ForwardIter2,
         typename = std::enable_if_t<IsForwardIteratorV<ForwardIter1> && IsForwardIteratorV<ForwardIter2>>>
constexpr bool
isPermutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2) {
    return isPermutation(first1, last1, first2, last2, std::equal_to<>());
}

template<typename Container,
         typename Iterator = decltype(std::begin(std::declval<Container>())),
         typename = decltype(std::end(std::declval<Container>()))>
[[nodiscard]] constexpr auto
enumerate(Container&& container) {
    struct IteratorImpl {
        using size_type = typename std::iterator_traits<Iterator>::size_type;
		using iterator_category = std::forward_iterator_tag;

        IteratorImpl& operator++() {
            ++iterator;
            ++index;
            return *this;
        }

        [[nodiscard]] bool operator==(const IteratorImpl& rhs) const {
            return iterator == rhs.iterator;
        }

        [[nodiscard]] bool operator!=(const IteratorImpl& rhs) const {
            return iterator != rhs.iterator;
        }

        [[nodiscard]] auto operator*() const {
            return std::tie(index, *iterator);
        }

        [[nodiscard]] auto operator*() {
            return std::tie(index, *iterator);
        }

        Iterator iterator;
        size_type index;
    };

    struct ContainerWrapper {
        [[nodiscard]] auto begin() const {
            return IteratorImpl{0, std::begin(container)};
        }

        [[nodiscard]] auto end() const {
            return IteratorImpl{std::size, std::end(container)};
        }

        Container container;
    };

    return ContainerWrapper{std::forward<Container>(container)};
}

}  // namespace nxt::core
