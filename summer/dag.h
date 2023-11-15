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
        }// namespace graph

        namespace util {
            using namespace boost;

            struct GetIndependentBeans {
                template<typename BeanVertexes>
                constexpr auto operator()(BeanVertexes &&vertexes) const {
                    return hana::filter(vertexes, [](const auto &vertex) {
                        using VertexType = typename decltype(hana::typeid_(vertex))::type;
                        return VertexType::OutDegree == hana::size_c<0>;
                    });
                }
            };

            constexpr GetIndependentBeans get_independent_beans{};

            struct FillImplementedMap {
                template<typename ImplementedMap, typename IndependentBeans>
                constexpr auto operator()(ImplementedMap &&map, IndependentBeans &&beans) const {
                    // 转换成Tuple(Pair(Intf, Impl)...)
                    auto processBean = [](auto &&bean) {
                        using VertexType = typename decltype(hana::typeid_(bean))::type;
                        auto buildPair = [](auto &&inType) {
                            return hana::make_pair(inType, hana::type_c<typename VertexType::BeanType>);
                        };
                        return hana::unpack(VertexType::InList, hana::make_tuple ^ hana::on ^ buildPair);
                    };
                    // 合并内层
                    // Tuple(Tuple(Pair(Intf, Impl)...), Tuple(Pair(Intf, Impl)...), ...) -> Tuple(Pair(Intf, Impl)...)
                    auto intf2implTuple = hana::unpack(beans, collection::tuple::merge_tuple ^ hana::on ^ processBean);
                    // Tuple(Intf...)
                    auto inputKeys = hana::unpack(intf2implTuple, hana::make_tuple ^ hana::on ^ hana::first);
                    auto intfKeys = hana::concat(hana::to_tuple(hana::keys(map)), inputKeys);
                    // 去除重复key
                    auto intfs = collection::tuple::remove_duplicates(intfKeys);
                    auto mergeMap = [&map, &intf2implTuple](auto &&intf) {
                        // 找出了实现了intf的Pairs
                        auto implPairs =
                                hana::filter(intf2implTuple, [&intf](const auto &i) { return hana::first(i) == intf; });
                        // 取Impls
                        auto impls = hana::transform(implPairs, [](const auto &p) { return hana::second(p); });
                        auto existedImpls = hana::find(map, intf).value_or(hana::make_tuple());
                        // std::cout << summer::print::to_string(result) << std::endl;
                        return hana::make_pair(intf, hana::concat(existedImpls, impls));
                    };

                    return hana::unpack(intfs, hana::make_map ^ hana::on ^ mergeMap);
                }
            };

            constexpr FillImplementedMap fill_implemented_map{};

            template<typename BeanResolver, typename IndependentTypes>
            struct ChangeableBeanResolver {
                using BeanType = typename BeanResolver::BeanType;
                constexpr static auto DependOn = hana::to_tuple(
                        hana::difference(hana::to_set(BeanResolver::DependOn), hana::to_set(IndependentTypes{})));
                constexpr static auto ImplementOf = BeanResolver::ImplementOf;
            };

            struct RemoveVertexDependency {
                template<typename IndependentBeans, typename BeanVertexes>
                constexpr auto operator()(BeanVertexes &&vertexes, IndependentBeans &&beans) const {
                    auto getImplemented = [](const auto &bean) {
                        using VertexType = typename decltype(hana::typeid_(bean))::type;
                        return VertexType::InList;
                    };
                    auto IndependentTypes =
                            hana::unpack(beans, collection::tuple::merge_tuple ^ hana::on ^ getImplemented);
                    auto removeDependency = [&IndependentTypes](const auto &vertex) {
                        using VertexType = typename decltype(hana::typeid_(vertex))::type;
                        using NewBeanResolver =
                                ChangeableBeanResolver<typename VertexType::NodeType, decltype(IndependentTypes)>;
                        return hana::type_c<graph::Vertex<NewBeanResolver>>;
                    };
                    return hana::unpack(vertexes, hana::make_tuple ^ hana::on ^ removeDependency);
                }
            };

            constexpr RemoveVertexDependency remove_vertex_dependency{};
        }// namespace util
    }    // namespace dag

}// namespace summer


#endif /* A67ECB16_1415_4C05_B637_32E9D042A1E9 */
