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
        auto independentBeans =
            hana::transform(independentVertexes, dag::operation::Vertex::ToBean);
        auto beanCreatorMap = hana::make_map();
        return hana::fold_left(
            independentBeans, beanCreatorMap, [](auto&& creatorMap, auto&& bean) {
                using BeanType = typename decltype(hana::typeid_(bean))::type;
                using BeanResolverType = define::BeanResolver<BeanType>;
                auto parents = dag::operation::Vertex::GetAllParents(hana::make_tuple(bean));
                auto creator = [creatorMap] {
                    auto argTypes = BeanResolverType::Args;
                    auto args = hana::transform(argTypes, [&creatorMap](auto&& bean0) {
                        using ArgType = typename decltype(hana::typeid_(bean0))::type;
                        using RawArgType = typename traits::ArgTypeTraits<ArgType>::type;
                        auto creators = hana::find(creatorMap, hana::type_c<RawArgType>)
                                            .value_or(hana::make_tuple());
                        return constructor::BeanCreatorInvoker<ArgType>::Invoke(creators);
                    });
                    return hana::unpack(std::move(args), [](auto&&... args0) {
                        if constexpr (traits::IsCreatorWrapper<BeanType>::value) {
                            return BeanType::Creator(std::forward<decltype(args0)>(args0)...);
                        } else {
                            return new BeanType(std::forward<decltype(args0)>(args0)...);
                        }
                    });
                };
                return MergeBeanMap(
                    creatorMap, parents,
                    constructor::BeanCreator<typename BeanResolverType::BeanType>{ creator });
            });
    };
};
}  // namespace summer::bean::factory

#endif /* SUMMER_BEAN_FACTORY */
