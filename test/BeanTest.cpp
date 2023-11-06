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

auto beanVertexes = hana::transform(hana::values(beanResolverMap), [](const auto &resolver) {
    using ResolverType = typename decltype(hana::typeid_(resolver))::type;
    return hana::type_c<graph::Vertex<ResolverType>>;
});

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
    auto implementedMap = util::fillImplementedMap(hana::make_map(), independentBeans);
    auto expect = hana::make_map(hana::make_pair(hana::type_c<A>, hana::make_tuple(hana::type_c<AImpl>)));
    BOOST_HANA_ASSERT(expect == implementedMap);
}
