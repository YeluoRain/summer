//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef COLLECTION_H
#define COLLECTION_H

#include "boost/hana.hpp"
#include "boost/mp11.hpp"

namespace summer::util::collection::tuple {
    using namespace boost;

    struct MergeTupleFuncType {
        template<typename Tuple0>
        static constexpr auto Merge(const Tuple0 &tuple0) { return tuple0; }

        template<typename Tuple0, typename Tuple1, typename... Tuplen>
        static constexpr auto Merge(const Tuple0 &tuple0, const Tuple1 &tuple1, const Tuplen &... tuplen) {
            auto tuple = hana::concat(tuple0, tuple1);
            return Merge(tuple, tuplen...);
        }
    };

    constexpr auto Merge = [](const auto &... xs) { return MergeTupleFuncType::Merge(xs...); };

    struct RemoveDuplicatesFuncType {
        template<typename Tuple>
        constexpr auto operator()(Tuple &&tuple) const {
            using Type = typename decltype(hana::typeid_(tuple))::type;
            return mp11::mp_unique<Type>{};
        }
    };

    constexpr RemoveDuplicatesFuncType RemoveDuplicates{};
}// namespace collection::tuple

#endif //COLLECTION_H
