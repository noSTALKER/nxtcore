#pragma once

#include <iterator>

namespace nxt {
namespace core {

/**

*/
template<typename T, std::size_t Size>
class Array {
public:
    static_assert(Size > 0, "Can't create a array with size 0, it leads to undefined behaviour");

    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using const_iterator = typename ConstArrayIterator<T, Size>;

    constexpr size_type size() const noexcept {
        return Size;
    }

    constexpr size_type max_size() const noexcept {
        return Size;
    }

    constexpr reference operator[](size_type index) {
        return data_[index];
    }

    constexpr const_reference operator[](size_type index) const {
        return data_[index];
    }

    constexpr bool empty() const noexcept {
        return Size == 0;
    }

    constexpr const_reference front() const noexcept {
        return data_[0];
    }

    constexpr reference front() noexcept {
        return data_[0];
    }

    constexpr pointer data() noexcept {
        return data_;
    }

    constexpr const_pointer data() const noexcept {
        return data_;
    }

    constexpr const_reference back() const noexcept {
        return data_[Size - 1];
    }

    constexpr reference back() noexcept {
        return data_[Size - 1];
    }

    constexpr const_iterator begin() const noexcept {
        return const_iterator(data_, 0);
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator(data_, 0);
    }

    constexpr const_iterator end() const noexcept {
        return const_iterator(data_, Size);
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator(data_, Size);
    }

    T data_[N];
};

template<class T, std::size_t Size>
class ConstArrayIterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::random_access_iterator_tag;
    constexpr ConstArrayIterator()
        : ptr_(nullptr) {}

    explicit constexpr ConstArrayIterator(pointer ptr)
        : ptr_(ptr) {}

    constexpr reference operator*() const {
        return *ptr_;
    }

    constexpr pointer operator->() const {
        return ptr_;
    }

    constexpr ConstArrayIterator& operator++() noexcept {
        ++ptr_;
        return *this;
    }

    constexpr ConstArrayIterator operator++(int) noexcept {
        ConstArrayIterator temp(ptr_);
        ++ptr_;
        return temp;
    }

    constexpr ConstArrayIterator& operator--() noexcept {
        --ptr_;
        return *this;
    }

    constexpr ConstArrayIterator operator--(int) noexcept {
        ConstArrayIterator temp(ptr_);
        --ptr_;
        return temp;
    }

    constexpr ConstArrayIterator& operator+=(const difference_type offset) noexcept {
        ptr_ += offset;
        return *this;
    }

    constexpr ConstArrayIterator operator+(const difference_type offset) const noexcept {
        return temp(ptr_ + offset);
    }

    constexpr ConstArrayIterator& operator-=(const difference_type offset) noexcept {
        ptr_ -= offset;
        return *this;
    }

    constexpr ConstArrayIterator operator-(const difference_type offset) const noexcept {
        return temp(ptr_ - offset);
    }

    constexpr bool operator==(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ == rhs.ptr_;
    }

    constexpr bool operator!=(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ != rhs.ptr_;
    }

    constexpr bool operator<(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ < rhs.ptr_;
    }

    constexpr bool operator>(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ > rhs.ptr_;
    }

    constexpr bool operator<=(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ <= rhs.ptr_;
    }

    constexpr bool operator>=(const ConstArrayIterator& rhs) const noexcept {
        return ptr_ >= rhs.ptr_;
    }

    constexpr reference operator[](const difference_type offset) const {
        return *(ptr_ + offset);
    }

private:
    pointer ptr_;
};
}  // namespace core
}  // namespace nxt
