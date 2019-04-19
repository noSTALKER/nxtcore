#pragma once

namespace nxt::core {

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

template<typename... Predicates>
constexpr auto
WhenAll(Predicates... ps) {
    return [=](const auto& value) { return (ps(value) && ...); };
}

template<typename... Predicates>
constexpr auto
WhenAny(Predicates... ps) {
    return [=](const auto& value) { return (ps(value) || ...); };
}

template<typename... Predicates>
constexpr auto
WhenNone(Predicates... ps) {
    return [=](const auto& value) { return !(ps(value) && ...); };
}

template<typename F, typename... Funcs>
constexpr auto
Compose(F f, Funcs... funcs) {
    return [=](const auto& value) { return f(Compose(funcs...)(value)); };
}

template<typename F>
constexpr auto
Compose(F f) {
    return [=](const auto& value) { return f(value); };
}

}  // namespace nxt::core
