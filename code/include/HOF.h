#pragma once

#include <functional>

namespace nxt::core {

/**
 * @brief Function which returns a invokable
 *
 * @param comp
 */
template<typename T>
[[nodiscard]] constexpr auto
equalTo(const T& comp) {
    return [comp](const T& value) { return comp == value; };
}

template<typename T>
[[nodiscard]] constexpr auto
notEqualTo(const T& comp) {
    return [comp](const T& value) { return comp != value; };
}

template<typename T>
[[nodiscard]] constexpr auto
lessThan(const T& comp) {
    return [comp](const T& value) { return value < comp; };
}

template<typename T>
[[nodiscard]] constexpr auto
greaterThan(const T& comp) {
    return [comp](const T& value) { return value > comp; };
}

template<typename... Predicates>
[[nodiscard]] constexpr auto
whenAll(Predicates... ps) {
    return [=](const auto& value) { return (ps(value) && ...); };
}

template<typename... Predicates>
[[nodiscard]] constexpr auto
whenAny(Predicates... ps) {
    return [=](const auto& value) { return (ps(value) || ...); };
}

template<typename... Predicates>
[[nodiscard]] constexpr auto
whenNone(Predicates... ps) {
    return [=](const auto& value) { return !(ps(value) && ...); };
}

template<typename F, typename... Funcs>
[[nodiscard]] constexpr auto
compose(F f, Funcs... funcs) {
    return [=](const auto& value) { return f(Compose(funcs...)(value)); };
}

template<typename F>
[[nodiscard]] constexpr auto
compose(F f) {
    return [=](const auto& value) { return f(value); };
}

}  // namespace nxt::core
