//
// Created by Zhongwen Wang on 2023/11/5.
//

#ifndef CCTEST_UTIL_H
#define CCTEST_UTIL_H

#include "boost/core/demangle.hpp"
#include "boost/hana.hpp"
#include "boost/mp11.hpp"
#include "boost/mp11/algorithm.hpp"
#include "traits.h"

namespace summer {
    namespace print {
        using namespace boost;
        struct ToString {
            template<typename T>
            std::string operator()(const T &) const {
                return core::demangle(typeid(T).name());
            }
        };

        ToString to_string{};
    }// namespace print
    namespace collection {
        namespace map {
            //            using namespace boost;
            //
            //            template<typename Key, typename Transformer>
            //            struct replace_helper_t {
            //                const Key &key;
            //                const Transformer &transformer;
            //
            //                template<typename Pair>
            //                constexpr decltype(auto) operator()(Pair &&p) const {
            //                    using ValueType = typename decltype(hana::typeid_(hana::second(p)))::type;
            //                    return hana::if_(
            //                            hana::equal(key, hana::first(p)),
            //                            hana::make_pair(hana::first(p),
            //                                            transformer(std::forward<ValueType>(hana::second(p)))),
            //                            std::forward<Pair>(p)
            //                    );
            //                }
            //            };
            //
            //            struct replace_t {
            //                template<typename Map, typename Key, typename Transformer>
            //                constexpr auto operator()(Map &&m, Key &&key, Transformer &&transformer) const {
            //                    auto replace_helper = replace_helper_t<Key, Transformer>{
            //                            std::forward<Key>(key),
            //                            std::forward<Transformer>(transformer)};
            //                    return hana::unpack(std::forward<Map>(m),
            //                                        hana::make_map ^ hana::on ^ replace_helper);
            //                }
            //            };
            //
            //            constexpr replace_t replace{};
        }
        namespace tuple {
            using namespace boost;
            struct MergeTuple {
                template<typename Tuple0>
                static constexpr auto Merge(const Tuple0 &tuple0) {
                    return tuple0;
                }

                template<typename Tuple0, typename Tuple1, typename... Tuplen>
                static constexpr auto Merge(const Tuple0 &tuple0, const Tuple1 &tuple1, const Tuplen &...tuplen) {
                    auto tuple = hana::concat(tuple0, tuple1);
                    return Merge(tuple, tuplen...);
                }
            };

            constexpr auto merge_tuple = [](const auto &...xs) { return collection::tuple::MergeTuple::Merge(xs...); };

            struct RemoveDuplicates {
                template<typename Tuple>
                constexpr auto operator()(Tuple &&tuple) const {
                    using Type = typename decltype(hana::typeid_(tuple))::type;
                    return mp11::mp_unique<Type>{};
                }
            };

            constexpr RemoveDuplicates remove_duplicates{};
        }// namespace tuple
    }    // namespace collection
}// namespace summer

#endif//CCTEST_UTIL_H
