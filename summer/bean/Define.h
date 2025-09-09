//
// Created by Zhongwen Wang on 2023/12/13.
//

#ifndef BEANDEFINE_H
#define BEANDEFINE_H

#include "Summer.h"
#include "bean/Traits.h"
#include "boost/describe.hpp"
#include "boost/mp11.hpp"
#include <memory>

#define INJECT_UNPACK(...) __VA_ARGS__

#define INJECT_CONSTRUCTOR(_class, _params)                                                                            \
    static void _InjectedFactory(INJECT_UNPACK _params) {}                                                             \
    _class(INJECT_UNPACK _params)

namespace summer::bean::define {
    using namespace boost;

    namespace detail {
        template<typename T, typename DerivedBases = describe::describe_bases<T, describe::mod_any_access>,
                 typename FuncTraits = traits::ConstructorTraits<decltype(&T::_InjectedFactory)>>
        struct BeanResolverImpl {
            constexpr static auto ImplementOf = [] {
                using HanaList = mp11::mp_rename<DerivedBases, hana::tuple>;
                auto hanaList = hana::to_tuple(HanaList{});
                return hana::transform(hanaList, [](auto &&describeType) {
                    using Descriptor = std::remove_reference_t<decltype(describeType)>;
                    return hana::type_c<typename Descriptor::type>;
                });
            }();
            constexpr static auto DependOn = [] {
                auto argTypes = FuncTraits::ARG_TYPES;
                return hana::transform(argTypes, [](auto &&argType) {
                    using ArgType = typename decltype(hana::typeid_(argType))::type;
                    using ArgTypePointerTraits = std::pointer_traits<std::decay_t<ArgType>>;
                    return hana::type_c<typename ArgTypePointerTraits::element_type>;
                });
            }();
        };
    }// namespace detail

    template<typename T>
    struct BeanResolver : detail::BeanResolverImpl<T> {
        static_assert(describe::has_describe_bases<T>::value, "Target class not annotationed with boost.descibe,"
                      " did you use BOOST_DESCRIBE_BASE?");
        constexpr static auto ImplementOf = detail::BeanResolverImpl<T>::ImplementOf;
        constexpr static auto DependOn = detail::BeanResolverImpl<T>::DependOn;
    };
}// namespace summer::bean::define

#endif//BEANDEFINE_H
