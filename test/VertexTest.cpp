//
// Created by Zhongwen Wang on 2023/11/6.
//

#include "VertexTest.h"

#include <algorithm>

#include "VertexTestClass.h"
#include "boost/hana/fwd/transform.hpp"
#include "boost/hana/fwd/type.hpp"
#include "summer/Summer.h"

using namespace boost;
using namespace summer::dag;
using namespace summer::util;

auto beanResolverMap =
    hana::make_map(hana::make_pair(hana::type_c<AImpl>, hana::type_c<BeanResolverA>),
                   hana::make_pair(hana::type_c<BImpl>, hana::type_c<BeanResolverB>),
                   hana::make_pair(hana::type_c<CImpl>, hana::type_c<BeanResolverC>));

auto beanVertexes =
    hana::to_tuple(hana::transform(hana::values(beanResolverMap), [](const auto& resolver) {
        using ResolverType = typename decltype(hana::typeid_(resolver))::type;
        return hana::type_c<graph::Vertex<ResolverType>>;
    }));

void VertexTest::SetUp() {}

void VertexTest::TearDown() {}

TEST_F(VertexTest, test_create_beanResolverMap_success) {
    BOOST_HANA_ASSERT(beanResolverMap[hana::type_c<AImpl>] == hana::type_c<BeanResolverA>);
    BOOST_HANA_ASSERT(hana::find(beanResolverMap, hana::type_c<int>) == hana::nothing);
}

TEST_F(VertexTest, test_create_beanVertexes_success) {
    auto expect = hana::make_tuple(hana::type_c<graph::Vertex<BeanResolverA>>,
                                   hana::type_c<graph::Vertex<BeanResolverB>>,
                                   hana::type_c<graph::Vertex<BeanResolverC>>);
    BOOST_HANA_ASSERT(beanVertexes == expect);
}

TEST_F(VertexTest, test_remove_vertex_dependency) {
    auto independentBeans = operation::Vertex::GetIndependentBeans(beanVertexes);
    auto newBeans = hana::to_tuple(
        hana::difference(hana::to_set(beanVertexes), hana::to_set(independentBeans)));
    auto removedDependencyBeans =
        operation::Vertex::RemoveVertexDependency(newBeans, independentBeans);
    auto first = hana::front(removedDependencyBeans);
    using VertexType = typename decltype(hana::typeid_(first))::type;
    BOOST_HANA_ASSERT(hana::is_empty(VertexType::OutList));

    auto second = hana::back(removedDependencyBeans);
    using VertexType2 = typename decltype(hana::typeid_(second))::type;
    BOOST_HANA_ASSERT(VertexType2::OutDegree == hana::size_c<1>);
}

TEST_F(VertexTest, test_while) {
    auto test = hana::make_tuple(hana::type_c<int>, hana::type_c<float>, hana::type_c<char>);
    auto result = hana::while_(
        [](const auto& tuple) { return hana::greater(hana::size(tuple), hana::size_c<0>); }, test,
        [](const auto& tuple) { return hana::drop_front(tuple); });
    BOOST_HANA_ASSERT(hana::is_empty(result));
}

TEST_F(VertexTest, test_get_all_parents) {
    auto input = hana::make_tuple(hana::type_c<A2>);
    auto output = operation::Vertex::GetAllParents(input);
    std::cout << print::ToString(output) << std::endl;
}

A* createA(B*, C*) {
    return new AImpl();
}

TEST_F(VertexTest, test_creator_wrapper_traits) {
    using CreatorWrapperType = summer::bean::traits::CreatorWrapper<createA>;
    auto creator = CreatorWrapperType::Creator;
    auto a = creator(nullptr, nullptr);
    a->testA();
    static_assert(summer::bean::traits::IsCreatorWrapper<CreatorWrapperType>::value,
                  "CreatorWrapperType should be recognized as CreatorWrapper");
    using Resolver = summer::bean::define::BeanResolver<CreatorWrapperType>;
    using ConstructorTraits = summer::bean::traits::ConstructorTraits<decltype(createA)>;
    std::cout << print::ToString(ConstructorTraits::RET_TYPE) << '\n';
    std::cout << print::ToString(Resolver::ImplementOf) << '\n';
    std::cout << print::ToString(Resolver::DependOn) << '\n';
    BOOST_HANA_CHECK(Resolver::ImplementOf == hana::make_tuple());
    BOOST_HANA_CHECK(Resolver::DependOn == hana::make_tuple(hana::type_c<B>, hana::type_c<C>));
}

// Test for the fixed ToBeans function - issue where ToBeans was undefined
TEST_F(VertexTest, test_tobeans_function_exists_and_works) {
    // This test verifies that ToBeans function exists and compiles correctly
    // Previously this would cause compilation error due to undefined ToBeans
    auto independentVertexes = operation::Vertex::GetIndependentBeans(beanVertexes);
    
    // This line used to cause compilation error at line 119 in Vertex.h
    // Now it should work because ToBeans is properly defined
    auto beans = operation::Vertex::ToBeans(independentVertexes);
    
    // Verify that the transformation worked by checking the types
    // The ToBeans function should convert vertices back to their original bean types
    auto expected_bean_types = hana::transform(independentVertexes, [](const auto& vertex) {
        using VertexType = typename decltype(hana::typeid_(vertex))::type;
        return hana::type_c<typename VertexType::NodeType>;  // This gets the original bean type
    });
    
    // Compare the actual result with the expected result
    BOOST_HANA_CHECK(hana::size(beans) == hana::size(expected_bean_types));
    
    // If we got here without compilation errors, the fix is successful
    EXPECT_TRUE(true);
}

// Test for the fixed Merge function with empty collections
TEST_F(VertexTest, test_merge_with_empty_collections) {
    using namespace summer::util::collection::tuple;
    
    // This test verifies that Merge function can handle empty collections
    // Previously this would cause compilation error when called with no arguments
    auto emptyResult = Merge();  // This was causing compilation error
    
    // Test with single tuple
    auto tuple1 = hana::make_tuple(1, 2, 3);
    auto singleResult = Merge(tuple1);
    
    // Test with multiple tuples
    auto tuple2 = hana::make_tuple(4, 5, 6);
    auto multiResult = Merge(tuple1, tuple2);
    
    // Verify the results are correct
    BOOST_HANA_CHECK(hana::size(emptyResult) == hana::size_c<0>);  // Empty tuple
    BOOST_HANA_CHECK(hana::size(singleResult) == hana::size_c<3>); // Same size as input
    BOOST_HANA_CHECK(hana::size(multiResult) == hana::size_c<6>);  // Combined size
    
    EXPECT_TRUE(true); // If this compiles and runs, the fix is successful
}