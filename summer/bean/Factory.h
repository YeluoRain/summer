/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SUMMER_BEAN_FACTORY
#define SUMMER_BEAN_FACTORY

#include "summer/bean/Constructor.h"
#include "summer/bean/Define.h"
#include "summer/dag/Graph.h"
#include "summer/dag/Vertex.h"

namespace summer::bean::factory {

struct Beans {
    static constexpr auto Inject = [](auto&& beans) {
        using namespace boost;
        auto beanVertexs = hana::transform(beans, [](auto& bean) {
            using BeanType = typename decltype(hana::typeid_(bean))::type;
            auto beanVertex = hana::type_c<dag::graph::Vertex<define::BeanResolver<BeanType>>>;
            return beanVertex;
        });
        return dag::operation::Vertex::GetBeansInOrder(beanVertexs);
    };

    static constexpr auto MergeBeanMap = [](auto&& beanMap, auto&& parents, auto&& instance) {
        using namespace boost;
        return hana::fold_left(parents, beanMap, [&](auto&& curBeanMap, auto&& parent) {
            auto instances = hana::find(curBeanMap, parent).value_or(hana::make_tuple());
            auto pair = hana::make_pair(parent, hana::append(instances, instance));
            auto newMap = hana::erase_key(curBeanMap, parent);
            return hana::insert(newMap, pair);
        });
    };

    static constexpr auto CreateFactory = [](auto&& beans) {
        using namespace boost;
        auto independentVertexes =
            dag::operation::Vertex::GetBeansInOrder(dag::operation::Vertex::ToVertexes(beans));
        auto independentBeanResolvers =
            hana::transform(independentVertexes, dag::operation::Vertex::ToBeanResolver);
        auto context = hana::make_tuple(hana::make_map(), hana::make_map());
        return hana::fold_left(
            independentBeanResolvers, context, [](auto&& context0, auto&& beanResolver) {
                auto resolverMap = context0[hana::size_c<0>];
                auto creatorMap = context0[hana::size_c<1>];
                using BeanResolverType = typename decltype(hana::typeid_(beanResolver))::type;
                using BeanType = typename BeanResolverType::BeanType;
                auto parents = dag::operation::Vertex::GetAllParents(hana::tuple_t<BeanType>);

                auto creator = [resolverMap, creatorMap]() -> std::shared_ptr<BeanType> {
                    auto argTypes = BeanResolverType::Args;
                    auto args =
                        hana::transform(argTypes, [&resolverMap, &creatorMap](auto&& bean0) {
                            using ArgType = typename decltype(hana::typeid_(bean0))::type;
                            using RawArgType = typename traits::ArgTypeTraits<ArgType>::type;
                            auto resolvers = hana::find(resolverMap, hana::type_c<RawArgType>)
                                                 .value_or(hana::make_tuple());
                            return constructor::BeanCreatorInvoker<ArgType>::Invoke(resolvers,
                                                                                    creatorMap);
                        });
                    return hana::unpack(std::move(args), [](auto&&... args0) {
                        if constexpr (traits::IsCreatorWrapper<
                                          typename BeanResolverType::Type>::value) {
                            using CreatorRetType = typename traits::ConstructorTraits<
                                decltype(BeanResolverType::Type::Creator)>::RetType;
                            if constexpr (std::is_same_v<CreatorRetType,
                                                        std::shared_ptr<BeanType>>) {
                                return BeanResolverType::Type::Creator(
                                    std::forward<decltype(args0)>(args0)...);
                            } else if constexpr (std::is_same_v<CreatorRetType,
                                                        std::unique_ptr<BeanType>>) {
                                auto uniquePtr = BeanResolverType::Type::Creator(
                                    std::forward<decltype(args0)>(args0)...);
                                return std::shared_ptr<BeanType>(uniquePtr.release());
                            } else {
                                return std::shared_ptr<BeanType>(
                                    BeanResolverType::Type::Creator(
                                        std::forward<decltype(args0)>(args0)...));
                            }
                        } else {
                            return std::shared_ptr<BeanType>(
                                new BeanType(std::forward<decltype(args0)>(args0)...));
                        }
                    });
                };

                auto pair =
                    hana::make_pair(hana::type_c<BeanType>,
                                    std::make_shared<constructor::BeanCreator<BeanType>>(
                                        typename constructor::BeanCreator<BeanType>::CreatorFuncType(creator)));
                auto newCreatorMap = hana::insert(creatorMap, pair);
                return hana::make_tuple(MergeBeanMap(resolverMap, parents, hana::type_c<BeanType>),
                                        newCreatorMap);
            });
    };
};
}  // namespace summer::bean::factory

#endif /* SUMMER_BEAN_FACTORY */