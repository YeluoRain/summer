//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef DAG_DETAIL_GRAPHOPERATION
#define DAG_DETAIL_GRAPHOPERATION

#include "boost/describe.hpp"
#include "summer/dag/Graph.h"
#include "summer/util/Collection.h"

namespace summer::dag::operation::detail {

namespace hana = boost::hana;
namespace describe = boost::describe;
namespace mp11 = boost::mp11;

template <typename Bean, typename Enable = void>
struct GetDerivedBase {
    constexpr static auto Bases = hana::make_tuple();
};

template <typename Bean>
struct GetDerivedBase<Bean, std::enable_if_t<describe::has_describe_bases<Bean>::value>> {
    constexpr static auto Bases = [] {
        using BaseDescriptors = describe::describe_bases<Bean, describe::mod_any_access>;
        using BaseTuple = mp11::mp_rename<BaseDescriptors, hana::tuple>;
        return hana::transform(
            BaseTuple(), [](auto base) { return hana::type_c<typename decltype(base)::type>; });
    }();
};

template <typename BeanResolver, typename IndependentTypes>
struct ChangeableBeanResolver {
    using BeanType = typename BeanResolver::BeanType;
    constexpr static auto DependOn = hana::remove_if(
        BeanResolver::DependOn, [](auto&& dep) { return hana::contains(IndependentTypes{}, dep); });
    constexpr static auto ImplementOf = BeanResolver::ImplementOf;
};

struct RemoveVertexDependencyFuncType {
    template <typename IndependentBeans, typename BeanVertexes>
    constexpr auto operator()(BeanVertexes&& vertexes, IndependentBeans&& beans) const {
        auto getImplemented = [](const auto& bean) {
            using VertexType = typename decltype(hana::typeid_(bean))::type;
            return VertexType::InList;
        };
        auto IndependentTypes =
            hana::unpack(beans, util::collection::tuple::Merge ^ hana::on ^ getImplemented);
        auto removeDependency = [&IndependentTypes](const auto& vertex) {
            using VertexType = typename decltype(hana::typeid_(vertex))::type;
            using NewBeanResolver =
                ChangeableBeanResolver<typename VertexType::NodeType, decltype(IndependentTypes)>;
            return hana::type_c<graph::Vertex<NewBeanResolver>>;
        };
        return hana::unpack(vertexes, hana::make_tuple ^ hana::on ^ removeDependency);
    }
};

}  // namespace summer::dag::operation::detail

#endif /* DAG_DETAIL_GRAPHOPERATION */
