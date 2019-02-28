#pragma once

#include <type_traits>

namespace nxt {
namespace core {

template<template<typename...> typename U, typename T>
struct IsTemplateInstanceOf : std::false_type {};

template<template<typename...> typename U, typename... args>
struct IsTemplateInstanceOf<U, U<args...>> : std::true_type {};

template<typename T, typename = void>
struct IsIterator : std::false_type {};

template<typename T>
struct IsIterator<T, std::void_t<std::iterator_traits<T>::iterator_category>> : std::true_type {};

}  // namespace core
}  // namespace nxt
