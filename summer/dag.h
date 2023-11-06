#ifndef A67ECB16_1415_4C05_B637_32E9D042A1E9
#define A67ECB16_1415_4C05_B637_32E9D042A1E9

#include "boost/hana.hpp"
#include "util.h"

namespace summer {
    namespace dag {
        namespace graph {
            using namespace boost;
            template<typename From_, typename To_>
            struct Edge {
                using From = From_;
                using To = To_;
            };

            template<typename T>
            struct Vertex {
                using NodeType = T;
                using BeanType = typename NodeType::BeanType;
                constexpr static auto InList = NodeType::ImplementOf;
                constexpr static auto OutList = NodeType::DependOn;
                constexpr static auto InDegree = hana::size(InList);
                constexpr static auto OutDegree = hana::size(OutList);
            };
        } // namespace graph

        namespace util {
            using namespace boost;

            struct GetIndependentBeans {
                template<typename BeanVertexs>
                constexpr auto operator()(BeanVertexs &&vertexs) const {
                    return hana::filter(vertexs, [](const auto &vertex) {
                        using VertexType = typename decltype(hana::typeid_(vertex))::type;
                        return VertexType::OutDegree == hana::size_c<0>;
                    });
                }
            };

            constexpr GetIndependentBeans get_independent_beans{};

            struct FillImplementedMap {
                template<typename ImplementedMap, typename IndependentBeans>
                constexpr auto operator()(ImplementedMap &&map,
                                          IndependentBeans &&beans) const {
                    auto processBean = [](auto &&bean) {
                        using VertexType = typename decltype(hana::typeid_(bean))::type;
                        auto buildPair = [](auto &&inType) {
                            return hana::make_pair(inType,
                                                   hana::type_c<typename VertexType::BeanType>);
                        };
                        return hana::unpack(VertexType::InList,
                                            hana::make_tuple ^ hana::on ^ buildPair);
                    };

                    auto mergeTuple = [](const auto &...xs) {
                        return collection::tuple::MergeTupe::Merge(xs...);
                    };
                    auto intf2implTuple =
                            hana::unpack(beans, mergeTuple ^ hana::on ^ processBean);

                    auto intfKeys =
                            hana::unpack(intf2implTuple, hana::make_tuple ^ hana::on ^ hana::first);
                    auto intfs = collection::tuple::remove_duplicates(intfKeys);
                    auto mergeMap = [&map, &intf2implTuple](auto &&intf) {
                        auto implPairs = hana::filter(intf2implTuple, [&intf](const auto &i) {
                            return hana::first(i) == intf;
                        });
                        auto impls = hana::transform(implPairs,
                                                     [](const auto &p) { return hana::second(p); });
                        return hana::if_(
                                hana::find(map, intf) == hana::nothing, hana::make_pair(intf, impls),
                                hana::make_pair(
                                        intf, hana::concat(impls, hana::to_tuple(hana::find(map, intf)))));
                    };

                    return hana::unpack(intfs, hana::make_map ^ hana::on ^ mergeMap);

                }
            };

            constexpr FillImplementedMap fillImplementedMap{};
        }
    } // namespace dag

} // namespace summer


#endif /* A67ECB16_1415_4C05_B637_32E9D042A1E9 */
