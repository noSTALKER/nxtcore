#pragma once

#include <type_traits>

namespace nxt::core {

template<template<typename...> typename U, typename T>
struct IsTemplateInstanceOf : std::false_type {};

template<template<typename...> typename U, typename... Args>
struct IsTemplateInstanceOf<U, U<Args...>> : std::true_type {};

template<typename T, typename = void>
struct IsIterator : std::false_type {};

template<typename T>
struct IsIterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>> : std::true_type {};

template<typename T>
constexpr auto IsIteratorV = IsIterator<T>::value;

template<typename T, typename = void>
struct IsOutputIterator : std::false_type {};

template<typename T>
struct IsOutputIterator<
    T,
    std::enable_if_t<std::is_base_of_v<std::output_iterator_tag, typename std::iterator_traits<T>::iterator_category>>>
    : std::true_type {};

template<typename T>
constexpr auto IsOutputIteratorV = IsOutputIterator<T>::value;

template<typename T, typename = void>
struct IsInputIterator : std::false_type {};

template<typename T>
struct IsInputIterator<
    T,
    std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>>>
    : std::true_type {};

template<typename T>
constexpr auto IsInputIteratorV = IsInputIterator<T>::value;

template<typename T, typename = void>
struct IsForwardIterator : std::false_type {};

template<typename T>
struct IsForwardIterator<
    T,
    std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<T>::iterator_category>>>
    : std::true_type {};

template<typename T>
constexpr auto IsForwardIteratorV = IsForwardIterator<T>::value;

template<typename T, typename = void>
struct IsBidirectionalIterator : std::false_type {};

template<typename T>
struct IsBidirectionalIterator<T,
                               std::enable_if_t<std::is_base_of_v<std::bidirectional_iterator_tag,
                                                                  typename std::iterator_traits<T>::iterator_category>>>
    : std::true_type {};

template<typename T>
constexpr auto IsBidirectionalIteratorV = IsBidirectionalIterator<T>::value;

template<typename T, typename = void>
struct IsRandomAccessIterator : std::false_type {};

template<typename T>
struct IsRandomAccessIterator<T,
                              std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag,
                                                                 typename std::iterator_traits<T>::iterator_category>>>
    : std::true_type {};

template<typename T>
constexpr auto IsRandomAccessIteratorV = IsRandomAccessIterator<T>::value;

template<typename T>
using IteratorValueTypeT = typename std::iterator_traits<T>::value_type;

template<typename T>
using IteratorDifferenceTypeT = typename std::iterator_traits<T>::difference_type;

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

template<template<typename...> typename Op, typename... Args>
constexpr auto IsDetectedV = IsDetected<Op, Args...>::value;

template<typename Default, template<typename...> typename Op, typename... Args>
using DetectedOrT = typename DetectedOr<Default, Op, Args...>::type;

template<typename Expected, template<typename...> typename Op, typename... Args>
using IsDetectedExact = std::is_same<Expected, DetectedT<Op, Args...>>;

template<typename Expected, template<typename...> typename Op, typename... Args>
constexpr auto IsDetectedExactV = IsDetectedExact<Expected, Op, Args...>::value;

template<typename U>
struct FirstTemplateParameter;

template<template<typename...> typename U, typename First, typename... Args>
struct FirstTemplateParameter<U<First, Args...>> {
    using type = First;
};

template<typename U>
using FirstTemplateParameterT = typename FirstTemplateParameter<U>::type;

template<typename U, typename Replace>
struct ReplaceFirstTemplateParameter;

template<template<typename...> typename U, typename Replace, typename First, typename... Args>
struct ReplaceFirstTemplateParameter<U<First, Args...>, Replace> {
    using type = U<Replace, Args...>;
};

template<typename U, typename Replace>
using ReplaceFirstTemplateParameterT = typename ReplaceFirstTemplateParameter<U, Replace>::type;

template<typename T>
using RemoveCVRefT = typename std::remove_cv_t<std::remove_reference_t<T>>;

template<typename Allocator>
constexpr auto ChoosePOCCA = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value &&
                             !std::allocator_traits<Allocator>::is_always_equal::value;

template<typename Allocator>
constexpr auto ChoosePOCMA = std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value &&
                             !std::allocator_traits<Allocator>::is_always_equal::value;

}  // namespace nxt::core
