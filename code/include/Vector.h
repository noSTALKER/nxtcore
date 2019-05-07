#pragma once
#include <iterator>
#include <type_traits>
#include <vector>

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

    explicit Vector(size_type count, const allocator_type& alloc = Allocator_type())
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
        , alloc_(alloc) {
        if constexpr (IsForwardIteratorV<InputIter>) {
            while (first != last) {
                emplaceBack(*first);
                ++first;
            }
        } else {
            auto count = std::distance(last, first);
            if (count > 0) {
                auto buffer = allocator_traits::allocate(alloc_, count);

                for (decltype(count) i = 0; i < count; ++i) {
                    allocator_traits::construct(alloc_, buffer + i, *first);
                    ++first;
                }

                size_ = count;
                capacity_ = count;
                data_ = buffer;
            }
        }
    }

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
        using std::swap;
        swap(data_, rhs.data_);
        swap(size_, rhs.size_);
        swap(capacity_, rhs.capacity_);
    }

	Vector& operator=(const Vector& rhs) {
	}

	Vector& operator=(Vector&& rhs) {

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

    void popBack() {
        if (size_ > 0) {
            allocator_traits::destroy(alloc_, (data_ + size_ - 1));
            --size_;
        }
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            allocator_traits::destroy(alloc_, (data_ + i));
        }
        size_ = 0;
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

    ~Vector() {
        clear();

		if (capacity > 0) {
            alloc_.deallocate(data_, capacity_);
        }
        
    }

private:
    void growIfNeeded() {
        if (size_ == capacity_) {
            growBuffer(capacity_ * 2);
        }
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
