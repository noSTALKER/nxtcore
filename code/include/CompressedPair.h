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

    [[nodiscard]] const First& first() const noexcept {
        return *this;
    }

    [[nodiscard]] First& first() noexcept {
        return *this;
    }

    [[nodiscard]] const Second& second() const noexcept {
        return second_;
    }

    [[nodiscard]] Second& second() noexcept {
        return second_;
    }

private:
    Second second_;
};

template<typename First, typename Second>
class CompressedPair<First, Second, false> final {
public:
    template<typename... Args>
    CompressedPair(ValueInitThenVariadicArgsT, Args&&... args)
        : first_()
        , second_(std::forward<Args>(args)...) {}

    template<typename FirstArg, typename... Args>
    CompressedPair(FirstArgThenVariadicArgsT, First&& firstArg, Args&&... args)
        : first_(std::forward<FirstArg>(firstArg))
        , second_(std::forward<Args>(args)...) {}

    [[nodiscard]] const First& first() const noexcept {
        return first_;
    }

    [[nodiscard]] First& first() noexcept {
        return first_;
    }

    [[nodiscard]] const Second& second() const noexcept {
        return second_;
    }

    [[nodiscard]] Second& second() noexcept {
        return second_;
    }

private:
    First first_;
    Second second_;
};

}  // namespace nxt::core
