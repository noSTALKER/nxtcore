#pragma once

#include <type_traits>

namespace nxt {
namespace core {

template<typename First, typename Second, bool = std::is_empty_v<First> && !std::is_final_v<First>>
class CompressedPair final : private First {
public:
    const First& getFirst() const noexcept {
        return *this;
    }

    First& getFirst() noexcept {
        return *this;
    }

    const Second& getSecond() const noexcept {
        return second_;
    }

	Second& getSecond() noexcept {
        return second_;
	}

private:
    Second second_;
};

template<typename First, typename Second>
class CompressedPair<First, Second, false> {

};

}  // namespace core
}  // namespace nxt