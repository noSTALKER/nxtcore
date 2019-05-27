#pragma once
#include <iterator>
#include <vector>
#include "TypeTraits.h"

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

    explicit Vector(const allocator_type& alloc) noexcept
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_(alloc) {}

    explicit Vector(size_type count, const allocator_type& alloc = allocator_type())
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_(alloc) {
        constructCountValues(count);
    }

    Vector(size_type count, const T& value, const allocator_type& alloc = allocator_type())
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_(alloc) {
        constructCountValues(count, value);
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    Vector(InputIter first, InputIter last)
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_() {
        if constexpr (IsForwardIteratorV<InputIter>) {
            // if it is forward iterator, we can calculate the distance and make copies
            auto count = std::distance(first, last);
            if (count > 0) {
                // allocate the buffer
                auto buffer = allocator_traits::allocate(alloc_, count);

                // construct new items by invoking copy contructor one by one
                for (decltype(count) i = 0; i < count; ++i) {
                    allocator_traits::construct(alloc_, buffer + i, *first);
                    ++first;
                }

                size_ = count;
                capacity_ = count;
                data_ = buffer;
            }
        } else {
            // else for input iterator type, just emplace back one by one
            while (first != last) {
                emplaceBack(*first);
                ++first;
            }
        }
    }

    Vector(std::initializer_list<value_type> values)
        : Vector(values.begin(), values.end()) {}

    Vector(const Vector& rhs)
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_(allocator_traits::select_on_container_copy_construction(rhs.alloc_)) {
        auto size = rhs.size();

        if (size > 0) {
            auto buffer = allocator_traits::allocate(alloc_, size);

            for (size_type i = 0; i < size; ++i) {
                allocator_traits::construct(alloc_, buffer + i, *(rhs.data_ + i));
            }

            size_ = size;
            capacity_ = size;
            data_ = buffer;
        }
    }

    Vector(Vector&& rhs) noexcept
        : size_(0)
        , capacity_(0)
        , data_(nullptr)
        , alloc_(std::move(rhs.alloc_)) {
        takeData(std::move(rhs));
    }

    Vector& operator=(const Vector& rhs) {
        if (this != std::addressof(rhs)) {
            if constexpr (ChoosePOCCA<allocator_type>) {
                if (alloc_ != rhs.alloc_) {
                    // if allocator needs to be propogated, we need to cleanup the
                    // allocation before making a copy of the allocator
                    cleanup();
                    alloc_ = rhs.alloc_;
                }
            }
        }

        assign(rhs.data_, rhs.data_ + rhs.size_);
        return *this;
    }

    Vector& operator=(Vector&& rhs) noexcept(allocator_traits::propagate_on_container_move_assignment::value ||
                                             allocator_traits::is_always_equal::value) {
        if (this != std::addressof(rhs)) {
            if constexpr (ChoosePOCMA<allocator_type>) {
                cleanup();
                alloc_ = std::move(rhs.alloc_);
            } else {
                if (alloc_ != rhs.alloc_) {
                    auto new_size = rhs.size_;
                    if (new_size > size_) {
                        if (new_size > capacity_) {
                            cleanup();
                            growBuffer(new_size, true);
                        }

                        size_type current_index = 0;
                        while (current_index < size_) {
                            data_ + current_index = std::move(*(rhs.data_ + current_index));
                            ++current_index;
                        }

                        while (current_index < new_size) {
                            allocator_traits::construct(
                                alloc_, data_ + current_index, std::move(*(rhs.data_ + current_index)));
                            ++current_index;
                        }
                    } else {
                        size_type current_index = 0;
                        while (current_index < new_size) {
                            data_ + current_index = std::move(*(rhs.data_ + current_index));
                            ++current_index;
                        }

                        while (current_index < size_) {
                            allocator_traits::destroy(alloc_, data_ + current_index);
                        }
                    }
                    size_ = new_size;
                    return;
                } else {
                    cleanup();
                }
            }

            takeData(std::move(rhs));
        }
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    void assign(InputIter first, InputIter last) {
        if constexpr (IsForwardIteratorV<InputIter>) {
            auto item_count = std::distance(first, last);
            if (item_count > size_) {
                if (item_count > capacity_) {
                    cleanup();
                    growBuffer(item_count, true);
                }

                size_type current_index = 0;
                while (current_index < size_) {
                    data_[current_index] = *first;

                    ++current_index;
                    ++first;
                }

                while (current_index < item_count) {
                    allocator_traits::construct(alloc_, data_ + current_index, *first);
                    ++current_index;
                    ++first;
                }
                size_ = item_count;

            } else {
                size_type current_index = 0;
                while (current_index < item_count) {
                    *(data_ + current_index) = *first;

                    ++current_index;
                    ++first;
                }

                while (current_index < size_) {
                    allocator_traits::destroy(alloc_, data_ + current_index);
                    ++current_index;
                }
                size_ = item_count;
            }
        } else {
        }
    }

    void assign(size_type new_size, const T& value) {
        if (new_size > size_) {
            if (new_size > capacity_) {
                cleanup();
                growBuffer(new_size, true);
            }

            size_type current_index = 0;
            while (current_index < size_) {
                *(data_ + current_index) = value;

                ++current_index;
                ++first;
            }

            for (current_index < item_count) {
                allocator_traits::construct(alloc_, data_ + current_index, value);
                ++current_index;
            }
            size_ = new_size;

        } else {
            size_type current_index = 0;
            while (current_index < new_size) {
                *(data_ + current_index) = *first;

                ++current_index;
                ++first;
            }

            while (current_index < size_) {
                allocator_traits::destroy(alloc_, data_ + current_index);
                ++current_index;
            }
            size_ = new_size;
        }
    }

    

    void assign(std::initializer_list<T> values) {
        assign(values.begin(), values.end());
    }

    void resize(size_type new_size) {
        resizeInternal(new_size);
    }

    void resize(size_type new_size, const T& value) {
        resizeInternal(new_size, value);
    }

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

    [[nodiscard]] const_reference back() const noexcept {
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
        emplaceBack(value);
    }

    void pushBack(T&& value) {
        emplaceBack(std::move(value));
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        growIfNeeded();
        allocator_traits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
    }

    void popBack() {
        if (size_ > 0) {
            allocator_traits::destroy(alloc_, (data_ + size_ - 1));
            --size_;
        }
    }

    iterator erase(const_iterator position) {
        return erase(position, position + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first == last)
            return const_cast<iterator>(first);

        iterator src = const_cast<iterator>(last);
        iterator dest = const_cast<iterator>(first);
        iterator end = data_ + size_;

        iterator result = dest;

        while (src != end) {
            *dest = *src;
            ++dest;
            ++src;
        }

        while (dest != end) {
            allocator_traits::destroy(alloc_, dest);
            ++dest;
            --size_;
        }

        return result;
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            allocator_traits::destroy(alloc_, (data_ + i));
        }
        size_ = 0;
    }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            growBuffer(new_capacity, true);
        }
    }

    void shrinkToFit() {
        if (capacity_ > size_) {
            growBuffer(size_, true);
        }
    }

    iterator insert(const_iterator position, const T& value) {
        return insertElement(position, value);
    }

    iterator insert(const_iterator position, T&& value) {
        return insertElement(position, std::move(value));
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        return insertElement(position, std::forward<Args>(args)...);
    }

    [[nodiscard]] pointer data() noexcept {
        return data_;
    }

    [[nodiscard]] const_pointer data() const noexcept {
        return data_;
    }

    ~Vector() {
        cleanup();
    }

private:
    void growIfNeeded() {
        if (size_ == capacity_) {
            growBuffer(capacity_ + 1, false);
        }
    }

    void cleanup() noexcept {
        clear();
        if (capacity_ > 0) {
            alloc_.deallocate(data_, capacity_);
        }
        capacity_ = 0;
    }

    template<typename... Args>
    void constructCountValues(size_type count, Args&&... args) {
        auto buffer = allocator_traits::allocate(alloc_, count);
        for (size_type i = 0; i < count; ++i) {
            allocator_traits::construct(alloc_, buffer + i, std::forward<Args>(args)...);
        }
        size_ = count;
        capacity_ = count;
        data_ = buffer;
    }

    template<typename... Args>
    void resizeInternal(size_type new_size, Args&&... args) {
        if (new_size > size_) {
            if (new_size > capacity_) {
                growBuffer(new_size, true);
            }

            size_type current_index = size_;
            while (current_index < new_size) {
                allocator_traits::construct(alloc_, data_ + current_index, std::forward<Args>(args)...);
                ++current_index;
            }
            size_ = new_size;

        } else {
            size_type current_index = new_size;
            while (current_index < size_) {
                allocator_traits::destroy(alloc_, data_ + current_index);
                ++current_index;
            }
            size_ = new_size;
        }
    }

    template<typename... Args>
    iterator insertElement(const_iterator position, Args&&... args) {
        if (size_ == capacity_) {
            constexpr size_type kMinSize = 8;
            auto actual_capacity = capacity_ * 2;
            actual_capacity = std::max(actual_capacity, kMinSize);

            auto new_buffer = allocator_traits::allocate(alloc_, actual_capacity);

            auto src = data_;
            auto end = data_ + size_;
            auto dest = new_buffer;

            while (src != position) {
                if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
                    allocator_traits::construct(alloc_, dest, std::move(*src));
                } else {
                    allocator_traits::construct(alloc_, dest, *src);
                }

                ++src;
                ++dest;
            }

            allocator_traits::construct(alloc_, dest, std::forward<Args>(args)...);
            auto result = dest;
            ++dest;

            while (src != end) {
                if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
                    allocator_traits::construct(alloc_, dest, std::move(*src));
                } else {
                    allocator_traits::construct(alloc_, dest, *src);
                }

                ++src;
                ++dest;
            }

            ++size_;
            data_ = new_buffer;
            capacity_ = actual_capacity;
            return result;
        } else {
            if (position == end()) {
                emplaceBack(std::forward<Args>(args)...);
                return data_ + size_ - 1;
            }

            // move the elements backwards one position
            auto dest = data_ + size_;

            auto last = dest;
            iterator first = const_cast<iterator>(position);

            // move the last element to unallocated space
            allocator_traits::construct(alloc_, last, std::move(*(last - 1)));
            --last;

            // move all the elements one position
            while (first != last) {
                --last;
                --dest;
                *dest = *last;
            }

            allocator_traits::construct(alloc_, first, std::forward<Args>(args)...);
            ++size_;
            return first;
        }
    }

    void growBuffer(size_type new_capacity, bool exact) {
        constexpr size_type kMinSize = 8;
        auto actual_capacity = new_capacity;
        if (!exact) {
            actual_capacity = std::max(new_capacity, capacity_ * 2);
            actual_capacity = std::max(actual_capacity, kMinSize);
        }

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

    void takeData(Vector&& rhs) {
        data_ = rhs.data_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;

        rhs.data_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
    }

    size_type size_;
    size_type capacity_;
    pointer data_;
    allocator_type alloc_;
};  // namespace nxt::core
}  // namespace nxt::core
