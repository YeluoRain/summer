/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SUMMER_BEAN_TRAITS
#define SUMMER_BEAN_TRAITS

#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

#include "boost/hana.hpp"

namespace summer::bean::traits {

// 基础模板
template <typename T>
struct ArgTypeTraits {
    using type = T;
};

// 指针
template <typename T>
struct ArgTypeTraits<T*> {
    using type = typename ArgTypeTraits<T>::type;
};

// std::shared_ptr
template <typename T>
struct ArgTypeTraits<std::shared_ptr<T>> {
    using type = typename ArgTypeTraits<T>::type;
};

// const
template <typename T>
struct ArgTypeTraits<const T> {
    using type = typename ArgTypeTraits<T>::type;
};

// &
template <typename T>
struct ArgTypeTraits<T&> {
    using type = typename ArgTypeTraits<T>::type;
};

// std::unique_ptr
template <typename T>
struct ArgTypeTraits<std::unique_ptr<T>> {
    using type = typename ArgTypeTraits<T>::type;
};

// std::list
template <typename T>
struct ArgTypeTraits<std::list<T>> {
    using type = typename ArgTypeTraits<T>::type;
};

// std::vector
template <typename T>
struct ArgTypeTraits<std::vector<T>> {
    using type = typename ArgTypeTraits<T>::type;
};

template <typename T>
struct ConstructorTraits : std::false_type {};

template <typename R, typename... Args>
struct ConstructorTraits<R(Args...)> : std::true_type {
    constexpr static std::size_t ARG_NUM = sizeof...(Args);
    constexpr static auto ARG_TYPES = boost::hana::make_tuple(boost::hana::type_c<Args>...);
    constexpr static auto RET_TYPE = boost::hana::type_c<R>;
    using RetType = typename ArgTypeTraits<R>::type;
};

template <typename R, typename... Args>
struct ConstructorTraits<R (*)(Args...)> : std::true_type {
    constexpr static std::size_t ARG_NUM = sizeof...(Args);
    constexpr static auto ARG_TYPES = boost::hana::make_tuple(boost::hana::type_c<Args>...);
    constexpr static auto RET_TYPE = boost::hana::type_c<R>;
    using RetType = typename ArgTypeTraits<R>::type;
};

template <typename R, typename... Args>
struct ConstructorTraits<R (*const)(Args...)> : std::true_type {
    constexpr static std::size_t ARG_NUM = sizeof...(Args);
    constexpr static auto ARG_TYPES = boost::hana::make_tuple(boost::hana::type_c<Args>...);
    constexpr static auto RET_TYPE = boost::hana::type_c<R>;
    using RetType = typename ArgTypeTraits<R>::type;
};

template <typename R, typename... Args>
struct ConstructorTraits<std::function<R(Args...)>> : std::true_type {
    constexpr static std::size_t ARG_NUM = sizeof...(Args);
    constexpr static auto ARG_TYPES = boost::hana::make_tuple(boost::hana::type_c<Args>...);
    constexpr static auto RET_TYPE = boost::hana::type_c<R>;
    using RetType = typename ArgTypeTraits<R>::type;
};

template <auto CreatorFunc>
struct CreatorWrapper {
    constexpr static auto Creator = CreatorFunc;
};

template <typename T, typename Enable = void>
struct IsCreatorWrapper : std::false_type {};

template <typename T>
struct IsCreatorWrapper<T, std::void_t<decltype(T::Creator)>> : std::true_type {};

}  // namespace summer::bean::traits

#endif /* SUMMER_BEAN_TRAITS */
