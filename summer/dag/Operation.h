//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef OPERATION_H
#define OPERATION_H

#include "dag/detail/GraphOperation.h"

namespace summer::dag::operation {
    constexpr detail::GetIndependentBeansFuncType GetIndependentBeans{};
    constexpr detail::FillImplementedMapFuncType FillImplementedMap{};
    constexpr detail::RemoveVertexDependencyFuncType RemoveVertexDependency{};
    constexpr detail::GetAllParentsFuncType GetAllParents{};

    struct GenerateBeanResolverContextFuncType {
        template<typename Vertexs>
        constexpr auto operator()(Vertexs &&vertexs) const {
            using namespace boost;
            auto summerContext = hana::make_tuple(hana::make_tuple(), hana::make_map(), vertexs);
            return hana::while_(
                    [](const auto &context) {
                        auto vertexes = hana::at(context, hana::int_c<2>);
                        return hana::greater(hana::size(vertexes), hana::size_c<0>);
                    },
                    summerContext,
                    [](const auto &context) {
                        // 第一个参数为独立点列表，有序
                        auto independentVertexes = hana::at(context, hana::int_c<0>);
                        // 第二个为实现了哪些接口和背后的实现类列表
                        auto implementedMap = hana::at(context, hana::int_c<1>);
                        // 第三个为剩下的点
                        auto vertexes = hana::at(context, hana::int_c<2>);

                        // 当前循环中找到的独立Bean
                        auto curIndependentVertexes = operation::GetIndependentBeans(vertexes);
                        // 将这些独立Bean加载到MAP
                        auto nextMap = operation::FillImplementedMap(implementedMap, curIndependentVertexes);
                        auto nextIndependentVertexes = hana::concat(independentVertexes, curIndependentVertexes);
                        // 将独立Bean移除后剩下的非独立Bean
                        auto restVertexes =
                                hana::to_tuple(
                                        hana::difference(hana::to_set(vertexes), hana::to_set(curIndependentVertexes)));
                        auto nextVertexes = operation::RemoveVertexDependency(restVertexes, curIndependentVertexes);
                        return hana::make_tuple(nextIndependentVertexes, nextMap, nextVertexes);
                    });
        }
    };

    constexpr GenerateBeanResolverContextFuncType GenerateBeanResolverContext{};

}

#endif //OPERATION_H
