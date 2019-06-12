#pragma once

#include <memory>

namespace nxt::core {

template<typename T, typename Op, typename Allocator = std::allocator<T>>
class SegmentTree {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;

    SegmentTree(std::initializer_list<value_type> values) 
        : data_()
        , input_size_(0) {
        buildTree(values.begin(), values.end());
    }

    size_type inputSize() const noexcept {
        return input_size_;
    }

    size_type size() const noexcept {
        return input_size_ > 0 ? 2 * input_size_ - 1 : 0;
    }

    const_reference result() const noexcept {
        return data_[0];
    }

    void update(size_type index, const value_type& new_value) {
        updateValue(0, 0, input_size_ - 1, index, new_value);
    }

    value_type query(size_type low, size_type high) {
        return queryValue(0, 0, input_size_ - 1, low, high);
    }

    ~SegmentTree() {
        for (size_type i = 0; i < size(); ++i) {
            allocator_traits::destroy(alloc_, data_ + i);
        }

        allocator_traits::deallocate(alloc_, data_, size());
    }

private:
    template<typename RandomAccessIter>
    void buildTree(RandomAccessIter first, RandomAccessIter last) {
        size_type input_size = std::distance(first, last);
        size_type size = 2 * input_size - 1;
        auto buffer = allocator_traits::allocate(alloc_, size);
        buildInterval(0, 0, input_size - 1, buffer, first);

        input_size_ = input_size;
        data_ = buffer;
    }

    template<typename RandomAccessIter>
    void buildInterval(size_type tree_index, size_type low, size_type high, pointer buffer, RandomAccessIter value) {
        if (low == high) {
            allocator_traits::construct(alloc_, buffer + tree_index, value[low]);
        } else {
            size_type mid = (low + high) / 2;
            buildInterval(2 * tree_index + 1, low, mid, buffer, value);
            buildInterval(2 * tree_index + 2, mid + 1, high, buffer, value);

            allocator_traits::construct(alloc_, buffer + tree_index, op_(buffer[2 * tree_index + 1], buffer[2 * tree_index + 2]));
        }
    }

    void updateValue(size_type tree_index, size_type low, size_type high, size_type index, const value_type& value) {
        if (low == high) {
            data_[tree_index] = value;
        } else {
            size_type mid = (low + high) / 2;
            if (index <= mid) {
                updateValue(2 * tree_index + 1, low, mid, index, value);
            } else {
                updateValue(2 * tree_index + 2, mid + 1, high, index, value);
            }
            data_[tree_index] = op_(data_[2 * tree_index + 1], data_[2 * tree_index + 2]);
        }
    }

    value_type queryValue(size_type tree_index,
        size_type low,
        size_type high,
        size_type range_low,
        size_type range_high) {
        if (low >= range_low && high <= range_high) {
            return data_[tree_index];
        } else {
            auto mid = (low + high) / 2;
            if (low == range_low && mid == range_high)
                return data_[2 * tree_index + 1];
            else if (mid + 1 == range_low && high == range_high)
                return data_[2 * tree_index + 1];
            else {
                auto value_low = queryValue(2 * tree_index + 1, low, mid, range_low, range_high);
                auto value_high = queryValue(2 * tree_index + 1, mid + 1, low, range_low, range_high);
                return op_(value_low, value_high);
            }
        }
    }

    size_type input_size_;
    pointer data_;
    Op op_;
    allocator_type alloc_;
};
}  // namespace nxt::core
