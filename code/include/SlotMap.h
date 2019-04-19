#pragma once

#include "Key.h"

#include <iterator>
#include <type_traits>

namespace nxt::core {

template<class T, class KeyType = Key, std::size_t BlockSize = 1024, Allocator = std::allocator<T>>
class SlotMap {
public:
    class SlotMapIterator;
    class ConstSlotMapIterator;

    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using size_type = typename KeyType::index_type;
    using key_type = KeyType;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using difference_type = typename std::allocator_traits<Allocator>::difference_type;
    using iterator = typename SlotMap<T, KeyType, Allocator>::SlotMapIterator;
    using const_iterator = typename SlotMap<T, KeyType, Allocator>::ConstSlotMapIterator;
    using key_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<key_type>;

    static constexpr auto block_size = BlockSize;

    SlotMap(size_type capacity = block_size)
        : size_(0)
        , free_index_(0)
        , capacity_(capacity) {
        // reserve the array
        data_.Reserve(capacity_);
        next_.Reserve(capacity_);

        // fill the next array with the next available index and 1th generation
        for (SizeType i = 0; i < capacity_; i++) {
            next_.EmplaceBack(i + 1, 1, 0);
        }
    }

    key_type insert(const T& value) {
        if (size_ == Capacity() - 1) {
            Reserve(Capacity() * 2);
        }

        if (data_.Size() > free_index_) {
            data_[free_index_].~T();
            new (&data_[free_index_]) T(value);
        } else {
            data_.PushBack(value);
        }

        return construct_key();
    }

    key_type insert(T&& value) {
        if (size_ == Capacity() - 1) {
            Reserve(Capacity() * 2);
        }

        if (data_.Size() > free_index_) {
            // FIXME : currently we are just delay destructing the item
            // if it being reinserted or in destructor
            data_[free_index_].~T();
            new (&data_[free_index_]) T(std::move(value));
        } else {
            data_.PushBack(std::move(value));
        }
        return construct_key();
    }

    template<class... Args>
    key_type emplace(Args&&... args) {
        if (size_ == Capacity() - 1) {
            Reserve(Capacity() * 2);
        }

        if (data_.Size() > free_index_) {
            // FIXME : currently we are just delay destructing the item
            // if it being reinserted or in destructor
            data_[free_index_].~T();
            // use placement new operator to create a new object at the given index
            new (&data_[free_index_]) T(std::forward<Args>(args)...);
        } else {
            data_.EmplaceBack(std::forward<Args>(args)...);
        }

        return construct_key();
    }

    bool exist(const key_type& key) const {
        return key == next_[key.index];
    }

    bool erase(const key_type& key) {
        if (Exist(key)) {
            SizeType index = key.index;
            // FIXME : currently we are just delay destructing the item
            // if it being reinserted or in destructor
            // data_[index].~T();

            // set the previous free index as the index
            next_[index].index = free_index_;

            // mark the item invalid
            next_[index].valid = 0;

            // increase the generation count for next allocation
            ++next_[index].generation;

            // set the free index to recently removed index
            free_index_ = index;

            // decrease the size
            --size_;
            return true;
        }

        return false;
    }

    const_reference at(const key_type& key) const {
        if (Exist(key)) {
            return data_[key.index];
        }

        throw std::out_of_range("Slotmap key is invalid");
    }

    reference at(const key_type& key) {
        if (Exist(key)) {
            return data_[key.index];
        }

        throw std::out_of_range("Slotmap key is invalid");
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void reserve(size_type capacity) {
        capacity_ = capacity;
        data_.Reserve(capacity_);
        next_.Reserve(capacity_);

        for (SizeType i = next_.Size(); i < capacity_; ++i) {
            next_.EmplaceBack(i + 1, 1, 0);
        }
    }

    iterator begin() noexcept {
        return SlotMapIterator(this, 0);
    }

    const_iterator begin() const noexcept {
        return ConstSlotMapIterator(this, 0);
    }

    const_iterator cbegin() const noexcept {
        return ConstSlotMapIterator(this, 0);
    }

    iterator end() noexcept {
        return SlotMapIterator(this, MaxValidSize());
    }

    const_iterator end() const noexcept {
        return ConstSlotMapIterator(this, MaxValidSize());
    }

    const_iterator cend() const noexcept {
        return ConstSlotMapIterator(this, MaxValidSize());
    }

    class SlotMapIterator {
    public:
        // for Iterator Traits
        using difference_type = typename SlotMap<T, KeyType, Allocator>::difference_type;
        using value_type = typename SlotMap<T, KeyType, Allocator>::value_type;
        using size_type = typename SlotMap<T, KeyType, Allocator>::size_type;
        using pointer = typename SlotMap<T, KeyType, Allocator>::pointer;
        using reference = typename SlotMap<T, KeyType, Allocator>::reference;
        using const_reference = typename SlotMap<T, KeyType, Allocator>::const_reference;

        using iterator_category = std::bidirectional_iterator_tag;

        SlotMapIterator(SlotMap* slot_map, size_type current_index) {
            slot_map_ = slot_map;
            current_index_ = current_index;
        }

        reference operator*() {
            return slot_map_->data_[slot_map_->next_[current_index_].index];
        }

        const_reference operator*() const {
            return slot_map_->data_[slot_map_->next_[current_index_].index];
        }

        SlotMapIterator& operator++() {
            size_t max_index = slot_map_->MaxValidSize();
            ++current_index_;
            while (current_index_ < max_index) {
                if (slot_map_->next_[current_index_].valid)
                    break;
                ++current_index_;
            }
            return *this;
        }

        bool operator==(const SlotMapIterator& rhs) const {
            return slot_map_ == rhs.slot_map_ && current_index_ == rhs.current_index_;
        }

        bool operator!=(const SlotMapIterator& rhs) const {
            return !(*this == rhs);
        }

    private:
        SlotMap* slot_map_;
        size_t current_index_;
    };

private:
    Key construct_key() noexcept {
        Key key{0, 0, 0};
        // set the key index to next free index
        key.index = free_index_;
        // set the key generation from the next array
        key.generation = next_[free_index_].generation;
        // set the key valid
        key.valid = 1;

        // since we are storing the next valid
        free_index_ = next_[free_index_].index;
        // change the current next values to match the key values
        next_[key.index].index = key.index;
        next_[key.index].valid = 1;

        // increase the size
        ++size_;
        return key;
    }

    SizeType MaxValidSize() const noexcept {
        return static_cast<SizeType>(data_.Size());
    }

    Vector<Block, BlockAllocator> blocks_;
    size_type size_;
    size_type free_index_;

    friend class SlotMapIterator;
    friend class ConstSlotMapIterator;
};

}  // namespace nxt::core
