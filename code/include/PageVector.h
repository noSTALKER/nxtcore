#pragma once

#include <memory>
#include <type_traits>

#include "Buffer.h"
#include "Vector.h"

namespace nxt::core {

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

private:
    using Page = Buffer<value_type, page_size>;

    using page_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Page>;
    using page_allocator_traits = typename std::allocator_traits<page_allocator>;
    using page_pointer_allocator = typename allocator_traits::template rebind_alloc<Page*>;

public:
    PageVector() noexcept(std::is_nothrow_default_constructible_v<page_allocator> &&
                          std::is_nothrow_constructible_v<decltype(pages_)>)
        : alloc_()
        , size_(0){}

              [[nodiscard]] size_type capacity() const noexcept {
        return pages_.size() * page_size;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] reference operator[](size_type index) noexcept {
        return pages_[index / page_size]->operator[](index % page_size);
    }

    [[nodiscard]] const_reference operator[](size_type index) const noexcept {
        return pages_[index / page_size]->operator[](index % page_size);
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    void pushBack(const T& value) {
        growIfNeeded();

        page_allocator_traits::construct(alloc_, pages[size_ / page_size]->pointer_to(size_ % page_size), value);
        size_ = new_size;
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