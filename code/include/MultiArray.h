#include <array>
#include <iostream>

template <typename T, size_t... Ns>
class MultiArray;

template <typename T, size_t N0, size_t... Ns>
class MultiArray<T, N0, Ns...> {
public:
    using internal_type = typename std::array<MultiArray<T, Ns...>, N0>;
    using reference = typename internal_type::reference;
    using const_reference = typename internal_type::const_reference;

    reference operator[](size_t index) {
        return array_[index];
    }

    constexpr size_t size() const {
        return array_.size();
    }

    template<typename SizeT, typename... Args>
    auto operator()(SizeT i, Args... args) {
        static_assert(std::is_convertible_v<SizeT, size_t>, "Invalid index type");
        return array_[i](args...);
    }

    template<typename SizeT>
    auto operator()(SizeT i) {
        static_assert(std::is_convertible_v<SizeT, size_t>, "Invalid index type");
        return array_[i];
    }

private:
    internal_type array_;
};

template <typename T, size_t N0>
class MultiArray<T, N0> {
public:
    using internal_type = typename std::array<T, N0>;
    using reference = typename internal_type::reference;
    reference operator[](size_t index) {
        return array_[index];
    }

    constexpr size_t size() const {
        return array_.size();
    }

    reference operator()(size_t index) {
        return array_[index];
    }

private:
    internal_type array_;
};