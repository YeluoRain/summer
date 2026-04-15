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

#ifndef SUMMER_DAG_GRAPHOPERATION
#define SUMMER_DAG_GRAPHOPERATION

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

#endif /* SUMMER_DAG_GRAPHOPERATION */
