#pragma once

#include <type_traits>

namespace nxt::core {

struct ValueInitThenVariadicArgsT {};
struct FirstArgThenVariadicArgsT {};

// This class uses empty-class optimization to save space when second item in the pair is a empty
// class
template<typename First, typename Second, bool = std::is_empty_v<First> && !std::is_final_v<First>>
class CompressedPair final : private First {
public:
    template<typename... Args>
    CompressedPair(ValueInitThenVariadicArgsT, Args&&... args)
        : First()
        , second_(std::forward<Args>(args)...) {}

    template<typename FirstArg, typename... Args>
    CompressedPair(FirstArgThenVariadicArgsT, First&& firstArg, Args&&... args)
        : First(std::forward<FirstArg>(firstArg))
        , second_(std::forward<Args>(args)...) {}

    const First& first() const noexcept {
        return *this;
    }

    First& first() noexcept {
        return *this;
    }

    const Second& second() const noexcept {
        return second_;
    }

    Second& second() noexcept {
        return second_;
    }

private:
    Second second_;
};

template<typename First, typename Second>
class CompressedPair<First, Second, false> {
public:
    template<typename... Args>
    CompressedPair(ValueInitThenVariadicArgsT, Args&&... args)
        : first_()
        , second_(std::forward<Args>(args)...) {}

    template<typename FirstArg, typename... Args>
    CompressedPair(FirstArgThenVariadicArgsT, First&& firstArg, Args&&... args)
        : first_(std::forward<FirstArg>(firstArg))
        , second_(std::forward<Args>(args)...) {}

    const First& first() const noexcept {
        return first_;
    }

    First& first() noexcept {
        return first_;
    }

    const Second& second() const noexcept {
        return second_;
    }

    Second& second() noexcept {
        return second_;
    }

private:
    First first_;
    Second second_;
};

}  // namespace nxt::core