#pragma once

#include <memory>
#include <type_traits>

#include "Buffer.h"
#include "Vector.h"

namespace nxt::core {

template<typename PageVector>
class PageVectorConstIterator {
public:
    using value_type = typename PageVector::value_type;
    using difference_type = typename PageVector::difference_type;
    using pointer = typename PageVector::const_pointer;
    using reference = typename PageVector::const_reference;
    using const_reference = typename PageVector::const_reference;
    using size_type = typename PageVector::size_type;
    using difference_type = typename PageVector::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    PageVectorConstIterator(PageVector* vector, size_type index) noexcept
        : page_vector_(vector)
        , current_index_(index) {}

    PageVectorConstIterator& operator++() noexcept {
        ++current_index_;
        return *this;
    }

    PageVectorConstIterator operator++(int) noexcept {
        PageVectorConstIterator temp(page_vector_, current_index_);
        ++current_index_;
        return temp;
    }

    PageVectorConstIterator& operator--() noexcept {
        --current_index_;
        return *this;
    }

    PageVectorConstIterator operator--(int) noexcept {
        PageVectorConstIterator temp(page_vector_, current_index_);
        --current_index_;
        return temp;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return page_vector_->operator[](current_index_);
    }

    [[nodiscard]] reference operator[](difference_type value) const noexcept {
        return page_vector_->operator[](current_index_ + value);
    }

    [[nodiscard]] PageVectorConstIterator operator-(difference_type value) const noexcept {
        return PageVectorConstIterator(page_vector_, current_index_ - value);
    }

    PageVectorConstIterator& operator-=(difference_type value) noexcept {
        current_index_ -= value;
        return *this;
    }

    [[nodiscard]] PageVectorConstIterator operator+(difference_type value) const noexcept {
        return PageVectorConstIterator(page_vector_, current_index_ + value);
    }

    PageVectorConstIterator& operator+=(difference_type value) noexcept {
        current_index_ += value;
        return *this;
    }

    [[nodiscard]] difference_type operator-(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ - rhs.current_index_;
    }

    [[nodiscard]] bool operator<(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ < rhs.current_index_;
    }

    [[nodiscard]] bool operator<=(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ <= rhs.current_index_;
    }

    [[nodiscard]] bool operator>(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ > rhs.current_index_;
    }

    [[nodiscard]] bool operator>=(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ >= rhs.current_index_;
    }

    [[nodiscard]] bool operator==(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ == rhs.current_index_;
    }

    [[nodiscard]] bool operator!=(const PageVectorConstIterator& rhs) const noexcept {
        return current_index_ != rhs.current_index_;
    }

protected:
    PageVector* page_vector_;
    size_type current_index_;
};

template<typename PageVector>
class PageVectorIterator : public PageVectorConstIterator<PageVector> {
public:
    using value_type = typename PageVector::value_type;
    using difference_type = typename PageVector::difference_type;
    using pointer = typename PageVector::pointer;
    using reference = typename PageVector::reference;
    using const_reference = typename PageVector::const_reference;
    using size_type = typename PageVector::size_type;
    using difference_type = typename PageVector::difference_type;
    using iterator_category = std::random_access_iterator_tag;
    using base_class = PageVectorConstIterator<PageVector>;

    PageVectorIterator(PageVector* vector, size_type index) noexcept
        : base_class(vector, index) {}

    PageVectorIterator& operator++() noexcept {
        ++current_index_;
        return *this;
    }

    [[nodiscard]] PageVectorIterator operator++(int) noexcept {
        PageVectorIterator temp(page_vector_, current_index_);
        ++current_index_;
        return temp;
    }

    PageVectorIterator& operator--() noexcept {
        --current_index_;
        return *this;
    }

    [[nodiscard]] PageVectorIterator operator--(int) noexcept {
        PageVectorIterator temp(page_vector_, current_index_);
        --current_index_;
        return temp;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return page_vector_->operator[](current_index_);
    }

    [[nodiscard]] reference operator[](difference_type value) const noexcept {
        return page_vector_->operator[](current_index_ + value);
    }

    [[nodiscard]] PageVectorIterator operator-(difference_type value) const noexcept {
        return PageVectorIterator(page_vector_, current_index_ - value);
    }

    PageVectorIterator& operator-=(difference_type value) noexcept {
        current_index_ -= value;
        return *this;
    }

    [[nodiscard]] PageVectorIterator operator+(difference_type value) const noexcept {
        return PageVectorIterator(page_vector_, current_index_ + value);
    }

    PageVectorIterator& operator+=(difference_type value) noexcept {
        current_index_ += value;
        return *this;
    }

    [[nodiscard]] difference_type operator-(const PageVectorIterator& rhs) const noexcept {
        return current_index_ - rhs.current_index_;
    }

protected:
    using base_class::current_index_;
    using base_class::page_vector_;
};

template<typename T, std::size_t PageSize = 32, typename Allocator = std::allocator<T>>
class PageVector {
public:
    constexpr static auto page_size = PageSize;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using difference_type = typename allocator_traits::difference_type;
    using size_type = typename allocator_traits::size_type;
    using iterator = PageVectorIterator<PageVector<T, PageSize, Allocator>>;
    using const_iterator = PageVectorConstIterator<PageVector<T, PageSize, Allocator>>;

private:
    using Page = Buffer<value_type, page_size>;

    using page_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Page>;
    using page_allocator_traits = typename std::allocator_traits<page_allocator>;
    using page_pointer_allocator = typename allocator_traits::template rebind_alloc<Page*>;

public:
    PageVector() noexcept(
        std::is_nothrow_default_constructible_v<page_allocator>&& std::is_nothrow_constructible_v<decltype(pages_)>)
        : size_(0)
        , alloc_() {}

    PageVector(const PageVector& rhs)
        : size_(0)
        , alloc_(page_allocator_traits::select_on_container_copy_construction(rhs.alloc_)) {
        reserve(rhs.capacity());

        for (const auto& value : rhs) {
            pushBack(value);
        }
    }

    PageVector(PageVector&& rhs) noexcept(std::is_nothrow_move_constructible_v<page_allocator>)
        : size_(0)
        , alloc_(std::move(rhs.alloc_)) {
        takeData(std::move(rhs));
    }

    PageVector& operator=(const PageVector& rhs) {
        if (this != std::addressof(rhs)) {
            if (ChoosePOCCA<page_allocator>) {
                if (alloc_ != rhs.alloc_) {
                    cleanup();
                    alloc_ = rhs.alloc_;
                }
            }
        }

        assign(rhs.begin(), rhs.end());
    }

    PageVector& operator=(PageVector&& rhs) {
        if (this != std::addressof(rhs)) {
            if (ChoosePOCMA<page_allocator>) {
                cleanup();
                alloc_ = std::move(rhs.alloc_);

            } else {
                if (alloc_ != rhs.alloc_) {
                    if (size_ > rhs.size_) {
                        for (size_type i = 0; i < rhs.size_; ++i) {
                            valueAt(i) = std::move(rhs.valueAt(i));
                        }

                        for (size_type i = rhs.size_; i < size_; ++i) {
                            page_allocator_traits::destroy(alloc_, pointerAt(i));
                        }

                    } else {
                        reserve(rhs.capacity());
                        for (size_type i = 0; i < size_; ++i) {
                            valueAt(i) = std::move(rhs.valueAt(i));
                        }

                        for (size_type i = size_; i < rhs.size_; ++i) {
                            page_allocator_traits::construct(alloc_, pointerAt(i), std::move(rhs.valueAt(i)));
                        }
                    }
                    size_ = rhs.size_;
                    return;
                } else {
                    cleanup();
                }
            }
        }
        takeData(std::move(rhs));
    }

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    PageVector(InputIter first, InputIter last)
        : size_(0)
        , alloc_() {
        assign(first, last);
    }

    PageVector(std::initializer_list<T> values)
        : PageVector(values.begin(), values.end()) {}

    template<typename InputIter, typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
    void assign(InputIter first, InputIter last) {
        if constexpr (IsForwardIteratorV<InputIter>) {
            auto item_count = static_cast<size_type>(std::distance(first, last));

            if (size_ > item_count) {
                for (size_type i = 0; i < item_count; ++i) {
                    valueAt(i) = *first;
                    ++first;
                }

                for (size_type i = item_count; i < size_; ++i) {
                    page_allocator_traits::destroy(alloc_, pointerAt(i));
                }
            } else {
                reserve(item_count);
                for (size_type i = 0; i < size_; ++i) {
                    valueAt(i) = *first;
                    ++first;
                }

                for (size_type i = size_; i < item_count; ++i) {
                    page_allocator_traits::construct(alloc_, pointerAt(i), *first);
                    ++first;
                }
            }

            size_ = item_count;
        } else {
            size_t item_added = 0;
            while (size_ > item_count && first != last) {
                valueAt(i) = *first;
                ++first;
                ++item_added;
            }

            while (first != last) {
                pushBack(*first);
                ++first;
                ++item_added;
            }

            for (size_type i = item_count; i < size_; ++i) {
                page_allocator_traits::destroy(alloc_, pointerAt(i));
            }
        }
    }

    void assign(size_type count, const T& value) {
        if (size_ > count) {
            for (size_type i = 0; i < count; ++i) {
                valueAt(i) = value;
            }

            for (size_type i = count; i < size_; ++i) {
                page_allocator_traits::destroy(alloc_, pointerAt(i));
            }
        } else {
            reserve(count);
            for (size_type i = 0; i < size_; ++i) {
                valueAt(i) = value;
            }

            for (size_type i = size_; i < item_count; ++i) {
                page_allocator_traits::construct(alloc_, pointerAt(i), value);
            }
        }

        size_ = item_count;
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return pages_.size() * page_size;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] reference operator[](size_type index) noexcept {
        return valueAt(index);
    }

    [[nodiscard]] const_reference operator[](size_type index) const noexcept {
        return valueAt(index);
    }

    [[nodiscard]] reference front() noexcept {
        return pages_.front()->operator[](0);
    }

    [[nodiscard]] const_reference front() const noexcept {
        return pages_.front()->operator[](0);
    }

    [[nodiscard]] reference back() noexcept {
        return pages_.back()->operator[]((size_ - 1) % page_size);
    }

    [[nodiscard]] const_reference back() const noexcept {
        return pages_.back()->operator[]((size_ - 1) % page_size);
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
        page_allocator_traits::construct(alloc_, pointerAt(size_), std::forward<Args>(args)...);
        ++size_;
    }

    void popBack() {
        if (size_ > 0) {
            auto last_element = (size_ - 1);
            page_allocator_traits::destroy(alloc_, pointerAt(last_element));
            --size_;
        }
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            page_allocator_traits::destroy(alloc_, pointerAt(i));
        }

        size_ = 0;
    }

    void reserve(size_type new_capacity) {
        auto old_capacity = capacity();
        if (new_capacity > old_capacity) {
            auto remainder = new_capacity % page_size;
            if (remainder > 0) {
                new_capacity += (page_size - remainder);
            }

            auto pages_to_add = (new_capacity - old_capacity) / page_size;
            for (size_type i = 0; i < pages_to_add; ++i) {
                auto page = page_allocator_traits::allocate(alloc_, 1);
                pages_.pushBack(page);
            }
        }
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        return const_iterator(this, 0);
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return const_iterator(this, 0);
    }

    [[nodiscard]] iterator begin() noexcept {
        return iterator(this, 0);
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator(this, size_);
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return const_iterator(this, size_);
    }

    [[nodiscard]] iterator end() noexcept {
        return iterator(this, size_);
    }

    ~PageVector() {
        cleanup();
    }

private:
    void growIfNeeded() {
        // allocate a new page if the size is equal to the capacity
        if (size_ == capacity()) {
            auto new_page = page_allocator_traits::allocate(alloc_, 1);
            pages_.pushBack(new_page);
        }
    }

    void cleanup() noexcept {
        clear();

        for (auto page : pages_) {
            page_allocator_traits::deallocate(alloc_, page, 1);
        }

        pages_.clear();
    }

    void takeData(PageVector&& rhs) noexcept {
        pages_ = std::move(rhs.pages_);
        size_ = rhs.size_;
        rhs.size_ = 0;
    }

    reference valueAt(size_type index) noexcept {
        return pages_[index / page_size]->operator[](index % page_size);
    }

    const_reference valueAt(size_type index) const noexcept {
        return pages_[index / page_size]->operator[](index % page_size);
    }

    pointer pointerAt(size_type index) noexcept {
        return pages_[index / page_size]->pointerAt(index % page_size);
    }

    const_pointer pointerAt(size_type index) const noexcept {
        return pages_[index / page_size]->pointerAt(index % page_size);
    }

    Vector<Page*, page_pointer_allocator> pages_;
    size_type size_;
    page_allocator alloc_;
};

// template type deduction guide for constructor which accepts input iterators
template<typename InputIter,
         std::size_t PageSize = 32,
         typename Allocator = std::allocator<IteratorValueTypeT<InputIter>>,
         typename = std::enable_if_t<IsInputIteratorV<InputIter>>>
PageVector(InputIter, InputIter)->PageVector<IteratorValueTypeT<InputIter>, PageSize, Allocator>;

}  // namespace nxt::core