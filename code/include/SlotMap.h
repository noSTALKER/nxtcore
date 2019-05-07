#pragma once

#include "Key.h"
#include "PageVector.h"
#include "Vector.h"

#include <iterator>
#include <type_traits>

namespace nxt::core {

template<typename SlotMap>
class SlotMapConstIterator {
public:
    // for Iterator Traits
    using difference_type = typename SlotMap::difference_type;
    using value_type = typename SlotMap::value_type;
    using size_type = typename SlotMap::size_type;
    using pointer = typename SlotMap::pointer;
    using reference = typename SlotMap::const_reference;
    using const_reference = typename SlotMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;

    SlotMapConstIterator(SlotMap* slot_map, size_type current_index) noexcept
        : slot_map_(slot_map)
        , current_index_(current_index) {}

    const_reference operator*() const {
        return slot_map_->valueAt(current_index_);
    }

    reference operator*() {
        return slot_map_->valueAt(current_index_);
    }

    SlotMapConstIterator& operator++() noexcept {
        size_t max_index = slot_map_->maxValidIndex();
        ++current_index_;
        while (current_index_ < max_index) {
            if (slot_map_->valid_list_[current_index_])
                break;
            ++current_index_;
        }
        return *this;
    }

    SlotMapConstIterator operator++(int) noexcept {
        SlotMapContIterator temp(slot_map_, current_index_);
        this->operator++();
        return temp;
    }

    SlotMapConstIterator& operator--() noexcept {
        size_t min_index = slot_map_->minValidIndex();
        --current_index_;
        while (current_index_ > min_index) {
            if (slot_map_->valid_list_[current_index_])
                break;
            --current_index_;
        }
        return *this;
    }

    SlotMapConstIterator operator--(int) {
        SlotMapConstIterator temp(slot_map_, current_index_);
        this->operator--();
        return temp;
    }

    bool operator==(const SlotMapConstIterator& rhs) const noexcept {
        return slot_map_ == rhs.slot_map_ && current_index_ == rhs.current_index_;
    }

    bool operator!=(const SlotMapConstIterator& rhs) const noexcept {
        return slot_map_ != rhs.slot_map_ || current_index_ != rhs.current_index_;
    }

protected:
    SlotMap* slot_map_;
    size_t current_index_;
};

template<typename SlotMap>
class SlotMapIterator : public SlotMapConstIterator<SlotMap> {
public:
    // for Iterator Traits
    using difference_type = typename SlotMap::difference_type;
    using value_type = typename SlotMap::value_type;
    using size_type = typename SlotMap::size_type;
    using pointer = typename SlotMap::pointer;
    using reference = typename SlotMap::const_reference;
    using const_reference = typename SlotMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;

    SlotMapIterator(SlotMap* slot_map, size_type current_index)
        : SlotMapConstIterator(slot_map, current_index) {}

    const_reference operator*() const {
        return slot_map_->valueAt(current_index_);
    }

    reference operator*() {
        return slot_map_->valueAt(current_index_);
    }

    SlotMapIterator& operator++() {
        auto max_index = slot_map_->maxValidIndex();
        ++current_index_;
        while (current_index_ < max_index) {
            if (slot_map_->valid_list_[current_index_])
                break;
            ++current_index_;
        }
        return *this;
    }

    SlotMapIterator operator++(int) noexcept {
        SlotMapIterator temp(slot_map_, current_index_);
        this->operator++();
        return temp;
    }

    SlotMapIterator& operator--() noexcept {
        size_t min_index = slot_map_->minValidIndex();
        --current_index_;
        while (current_index_ > min_index) {
            if (slot_map_->valid_list_[current_index_])
                break;
            --current_index_;
        }
        return *this;
    }

    SlotMapIterator operator--(int) noexcept {
        SlotMapIterator temp(slot_map_, current_index_);
        this->operator--();
        return temp;
    }
};

template<typename T, typename KeyType = Key, std::size_t BlockSize = 1024, typename Allocator = std::allocator<T>>
class SlotMap {
public:
    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using key_type = KeyType;
    using size_type = typename KeyType::index_type;
    using generation_type = typename KeyType::generation_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using difference_type = typename std::allocator_traits<Allocator>::difference_type;
    using iterator = typename SlotMapIterator<SlotMap<T, Key, BlockSize, Allocator>>;
    using const_iterator = typename SlotMapConstIterator<SlotMap<T, Key, BlockSize, Allocator>>;
    using key_type_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<key_type>;
    static constexpr auto block_size = BlockSize;

private:
    using Page = Buffer<value_type, block_size>;
    using page_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<Page>;
    using page_pointer_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<Page*>;
    using page_allocator_traits = std::allocator_traits<page_allocator>;
    using bool_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<bool>;

public:
    SlotMap(size_type capacity = block_size) noexcept(
        std::is_nothrow_default_constructible_v<page_allocator>&&
            std::is_nothrow_default_constructible_v<PageVector<key_type, block_size, key_type_allocator>>&&
                std::is_nothrow_default_constructible_v<PageVector<bool, block_size, bool_allocator>>)
        : size_(0)
        , free_index_(0)
        , max_valid_index_(0)
        , min_valid_index_(0)
        , alloc_() {
        reserve(capacity);
    }

    SlotMap(const SlotMap& rhs) 
		: size_(0)
		, free_index_(0)
		, max_valid_index_(0)
		, min_valid_index_(0)
		, alloc_() {

	}

    SlotMap(SlotMap&& rhs)
        : size_(0)
        , free_index_(0)
        , max_valid_index_(0)
        , min_valid_index_(0)
        , alloc_(std::move(rhs.alloc_)) {
        using std::swap;
        swap(size_, rhs.size_);
        swap(free_index_, rhs.free_index_);
        swap(max_valid_index_, rhs.max_valid_index_);
        swap(min_valid_index_, rhs.min_valid_index_);
        swap(pages_, rhs.pages_);
        swap(next_list_, rhs.next_list_);
        swap(valid_list_, rhs.valid_list_);
    }

    key_type insert(const T& value) {
        return emplace(value);
    }

    key_type insert(T&& value) {
        return emplace(std::move(value));
    }

    template<class... Args>
    key_type emplace(Args&&... args) {
        if (size_ == capacity() - 1) {
            reserve(capacity() + block_size);
        }

        page_allocator_traits::construct(alloc_, pointerAt(free_index_), std::forward<Args>(args)...);

        Key key;
        auto& old_key = next_list_[free_index_];
        // set the key index to next free index
        key.index = free_index_;
        // set the key generation from the next array
        key.generation = old_key.generation;

        // mark the item valid
        valid_list_[free_index_] = true;

        // since we are storing the next valid
        free_index_ = old_key.index;
        // change the current next values to match the key values
        old_key.index = key.index;

        // update the value of min_valid_index and max_valid_index
        if (key.index < min_valid_index_) {
            min_valid_index_ = key.index;
        }

        if (max_valid_index_ <= key.index) {
            max_valid_index_ = key.index + 1;
        }

        // increase the size
        ++size_;
        return key;
    }

    bool exist(const key_type& key) const noexcept {
        return key == next_list_[key.index];
    }

    bool erase(const key_type& key) {
        if (exist(key)) {
            size_type index = key.index;
            page_allocator_traits::destroy(alloc_, pointerAt(index));

            auto& next_data = next_list_[index];
            // set the previous free index as the index
            next_data.index = free_index_;

            // mark the item invalid
            valid_list_[index] = false;

            // increase the generation count for next allocation
            ++next_data.generation;

            // set the free index to recently removed index
            free_index_ = index;

            // decrease the size
            --size_;

            // if the removed item was a either a min_valid_index or max_valid_index,
            // update the values
            if (index == min_valid_index_) {
                while (min_valid_index_ < max_valid_index_) {
                    if (valid_list_[min_valid_index_])
                        break;
                    ++min_valid_index_;
                }
            }

            if (index + 1 == max_valid_index_) {
                while (max_valid_index_ > min_valid_index_) {
                    if (valid_list_[max_valid_index_ - 1])
                        break;
                    --max_valid_index_;
                }
            }

            return true;
        }

        return false;
    }

    const_reference at(const key_type& key) const {
        if (exist(key)) {
            return valueAt(key.index);
        }

        throw std::out_of_range("Slotmap key is invalid");
    }

    reference at(const key_type& key) {
        if (exist(key)) {
            return valueAt(key.index);
        }

        throw std::out_of_range("Slotmap key is invalid");
    }

    reference operator[](const key_type& key) {
        return valueAt(key.index);
    }

    const_reference operator[](const key_type& key) const {
        return valueAt(key.index);
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type capacity() const noexcept {
        return pages_.size() * block_size;
    }

    void reserve(size_type new_capacity) {
        auto old_capacity = capacity();
        if (new_capacity <= old_capacity)
            return;

        // round up the capacity to next block_size multiple
        auto remainder = new_capacity % block_size;
        if (remainder != 0) {
            new_capacity += (block_size - remainder);
        }

        auto block_to_add = (new_capacity - old_capacity) / block_size;

        for (size_type i = 0; i < block_to_add; ++i) {
            auto page = page_allocator_traits::allocate(alloc_, 1);
            pages_.pushBack(page);
        }

        for (size_type i = next_list_.size(); i < new_capacity; ++i) {
            next_list_.emplaceBack(i + 1, 1);
            valid_list_.emplaceBack(false);
        }
    }

    void clear() noexcept {
        // destroy items which are valid and increment the generation
        for (size_type i = 0; i < capacity(); ++i) {
            if (valid_list_[i]) {
                page_allocator_traits::destroy(alloc_, pointerAt(i));
                ++next_list_[i].generation;
            }

            // reset the list to point to next item
            next_list_[i].index = i + 1;
            valid_list_[i] = false;
        }

        min_valid_index_ = 0;
        max_valid_index_ = 0;
        size_ = 0;
        free_index_ = 0;
    }

    iterator begin() noexcept {
        return iterator(this, min_valid_index_);
    }

    const_iterator begin() const noexcept {
        return const_iterator(this, min_valid_index_);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(this, min_valid_index_);
    }

    iterator end() noexcept {
        return iterator(this, max_valid_index_);
    }

    const_iterator end() const noexcept {
        return const_iterator(this, max_valid_index_);
    }

    const_iterator cend() const noexcept {
        return const_iterator(this, max_valid_index_);
    }

    ~SlotMap() {
        clear();

        for (auto& page : pages_) {
            page_allocator_traits::deallocate(alloc_, page, 1);
        }

        pages_.clear();
    }

private:
    size_type minValidIndex() const noexcept {
        return min_valid_index_;
    }

    size_type maxValidIndex() const noexcept {
        return max_valid_index_;
    }

    reference valueAt(size_type index) {
        return pages_[index / block_size]->operator[](index % block_size);
    }

    const_reference valueAt(size_type index) const {
        return pages_[index / block_size]->operator[](index % block_size);
    }

    pointer pointerAt(size_type index) {
        return pages_[index / block_size]->pointerAt(index % block_size);
    }

    const_pointer pointerAt(size_type index) const {
        return pages_[index / block_size]->pointerAt(index % block_size);
    }

    Vector<Page*, page_pointer_allocator> pages_;
    PageVector<key_type, block_size, key_type_allocator> next_list_;
    PageVector<bool, block_size, bool_allocator> valid_list_;
    size_type free_index_;
    size_type max_valid_index_;
    size_type min_valid_index_;
    size_type size_;
    page_allocator alloc_;

    friend class const_iterator;
    friend class iterator;
};

}  // namespace nxt::core
