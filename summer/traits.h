//
// Created by Zhongwen Wang on 2023/11/5.
//

#ifndef CCTEST_TRAITS_H
#define CCTEST_TRAITS_H

#include "boost/hana.hpp"

namespace summer {
    namespace traits {
        template<typename HanaType>
        struct HanaTraitsHelper {
            static constexpr auto test(HanaType hanaType) -> typename decltype(boost::hana::typeid_(hanaType))::type;
        };

        template<typename HanaType, HanaType hanaType>
        struct HanaTraits {
            using Type = decltype(HanaTraitsHelper<HanaType>::test(hanaType));
        };
    }
}

#endif //CCTEST_TRAITS_H
