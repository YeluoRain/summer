//
// Created by Zhongwen Wang on 2023/11/6.
//

#include "BeanTest.h"
#include "TestClass.h"
#include "dag.h"

using namespace boost;
using namespace summer::dag;

auto beanResolverMap = hana::make_map(hana::make_pair(hana::type_c<AImpl>, hana::type_c<BeanResolverA>),
                                      hana::make_pair(hana::type_c<BImpl>, hana::type_c<BeanResolverB>),
                                      hana::make_pair(hana::type_c<CImpl>, hana::type_c<BeanResolverC>));

auto beanVertexes = hana::to_tuple(hana::transform(hana::values(beanResolverMap), [](const auto &resolver) {
    using ResolverType = typename decltype(hana::typeid_(resolver))::type;
    return hana::type_c<graph::Vertex<ResolverType>>;
}));

void BeanTest::SetUp() {}

void BeanTest::TearDown() {}

TEST_F(BeanTest, test_create_beanResolverMap_success) {
    BOOST_HANA_ASSERT(beanResolverMap[hana::type_c<AImpl>] == hana::type_c<BeanResolverA>);
    BOOST_HANA_ASSERT(hana::find(beanResolverMap, hana::type_c<int>) == hana::nothing);
}

TEST_F(BeanTest, test_create_beanVertexes_success) {
    auto expect =
            hana::make_tuple(hana::type_c<graph::Vertex<BeanResolverA>>, hana::type_c<graph::Vertex<BeanResolverB>>,
                             hana::type_c<graph::Vertex<BeanResolverC>>);
    BOOST_HANA_ASSERT(beanVertexes == expect);
}

TEST_F(BeanTest, test_generate_dependency_success) {
    auto independentBeans = util::get_independent_beans(beanVertexes);
    auto implementedMap = util::fill_implemented_map(hana::make_map(), independentBeans);
    auto expect = hana::make_map(hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>)));
    BOOST_HANA_ASSERT(expect == implementedMap);

    auto nextMap = util::fill_implemented_map(implementedMap, beanVertexes);
    auto expect2 =
            hana::make_map(hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>, hana::type_c<AImpl>)),
                           hana::make_pair(hana::type_c<B>, hana::make_tuple(hana::type_c<BImpl>)),
                           hana::make_pair(hana::type_c<C>, hana::make_tuple(hana::type_c<CImpl>)));
    // std::cout << summer::print::to_string(expect2) << std::endl;
    BOOST_HANA_ASSERT(expect2 == nextMap);
}

TEST_F(BeanTest, test_remove_vertex_dependency) {
    auto independentBeans = util::get_independent_beans(beanVertexes);
    auto newBeans = hana::to_tuple(hana::difference(hana::to_set(beanVertexes), hana::to_set(independentBeans)));
    auto removedDependencyBeans = util::remove_vertex_dependency(newBeans, independentBeans);
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
    auto test = hana::make_tuple(hana::type_c<int>, hana::type_c<float>, hana::type_c<char>);
    auto result = hana::while_([](const auto &tuple) { return hana::greater(hana::size(tuple), hana::size_c<0>); },
                               test, [](const auto &tuple) { return hana::drop_front(tuple); });
    BOOST_HANA_ASSERT(hana::is_empty(result));
}

TEST_F(BeanTest, test_get_all_independent_beans) {
    // (independentBeans, implementedMap, beanVertexes)
    auto summerContext = hana::make_tuple(hana::make_tuple(), hana::make_map(), beanVertexes);
    auto result = hana::while_(
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
                auto curIndependentVertexes = util::get_independent_beans(vertexes);
                // 将这些独立Bean加载到MAP TODO: 有问题
                auto nextMap = util::fill_implemented_map(implementedMap, curIndependentVertexes);
                auto nextIndependentVertexes = hana::concat(independentVertexes, curIndependentVertexes);
                // std::cout << summer::print::to_string(implementedMap) << std::endl;
                // std::cout << summer::print::to_string(nextMap) << std::endl;
                // std::cout << summer::print::to_string(curIndependentVertexes) << std::endl;
                // 将独立Bean移除后剩下的非独立Bean
                auto restVertexes =
                        hana::to_tuple(hana::difference(hana::to_set(vertexes), hana::to_set(curIndependentVertexes)));
                auto nextVertexes = util::remove_vertex_dependency(restVertexes, curIndependentVertexes);
                return hana::make_tuple(nextIndependentVertexes, nextMap, nextVertexes);
            });
    auto map = hana::at(result, hana::int_c<1>);
    auto expect = hana::make_map(
        hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>)),
        hana::make_pair(hana::type_c<B>, hana::make_tuple(hana::type_c<BImpl>)),
        hana::make_pair(hana::type_c<C>, hana::make_tuple(hana::type_c<CImpl>))
    );
    BOOST_HANA_ASSERT(map == expect);
}
