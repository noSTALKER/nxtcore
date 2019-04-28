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

    PageVectorConstIterator(PageVector* vector, size_type index)
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

    PageVectorIterator(PageVector* vector, size_type index)
        : PageVectorConstIterator(vector, index) {}

    PageVectorIterator& operator++() noexcept {
        ++current_index_;
        return *this;
    }

    PageVectorIterator operator++(int) noexcept {
        PageVectorIterator temp(page_vector_, current_index_);
        ++current_index_;
        return temp;
    }

    PageVectorIterator& operator--() noexcept {
        --current_index_;
        return *this;
    }

    PageVectorIterator operator--(int) noexcept {
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
};

template<typename T, std::size_t PageSize, typename Allocator = std::allocator<T>>
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
        : alloc_()
        , size_(0){}

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
        return pages_[index / page_size]->operator[](index % page_size);
    }

    [[nodiscard]] const_reference operator[](size_type index) const noexcept {
        return pages_[index / page_size]->operator[](index % page_size);
    }

    [[nodiscard]] reference front() { return pages_.front()->operator[](0); }

    [[nodiscard]] const_reference front() const {
        return pages_.front()->operator[](0);
    }

    [[nodiscard]] reference back() { return pages_.back()->operator[]((size_ - 1) % page_size); }

    [[nodiscard]] const_reference back() const {
        return pages_.back()->operator[]((size_ - 1) % page_size);
    }

    void pushBack(const T& value) {
        growIfNeeded();

        page_allocator_traits::construct(alloc_, pages_[size_ / page_size]->pointer_to(size_ % page_size), value);
        ++size_;
    }

    void pushBack(T&& value) {
        growIfNeeded();
        page_allocator_traits::construct(
            alloc_, pages_[size_ / page_size]->pointer_to(size_ % page_size), std::move(value));
        ++size_;
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        growIfNeeded();
        page_allocator_traits::construct(
            alloc_, pages_[size_ / page_size]->pointer_to(size_ % page_size), std::forward<Args>(args)...);
        ++size_;
    }

    void popBack() {
        if (size_ > 0) {
            page_allocator_traits::destroy(alloc_, pages_[size_ / page_size]->pointer_to(size_ % page_size));
            --size_;
        }
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            page_allocator_traits::destroy(alloc_, pages_[size_ / page_size]->pointer_to(size_ % page_size));
        }

        size_ = 0;
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
        clear();

        for (auto& page : pages_) {
            alloc_.deallocate(page, 1);
        }

        pages_.clear();
    }

private:
    void growIfNeeded() {
        if (size_ == capacity()) {
            auto new_page = page_allocator_traits::allocate(alloc_, 1);
            pages_.pushBack(new_page);
        }
    }

    Vector<Page*, page_pointer_allocator> pages_;
    size_type size_;
    page_allocator alloc_;
};

}  // namespace nxt::core