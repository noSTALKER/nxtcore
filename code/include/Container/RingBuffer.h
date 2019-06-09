#pragma once

#include <iterator>

#include "../Maths.h"

namespace nxt::core {

template<typename T, typename Allocator = std::allocator<T>>
class RingBuffer {
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

	static_assert(std::is_unsigned_v<size_type>, "size_type must a unsigned integral");

    RingBuffer() noexcept(std::is_nothrow_default_constructible_v<allocator_type>)
        : data_()
        , front_(0)
        , back_(0)
        , capacity_(0)
        , alloc_() {}

    RingBuffer(size_type capacity)
        : data()
        , front_(0)
        , back_(0)
        , capacity_(0)
        , alloc_() {
        reserve(capacity);
    }

    RingBuffer(const RingBuffer& rhs)
        : data_()
        , front_(0)
        , back_(0)
        , capacity_(0)
        , alloc_(allocator_traits::select_on_container_copy_construction(rhs.alloc_)) {
        reserve(rhs.capacity());
        for (auto start = rhs.front_; start != rhs.back_; start = rhs.getNext(start)) {
            allocator_traits::construct(alloc_, data_ + start, rhs.data_[start]);
        }

        front_ = rhs.front_;
        back_ = rhs.back_;
    }

    RingBuffer(RingBuffer&& rhs)
        : data_()
        , front_(0)
        , back_(0)
        , capacity_(0)
        , alloc_(std::move(rhs.alloc_)) {
        data_ = rhs.data_;
        front_ = rhs.front_;
        back_ = rhs.back_;
        capacity_ = rhs.capacity_;

        rhs.data_ = nullptr;
        rhs.front_ = 0;
        rhs.back_ = 0;
        rhs.capacity_ = 0;
    }

    void pushBack(const T& value) {
        emplaceBack(value);
    }

    void pushBack(T&& value) {
        emplaceBack(std::move(value));
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        if (full()) {
            reserve(capacity_ + 1);
        }

        allocator_traits::construct(alloc_, data_ + back_, std::forward<Args>(args)...);
        back_ = getNext(back_);
    }

    void popFront() {
        if (!empty()) {
            allocator_traits::destroy(alloc_, data_ + front_);
            front_ = getNext(front_);
        }
    }

    [[nodiscard]] value_type popFrontAndExtract() {
        if (!empty()) {
            value_type result = std::move(data_[front_]);
            allocator_traits::destroy(alloc_, data_ + front_);
            front_ = getNext(front_);
            return result;
        }

		throw std::logic_error("trying to pop value from empty ring buffer");
    }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            if (!isPowerOf2(new_capacity) || new_capacity < 2) {
                new_capacity = getNextPowerOf2(new_capacity);
            }

            auto new_buffer = allocator_traits::allocate(alloc_, new_capacity);
            auto index = front_;
            for (size_type start = front_; start != back_; start = getNext(start)) {
                if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
                    allocator_traits::construct(alloc_, new_buffer + index, std::move(data_[start]));
                } else {
                    allocator_traits::construct(alloc_, new_buffer + start, data_[start]);
                }
                index = mask(index + 1, new_capacity);
            }

            for (size_type i = front_; i != back_; i = getNext(i)) {
                allocator_traits::destroy(alloc_, data_ + i);
            }

            back_ = index;
            data_ = new_buffer;
            capacity_ = new_capacity;
        }
    }

    [[nodiscard]] bool empty() const noexcept {
        return back_ == front_;
    }

    [[nodiscard]] bool full() const noexcept {
        if (capacity_ > 0)
            return getNext(back_) == front_;
        return true;
    }

    [[nodiscard]] reference front() {
        return data_[front_];
    }

    [[nodiscard]] const_reference front() const {
        return data_[front_];
    }

    [[nodiscard]] reference back() {
        return data_[getLast(back_)];
    }

    [[nodiscard]] const_reference back() const {
        return data_[getLast(back_)];
    }

    void clear() noexcept {
        while (back_ != front_) {
            allocator_traits::destroy(alloc_, data_ + front_);
            front_ = getNext(front_);
        }
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] size_type size() const noexcept {
        return mask(back_ - front_);
    }

    ~RingBuffer() {
        clear();

        if (capacity_ > 0) {
            allocator_traits::deallocate(alloc_, data_, capacity_);
            capacity_ = 0;
            data_ = nullptr;
        }
    }

private:
    size_type mask(size_type value) const noexcept {
        return value & (capacity_ - 1);
    }

    size_type mask(size_type value, size_type capacity) const noexcept {
        return value & (capacity - 1);
    }

    size_type getNext(size_type index) const noexcept {
        return mask(index + 1);
    }

    size_type getLast(size_type index) const noexcept {
        return mask(index - 1);
    }

    pointer data_;
    size_type front_;
    size_type back_;
    size_type capacity_;
    allocator_type alloc_;
};
}  // namespace nxt::core