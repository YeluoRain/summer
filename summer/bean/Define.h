//
// Created by Zhongwen Wang on 2023/12/13.
//

#ifndef SUMMER_BEAN_DEFINE
#define SUMMER_BEAN_DEFINE

#include <memory>

#include "boost/describe.hpp"
#include "boost/mp11.hpp"
#include "summer/bean/Traits.h"
#include "summer/dag/GraphOperation.h"

#define INJECT_UNPACK(...) __VA_ARGS__

#define INJECT_CONSTRUCTOR(_class, _params)                  \
    static _class* _InjectedFactory(INJECT_UNPACK _params) { \
        return nullptr;                                      \
    }                                                        \
    using FactortMethodType = decltype(_InjectedFactory);    \
    _class(INJECT_UNPACK _params)

#define INJECT_EXPLICIT_CONSTRUCTOR(_class, _params)         \
    static _class* _InjectedFactory(INJECT_UNPACK _params) { \
        return nullptr;                                      \
    }                                                        \
    using FactortMethodType = decltype(_InjectedFactory);    \
    explicit _class(INJECT_UNPACK _params)

namespace summer::bean::define {
using namespace boost;

namespace detail {
template <typename T>
struct BeanResolverHelper {
    using FuncTraits = traits::ConstructorTraits<T>;

    constexpr static auto ImplementOf = [] {
        using RetType = typename decltype(hana::typeid_(FuncTraits::RET_TYPE))::type;
        using RawRetType = typename traits::ArgTypeTraits<RetType>::type;
        return dag::operation::GetDerivedBase<RawRetType>::Bases;
    }();
    constexpr static auto DependOn = [] {
        auto argTypes = FuncTraits::ARG_TYPES;
        return hana::transform(argTypes, [](auto&& argType) {
            using ArgType = typename decltype(hana::typeid_(argType))::type;
            return hana::type_c<typename traits::ArgTypeTraits<ArgType>::type>;
        });
    }();

    constexpr static auto Args = FuncTraits::ARG_TYPES;
};

template <typename T, typename Enable = void>
struct BeanResolverImpl {
    static_assert(false,
                  "Target class not annotationed with boost.descibe,"
                  " did you use BOOST_DESCRIBE_BASE?");
};
template <typename T>
struct BeanResolverImpl<T, std::enable_if_t<describe::has_describe_bases<T>::value>>
    : BeanResolverHelper<typename T::FactortMethodType> {};

template <typename T>
struct BeanResolverImpl<T, std::enable_if_t<std::is_function_v<T>>> : BeanResolverHelper<T> {};
}  // namespace detail

template <typename T>
struct BeanResolver : detail::BeanResolverImpl<T> {
    using BeanType = T;
    constexpr static auto ImplementOf = detail::BeanResolverImpl<T>::ImplementOf;
    constexpr static auto DependOn = detail::BeanResolverImpl<T>::DependOn;
    constexpr static auto Args = detail::BeanResolverImpl<T>::Args;
};
}  // namespace summer::bean::define

#endif /* SUMMER_BEAN_DEFINE */
