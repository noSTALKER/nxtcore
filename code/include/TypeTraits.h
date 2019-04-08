#pragma once

#include <type_traits>
#include <vector>
#include <atomic>

namespace nxt {
namespace core {

template<template<typename...> typename U, typename T>
struct IsTemplateInstanceOf : std::false_type {};

template<template<typename...> typename U, typename... Args>
struct IsTemplateInstanceOf<U, U<Args...>> : std::true_type {};

template<typename T, typename = void>
struct IsIterator : std::false_type {};

template<typename T>
struct IsIterator<T, std::void_t<std::iterator_traits<T>::iterator_category>> : std::true_type {};

struct NoSuchClass {
    NoSuchClass() = delete;
    ~NoSuchClass() = delete;
    NoSuchClass(const NoSuchClass&) = delete;
    void operator=(const NoSuchClass&) = delete;
};

template<typename Default, typename Void, template<typename...> typename Op, typename... Args>
struct Detection {
    using type = Default;
    using value_t = std::false_type;
};

template<template<typename...> typename Op, typename... Args>
struct Detection<NoSuchClass, std::void_t<Op<Args...>>, Op, Args...> : std::true_type {
    using type = Op<Args...>;
    using value_t = std::true_type;
};

template<template<typename...> typename Op, typename... Args>
using DetectedT = typename Detection<NoSuchClass, void, Op, Args...>::type;

template<template<typename...> typename Op, typename... Args>
using IsDetected = typename Detection<NoSuchClass, void, Op, Args...>::value_t;

template<typename Default, template<typename...> typename Op, typename... Args>
using DetectedOr = Detection<Default, void, Op, Args...>;

template<template <typename...> typename Op, typename... Args>
using IsDetectedV = IsDetected<Op, Args...>::value;

template<typename Default, template <typename...> typename Op, typename... Args>
using IsDetectedT = DetectedOr<Default, Op, Args...>::type;

template<typename Default>
using IsDetectedSame = DetectedOr<Default, std::is_same_t, Default>;

template<typename U> struct FirstTemplateParameter;

template<template<typename...> typename U, typename First, typename... Args>
struct FirstTemplateParameter<U<First, Args...>> {
    using type = typename First;
};

template<typename U>
using FirstTemplateParameterT = typename FirstTemplateParameter<U>::type;

template<typename U, typename Replace>
struct ReplaceFirstTemplateParameter;

template<template<typename...> typename U, typename Replace, typename First, typename... Args>
struct ReplaceFirstTemplateParameter<U<First, Args...>, Replace> {
    using type = typename U<Replace, Args...>;
};

template<typename U, typename Replace>
using ReplaceFirstTemplateParameterT = typename ReplaceFirstTemplateParameter<U, Replace>::type;

}  // namespace core
}  // namespace nxt
