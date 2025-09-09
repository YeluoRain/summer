//
// Created by Zhongwen Wang on 2023/11/6.
//

#include "BeanTest.h"
#include "Summer.h"
#include "TestClass.h"
#include "boost/hana/fwd/transform.hpp"
#include "boost/hana/fwd/type.hpp"
#include <algorithm>

using namespace boost;
using namespace summer::dag;
using namespace summer::util;

auto beanResolverMap = hana::make_map(
    hana::make_pair(hana::type_c<AImpl>, hana::type_c<BeanResolverA>),
    hana::make_pair(hana::type_c<BImpl>, hana::type_c<BeanResolverB>),
    hana::make_pair(hana::type_c<CImpl>, hana::type_c<BeanResolverC>));

auto beanVertexes = hana::to_tuple(
    hana::transform(hana::values(beanResolverMap), [](const auto &resolver) {
      using ResolverType = typename decltype(hana::typeid_(resolver))::type;
      return hana::type_c<graph::Vertex<ResolverType>>;
    }));

void BeanTest::SetUp() {}

void BeanTest::TearDown() {}

TEST_F(BeanTest, test_create_beanResolverMap_success) {
  BOOST_HANA_ASSERT(beanResolverMap[hana::type_c<AImpl>] ==
                    hana::type_c<BeanResolverA>);
  BOOST_HANA_ASSERT(hana::find(beanResolverMap, hana::type_c<int>) ==
                    hana::nothing);
}

TEST_F(BeanTest, test_create_beanVertexes_success) {
  auto expect = hana::make_tuple(hana::type_c<graph::Vertex<BeanResolverA>>,
                                 hana::type_c<graph::Vertex<BeanResolverB>>,
                                 hana::type_c<graph::Vertex<BeanResolverC>>);
  BOOST_HANA_ASSERT(beanVertexes == expect);
}

TEST_F(BeanTest, test_generate_dependency_success) {
  auto independentBeans = operation::GetIndependentBeans(beanVertexes);
  auto implementedMap =
      operation::FillImplementedMap(hana::make_map(), independentBeans);
  auto expect = hana::make_map(
      hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>)));
  BOOST_HANA_ASSERT(expect == implementedMap);

  auto nextMap = operation::FillImplementedMap(implementedMap, beanVertexes);
  auto expect2 = hana::make_map(
      hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>,
                                                        hana::type_c<AImpl>)),
      hana::make_pair(hana::type_c<B>, hana::make_tuple(hana::type_c<BImpl>)),
      hana::make_pair(hana::type_c<C>, hana::make_tuple(hana::type_c<CImpl>)));
  // std::cout << summer::print::to_string(expect2) << std::endl;
  BOOST_HANA_ASSERT(expect2 == nextMap);
}

TEST_F(BeanTest, test_remove_vertex_dependency) {
  auto independentBeans = operation::GetIndependentBeans(beanVertexes);
  auto newBeans = hana::to_tuple(hana::difference(
      hana::to_set(beanVertexes), hana::to_set(independentBeans)));
  auto removedDependencyBeans =
      operation::RemoveVertexDependency(newBeans, independentBeans);
  auto getDependency = [](const auto &vertex) {
    using VertexType = typename decltype(hana::typeid_(vertex))::type;
    return VertexType::OutList;
  };
  auto first = hana::front(removedDependencyBeans);
  using VertexType = typename decltype(hana::typeid_(first))::type;
  BOOST_HANA_ASSERT(hana::is_empty(VertexType::OutList));

  auto second = hana::back(removedDependencyBeans);
  using VertexType2 = typename decltype(hana::typeid_(second))::type;
  BOOST_HANA_ASSERT(VertexType2::OutDegree == hana::size_c<1>);
}

TEST_F(BeanTest, test_while) {
  auto test = hana::make_tuple(hana::type_c<int>, hana::type_c<float>,
                               hana::type_c<char>);
  auto result = hana::while_(
      [](const auto &tuple) {
        return hana::greater(hana::size(tuple), hana::size_c<0>);
      },
      test, [](const auto &tuple) { return hana::drop_front(tuple); });
  BOOST_HANA_ASSERT(hana::is_empty(result));
}

TEST_F(BeanTest, test_get_all_independent_beans) {
  // (independentBeans, implementedMap, beanVertexes)
  auto result = operation::GenerateBeanResolverContext(beanVertexes);
  auto independentBeans = hana::at(result, hana::int_c<0>);
  auto intf2ImplMap = hana::at(result, hana::int_c<1>);
  auto r = hana::transform(independentBeans, [](auto &vertex) {
    using VertexType = typename decltype(hana::typeid_(vertex))::type;
    return hana::type_c<typename VertexType::BeanType>;
  });
  std::cout << print::ToString(r) << std::endl;
  auto expect = hana::make_map(
      hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>)),
      hana::make_pair(hana::type_c<B>, hana::make_tuple(hana::type_c<BImpl>)),
      hana::make_pair(hana::type_c<C>, hana::make_tuple(hana::type_c<CImpl>)));
  std::cout << print::ToString(result) << std::endl;
  BOOST_HANA_ASSERT(intf2ImplMap == expect);
}

TEST_F(BeanTest, test_get_all_parents) {
  auto input = hana::make_tuple(hana::type_c<A2>);
  auto output = operation::GetAllParents(input);
  std::cout << print::ToString(output) << std::endl;
}