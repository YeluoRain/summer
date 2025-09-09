//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef VERTEXOPERATION_H
#define VERTEXOPERATION_H

#include "util/Collection.h"
#include "boost/describe.hpp"

namespace summer::dag::operation::detail {
    using namespace boost;

    struct GetIndependentBeansFuncType {
        template<typename BeanVertexes>
        constexpr auto operator()(BeanVertexes &&vertexes) const {
            return hana::filter(vertexes, [](const auto &vertex) {
                using VertexType = typename decltype(hana::typeid_(vertex))::type;
                return VertexType::OutDegree == hana::size_c<0>;
            });
        }
    };

    template<typename Bean, typename Enable = void>
    struct GetDerivedBase {
        constexpr static auto Bases = hana::make_tuple();
    };

    template<typename Bean>
    struct GetDerivedBase<Bean, std::enable_if_t<describe::has_describe_bases<Bean>::value>> {
        constexpr static auto Bases = [] {
            using BaseDescriptors = describe::describe_bases<Bean, describe::mod_any_access>;
            using BaseTuple = mp11::mp_rename<BaseDescriptors, hana::tuple>;
            return hana::transform(BaseTuple(), [](auto base) { return hana::type_c<typename decltype(base)::type>; });
        }();
    };

    struct GetAllParentsFuncType {
        template<typename Beans>
        constexpr auto operator()(Beans &&beans) const {
            // allParents, lefts
            auto allParentsContext = hana::make_tuple(hana::make_tuple(), beans);
            auto result = hana::while_([](auto &&context) {
                auto lefts = hana::at(context, hana::size_c<1>);
                return hana::greater(hana::size(lefts), hana::size_c<0>);
            }, allParentsContext, [](auto &&context) {
                auto allParents = hana::at(context, hana::size_c<0>);
                auto lefts = hana::at(context, hana::size_c<1>);
                auto nextParents = util::collection::tuple::Merge(allParents, lefts);
                auto leftParents = hana::transform(lefts, [](auto &&left) {
                    using Type = typename decltype(hana::typeid_(left))::type;
                    return GetDerivedBase<Type>::Bases;
                });
                auto nextLefts = hana::unpack(leftParents, util::collection::tuple::Merge);
                return hana::make_tuple(nextParents, nextLefts);
            });
            return hana::at(result, hana::size_c<0>);
        }
    };

    constexpr GetAllParentsFuncType _GetAllParents{};

    struct FillImplementedMapFuncType {
        template<typename ImplementedMap, typename IndependentBeans>
        constexpr auto operator()(ImplementedMap &&map, IndependentBeans &&beans) const {
            // 转换成Tuple(Pair(Intf, Impl)...)
            auto processBean = [](auto &&bean) {
                using VertexType = typename decltype(hana::typeid_(bean))::type;
                auto buildPair = [](auto &&inType) {
                    return hana::make_pair(inType, hana::type_c<typename VertexType::BeanType>);
                };
                return hana::unpack(_GetAllParents(VertexType::InList), hana::make_tuple ^ hana::on ^ buildPair);
            };
            // 合并内层
            // Tuple(Tuple(Pair(Intf, Impl)...), Tuple(Pair(Intf, Impl)...), ...) -> Tuple(Pair(Intf, Impl)...)
            auto intf2implTuple = hana::unpack(
                    beans, util::collection::tuple::Merge ^ hana::on ^ processBean);
            // Tuple(Intf...)
            auto inputKeys = hana::unpack(intf2implTuple, hana::make_tuple ^ hana::on ^ hana::first);
            auto intfKeys = hana::concat(hana::to_tuple(hana::keys(map)), inputKeys);
            // 去除重复key
            auto intfs = util::collection::tuple::RemoveDuplicates(intfKeys);
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


    template<typename BeanResolver, typename IndependentTypes>
    struct ChangeableBeanResolver {
        using BeanType = typename BeanResolver::BeanType;
        constexpr static auto DependOn = hana::to_tuple(
                hana::difference(hana::to_set(BeanResolver::DependOn), hana::to_set(IndependentTypes{})));
        constexpr static auto ImplementOf = BeanResolver::ImplementOf;
    };

    struct RemoveVertexDependencyFuncType {
        template<typename IndependentBeans, typename BeanVertexes>
        constexpr auto operator()(BeanVertexes &&vertexes, IndependentBeans &&beans) const {
            auto getImplemented = [](const auto &bean) {
                using VertexType = typename decltype(hana::typeid_(bean))::type;
                return VertexType::InList;
            };
            auto IndependentTypes =
                    hana::unpack(beans, util::collection::tuple::Merge ^ hana::on ^ getImplemented);
            auto removeDependency = [&IndependentTypes](const auto &vertex) {
                using VertexType = typename decltype(hana::typeid_(vertex))::type;
                using NewBeanResolver =
                        ChangeableBeanResolver<typename VertexType::NodeType, decltype(IndependentTypes)>;
                return hana::type_c<graph::Vertex<NewBeanResolver>>;
            };
            return hana::unpack(vertexes, hana::make_tuple ^ hana::on ^ removeDependency);
        }
    };


}

#endif //VERTEXOPERATION_H
