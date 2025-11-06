//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef DAG_DETAIL_GRAPHOPERATION
#define DAG_DETAIL_GRAPHOPERATION

#include "boost/describe.hpp"
#include "summer/dag/Graph.h"
#include "summer/util/Collection.h"

namespace summer::dag::operation {

namespace hana = boost::hana;
namespace describe = boost::describe;
namespace mp11 = boost::mp11;

template <typename BeanType, typename Enable = void>
struct GetDerivedBase {
    constexpr static auto Bases = hana::make_tuple();
};

template <typename BeanType>
struct GetDerivedBase<BeanType, std::enable_if_t<describe::has_describe_bases<BeanType>::value>> {
    constexpr static auto Bases = [] {
        using BaseDescriptors = describe::describe_bases<BeanType, describe::mod_any_access>;
        using BaseTuple = mp11::mp_rename<BaseDescriptors, hana::tuple>;
        return hana::transform(
            BaseTuple(), [](auto base) { return hana::type_c<typename decltype(base)::type>; });
    }();
};

template <typename BeanResolver, typename IndependentTypes>
struct ChangeableBeanResolver {
    using Type = typename BeanResolver::Type;
    using BeanType = typename BeanResolver::BeanType;
    constexpr static auto DependOn = hana::remove_if(
        BeanResolver::DependOn, [](auto&& dep) { return hana::contains(IndependentTypes{}, dep); });
    constexpr static auto ImplementOf = BeanResolver::ImplementOf;

    constexpr static auto Args = BeanResolver::Args;
};

}  // namespace summer::dag::operation

#endif /* DAG_DETAIL_GRAPHOPERATION */
