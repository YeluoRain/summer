//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef OPERATION_H
#define OPERATION_H

#include "bean/Define.h"
#include "boost/describe.hpp"
#include "dag/detail/GraphOperation.h"

namespace summer::dag::operation {
namespace hana = boost::hana;

struct Vertex {

  static constexpr auto ToVertex = [](auto &&bean) {
    using namespace boost;
    using BeanType = typename decltype(hana::typeid_(bean))::type;
    return hana::type_c<
        dag::graph::Vertex<bean::define::BeanResolver<BeanType>>>;
  };

  static constexpr auto ToVertexes = [](auto &&beans) {
    return boost::hana::transform(beans, ToVertex);
  };

  static constexpr auto GetIndependentBeans = [](auto &&vertexes) {
    return hana::filter(vertexes, [](const auto &vertex) {
      using VertexType = typename decltype(hana::typeid_(vertex))::type;
      return VertexType::OutDegree == hana::size_c<0>;
    });
  };

  static constexpr auto GetAllParents = [](auto &&beans) {
    // allParents, lefts
    auto allParentsContext = hana::make_tuple(hana::make_tuple(), beans);
    auto result = hana::while_(
        [](auto &&context) {
          auto lefts = hana::at(context, hana::size_c<1>);
          return hana::greater(hana::size(lefts), hana::size_c<0>);
        },
        allParentsContext,
        [](auto &&context) {
          auto allParents = hana::at(context, hana::size_c<0>);
          auto lefts = hana::at(context, hana::size_c<1>);
          auto nextParents = util::collection::tuple::Merge(allParents, lefts);
          auto leftParents = hana::transform(lefts, [](auto &&left) {
            using Type = typename decltype(hana::typeid_(left))::type;
            return detail::GetDerivedBase<Type>::Bases;
          });
          auto nextLefts =
              hana::unpack(leftParents, util::collection::tuple::Merge);
          return hana::make_tuple(nextParents, nextLefts);
        });
    return hana::at(result, hana::size_c<0>);
  };

  static constexpr auto FillImplementedMap = [](auto &&map, auto &&beans) {
    // 转换成Tuple(Pair(Intf, Impl)...)
    auto processBean = [](auto &&bean) {
      using VertexType = typename decltype(hana::typeid_(bean))::type;
      auto buildPair = [](auto &&inType) {
        return hana::make_pair(inType,
                               hana::type_c<typename VertexType::BeanType>);
      };
      return hana::unpack(GetAllParents(VertexType::InList),
                          hana::make_tuple ^ hana::on ^ buildPair);
    };
    // 合并内层
    // Tuple(Tuple(Pair(Intf, Impl)...), Tuple(Pair(Intf, Impl)...), ...) ->
    // Tuple(Pair(Intf, Impl)...)
    auto intf2implTuple = hana::unpack(beans, util::collection::tuple::Merge ^
                                                  hana::on ^ processBean);
    // Tuple(Intf...)
    auto inputKeys =
        hana::unpack(intf2implTuple, hana::make_tuple ^ hana::on ^ hana::first);
    auto intfKeys = hana::concat(hana::to_tuple(hana::keys(map)), inputKeys);
    // 去除重复key
    auto intfs = util::collection::tuple::RemoveDuplicates(intfKeys);
    auto mergeMap = [&map, &intf2implTuple](auto &&intf) {
      // 找出了实现了intf的Pairs
      auto implPairs = hana::filter(intf2implTuple, [&intf](const auto &i) {
        return hana::first(i) == intf;
      });
      // 取Impls
      auto impls = hana::transform(
          implPairs, [](const auto &p) { return hana::second(p); });
      auto existedImpls = hana::find(map, intf).value_or(hana::make_tuple());
      // std::cout << summer::print::to_string(result) << std::endl;
      return hana::make_pair(intf, hana::concat(existedImpls, impls));
    };

    return hana::unpack(intfs, hana::make_map ^ hana::on ^ mergeMap);
  };

  static constexpr auto RemoveVertexDependency = [](auto &&vertexes,
                                                    auto &&beans) {
    auto getImplemented = [](const auto &bean) {
      using VertexType = typename decltype(hana::typeid_(bean))::type;
      return VertexType::InList;
    };
    auto IndependentTypes = hana::unpack(beans, util::collection::tuple::Merge ^
                                                    hana::on ^ getImplemented);
    auto removeDependency = [&IndependentTypes](const auto &vertex) {
      using VertexType = typename decltype(hana::typeid_(vertex))::type;
      using NewBeanResolver =
          detail::ChangeableBeanResolver<typename VertexType::NodeType,
                                         decltype(IndependentTypes)>;
      return hana::type_c<graph::Vertex<NewBeanResolver>>;
    };
    return hana::unpack(vertexes,
                        hana::make_tuple ^ hana::on ^ removeDependency);
  };

  static constexpr auto ChecHasIndependentBeans = [](auto &&vertexes) {
    return hana::any_of(vertexes, [](const auto &vertex) {
      using VertexType = typename decltype(hana::typeid_(vertex))::type;
      return VertexType::OutDegree == hana::size_c<0>;
    });
  };

  static constexpr auto GetBeansInOrder = [](auto &&vertexs) {
    using namespace boost;
    auto summerContext =
        hana::make_tuple(hana::make_tuple(), hana::true_c, vertexs);
    auto context = hana::while_(
        [](const auto &context) {
          auto vertexes = hana::at(context, hana::int_c<2>);
          auto checkResult = hana::at(context, hana::int_c<1>);
          return hana::and_(checkResult, hana::greater(hana::size(vertexes),
                                                       hana::size_c<0>));
        },
        summerContext,
        [](const auto &context) {
          // 第一个参数为独立点列表，有序
          auto independentVertexes = hana::at(context, hana::int_c<0>);
          // 第三个为剩下的点
          auto vertexes = hana::at(context, hana::int_c<2>);
          auto checkResult = ChecHasIndependentBeans(vertexes);
          static_assert(hana::value(checkResult), "Graph has cycle or missing "
                                                  "dependency");
          auto curIndependentVertexes = GetIndependentBeans(vertexes);
          auto nextIndependentVertexes =
              hana::concat(independentVertexes, curIndependentVertexes);
          // 将独立Bean移除后剩下的非独立Bean
          auto restVertexes = util::collection::tuple::TupleMinus(
              vertexes, curIndependentVertexes);
          auto nextVertexes =
              RemoveVertexDependency(restVertexes, curIndependentVertexes);
          return hana::make_tuple(nextIndependentVertexes, checkResult,
                                  nextVertexes);
        });
    return hana::at(context, hana::int_c<0>);
  };
};
} // namespace summer::dag::operation

#endif // OPERATION_H
