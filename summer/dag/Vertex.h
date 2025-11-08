//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef SUMMER_DAG_VERTEX
#define SUMMER_DAG_VERTEX

#include "summer/bean/Define.h"
#include "summer/dag/GraphOperation.h"
#include "summer/util/Print.h"

namespace summer::dag::operation {
namespace hana = boost::hana;

struct Vertex {
    static constexpr auto ToVertex = [](auto&& bean) {
        using namespace boost;
        using BeanType = typename decltype(hana::typeid_(bean))::type;
        return hana::type_c<dag::graph::Vertex<bean::define::BeanResolver<BeanType>>>;
    };

    static constexpr auto ToVertexes = [](auto&& beans) {
        return boost::hana::transform(beans, ToVertex);
    };

    static constexpr auto ToBeanResovler = [](auto&& vertex) {
        using namespace boost;
        using VertexType = typename decltype(hana::typeid_(vertex))::type;
        return hana::type_c<typename VertexType::NodeType>;
    };

    static constexpr auto GetIndependentBeans = [](auto&& vertexes) {
        return hana::filter(vertexes, [](const auto& vertex) {
            using VertexType = typename decltype(hana::typeid_(vertex))::type;
            return VertexType::OutDegree == hana::size_c<0>;
        });
    };

    static constexpr auto GetAllParents = [](auto&& beans) {
        // allParents, lefts
        auto allParentsContext = hana::make_tuple(hana::make_tuple(), beans);
        auto result = hana::while_(
            [](auto&& context) {
                auto lefts = hana::at(context, hana::size_c<1>);
                return hana::greater(hana::size(lefts), hana::size_c<0>);
            },
            allParentsContext,
            [](auto&& context) {
                auto allParents = hana::at(context, hana::size_c<0>);
                auto lefts = hana::at(context, hana::size_c<1>);
                auto nextParents = util::collection::tuple::Merge(allParents, lefts);
                auto leftParents = hana::transform(lefts, [](auto&& left) {
                    using Type = typename decltype(hana::typeid_(left))::type;
                    return GetDerivedBase<Type>::Bases;
                });
                auto nextLefts = hana::unpack(leftParents, util::collection::tuple::Merge);
                return hana::make_tuple(nextParents, nextLefts);
            });
        return hana::at(result, hana::size_c<0>);
    };

    static constexpr auto RemoveVertexDependency = [](auto&& vertexes, auto&& beans) {
        auto getImplemented = [](const auto& bean) {
            using VertexType = typename decltype(hana::typeid_(bean))::type;
            return VertexType::InList;
        };
        auto IndependentTypes =
            hana::unpack(beans, util::collection::tuple::Merge ^ hana::on ^ getImplemented);
        auto removeDependency = [](const auto& vertex) {
            using VertexType = typename decltype(hana::typeid_(vertex))::type;
            using NewBeanResolver =
                ChangeableBeanResolver<typename VertexType::NodeType, decltype(IndependentTypes)>;
            return hana::type_c<graph::Vertex<NewBeanResolver>>;
        };
        return hana::unpack(vertexes, hana::make_tuple ^ hana::on ^ removeDependency);
    };

    static constexpr auto CheckHasIndependentBeans = [](auto&& vertexes) {
        return hana::any_of(vertexes, [](const auto& vertex) {
            using VertexType = typename decltype(hana::typeid_(vertex))::type;
            return VertexType::OutDegree == hana::size_c<0>;
        });
    };

    static constexpr auto GetBeansInOrder = [](auto&& vertexs) {
        using namespace boost;
        auto summerContext = hana::make_tuple(hana::make_tuple(), hana::true_c, vertexs);
        auto context = hana::while_(
            [](const auto& context0) {
                auto vertexes = hana::at(context0, hana::int_c<2>);
                auto checkResult = hana::at(context0, hana::int_c<1>);
                return hana::and_(checkResult,
                                  hana::greater(hana::size(vertexes), hana::size_c<0>));
            },
            summerContext,
            [](const auto& context0) {
                // 第一个参数为独立点列表，有序
                auto independentVertexes = hana::at(context0, hana::int_c<0>);
                // 第三个为剩下的点
                auto vertexes = hana::at(context0, hana::int_c<2>);
                auto checkResult = CheckHasIndependentBeans(vertexes);
                if constexpr (hana::not_(checkResult)) {
                    auto nextBeans = util::collection::tuple::RemoveDuplicates(
                        hana::flatten(hana::transform(vertexes, [](const auto& vertex) {
                            using VertexType = typename decltype(hana::typeid_(vertex))::type;
                            return VertexType::OutList;
                        })));
                    auto missingBeans = hana::remove_if(nextBeans, [&](const auto& bean) {
                        return hana::contains(ToBeans(independentVertexes), bean);
                    });
                    util::print::DependencyErrorPrint<decltype(missingBeans)>::Print();
                }
                auto curIndependentVertexes = GetIndependentBeans(vertexes);
                auto nextIndependentVertexes =
                    hana::concat(independentVertexes, curIndependentVertexes);
                // 将独立Bean移除后剩下的非独立Bean
                auto restVertexes =
                    util::collection::tuple::TupleMinus(vertexes, curIndependentVertexes);
                auto nextVertexes = RemoveVertexDependency(restVertexes, curIndependentVertexes);
                return hana::make_tuple(nextIndependentVertexes, checkResult, nextVertexes);
            });
        return hana::at(context, hana::int_c<0>);
    };
};
}  // namespace summer::dag::operation

#endif /* SUMMER_DAG_VERTEX */
