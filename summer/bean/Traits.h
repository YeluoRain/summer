//
// Created by Zhongwen Wang on 2023/12/15.
//

#ifndef TRAITS_H
#define TRAITS_H

namespace summer::bean::traits {
    template<typename T>
    struct ConstructorTraits : std::false_type {};

    template<typename R, typename... Args>
    struct ConstructorTraits<R (*)(Args...)> : std::true_type {
        constexpr static std::size_t ARG_NUM = sizeof...(Args);
        constexpr static auto ARG_TYPES = boost::hana::make_tuple(boost::hana::type_c<Args>...);
    };
}

#endif //TRAITS_H
