#pragma once
#include <iterator>
#include <type_traits>

namespace nxt::core {

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = typename std::allocator_traits<allocator_type>;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;

    Vector() noexcept(std::is_nothrow_default_constructible_v<allocator_type>)
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_() {}

    [[nodiscard]] iterator begin() noexcept {
        return data_;
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return data_;
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        return data_;
    }

    [[nodiscard]] iterator end() noexcept {
        return data_ + size_;
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return data_ + size_;
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return data_ + size_;
    }

    [[nodiscard]] reference front() noexcept {
        return *data_;
    }

    [[nodiscard]] const_reference front() const noexcept {
        return *data_;
    }

    [[nodiscard]] reference back() noexcept {
        return data_[size_ - 1];
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] reference operator[](size_type position) {
        return data_[position];
    }

    [[nodiscard]] const_reference operator[](size_type position) const {
        return data_[position];
    }

    void pushBack(const T& value) {
        growIfNeeded();
        allocator_traits::construct(alloc_, data_ + size_, value);
        ++size_;
    }

    void pushBack(T&& value) {
        growIfNeeded();
        allocator_traits::construct(alloc_, data_ + size_, std::move(value));
        ++size_;
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        growIfNeeded();
        allocator_traits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
    }

    iterator insert(const_iterator position, const T& value) {}

    iterator insert(const_iterator position, T&& value) {}

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {}

    [[nodiscard]] pointer data() noexcept {
        return data_;
    }

    [[nodiscard]] const_pointer data() const noexcept {
        return data_;
    }

private:
    void growIfNeeded() {
        if (size_ == capacity_) {
            growBuffer(capacity_ * 2);
        }
    }

    void growBuffer(size_type new_capacity) {
        constexpr size_type kMinSize = 8;
        auto buffer_size = new_capacity;
        if (new_capacity > capacity_ || new_capacity < kMinSize) {
            auto actual_capacity = std::max(new_capacity, capacity_ * 2);
            actual_capacity = std::max(actual_capacity, kMinSize);
            auto new_buffer = allocator_traits::allocate(alloc_, actual_capacity);

            if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
                for (size_type i = 0; i < size_; ++i) {
                    allocator_traits::construct(alloc_, new_buffer + i, std::move(data_[i]));
                }
            } else {
                for (size_type i = 0; i < size_; ++i) {
                    allocator_traits::construct(alloc_, new_buffer + i, data_[i]);
                }
            }

            for (size_type i = 0; i < size_; ++i) {
                allocator_traits::destroy(alloc_, data_ + i);
            }

            allocator_traits::deallocate(alloc_, data_, capacity_);
            data_ = new_buffer;
            capacity_ = actual_capacity;
        }
    }

    pointer data_;
    size_type size_;
    size_type capacity_;
    allocator_type alloc_;
};
}  // namespace nxt::core
