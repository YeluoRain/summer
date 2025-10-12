//
// Created by Zhongwen Wang on 2023/12/15.
//

#ifndef TRAITS_H
#define TRAITS_H

#include "boost/hana.hpp"
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

namespace summer::bean::traits {

// 基础模板
template <typename T> struct ArgTypeTraits {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

// 指针
template <typename T> struct ArgTypeTraits<T *> {
  using type = typename ArgTypeTraits<T>::type;
};

// std::shared_ptr
template <typename T> struct ArgTypeTraits<std::shared_ptr<T>> {
  using type = typename ArgTypeTraits<T>::type;
};

// std::unique_ptr
template <typename T> struct ArgTypeTraits<std::unique_ptr<T>> {
  using type = typename ArgTypeTraits<T>::type;
};

// std::list
template <typename T> struct ArgTypeTraits<std::list<T>> {
  using type = typename ArgTypeTraits<T>::type;
};

// std::vector
template <typename T> struct ArgTypeTraits<std::vector<T>> {
  using type = typename ArgTypeTraits<T>::type;
};

template <typename T> struct ConstructorTraits : std::false_type {};

template <typename R, typename... Args>
struct ConstructorTraits<R (*)(Args...)> : std::true_type {
  constexpr static std::size_t ARG_NUM = sizeof...(Args);
  constexpr static auto ARG_TYPES =
      boost::hana::make_tuple(boost::hana::type_c<Args>...);
};
} // namespace summer::bean::traits

#endif // TRAITS_H
