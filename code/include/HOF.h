#pragma once

namespace nxt {
namespace core {

/**
 * @brief Function which returns a invokable
 *
 * @param comp
 */
template<typename T>
constexpr auto
Equals(const T& comp) {
    return [comp](const T& value) { return comp = value; };
}

template<typename F1, typename F2>
constexpr auto
Compose(F1 f1, F2 f2) {
    return [f1, f2](const auto& value) { return f1(f2(value)); };
}

template<typename... Predicates>
constexpr auto
WhenAll(Predicates... ps) {
    return [=](const auto& value) { return (ps(value) && ...); };
}

constexpr int
increment(int x) {
    return x + 1;
}

}  // namespace core
}  // namespace ess
