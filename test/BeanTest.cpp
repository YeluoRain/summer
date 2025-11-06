#include "BeanTest.h"

#include "TestBeanClass.h"
#include "summer/Summer.h"

using namespace boost;
using namespace summer;

TEST_F(BeanTest, test_construct_beans_by_bean_factory) {
    using namespace NormalCase;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    auto a1 = container.GetShared<AImpl>();
    auto b1 = container.GetShared<BImpl>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_EQ(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_with_creator_function) {
    using namespace NormalCase;
    auto container = ContainerBuilder<>().WithBeans<BImpl, AImpl>().WithCreators<createC>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    auto a1 = container.GetShared<AImpl>();
    auto b1 = container.GetShared<BImpl>();
    // auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_all_beans_with_creator_function) {
    using namespace NormalCase;
    auto container = ContainerBuilder<>().WithCreators<createC, createA, createB>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    // auto a1 = container.GetShared<AImpl>();
    // auto b1 = container.GetShared<BImpl>();
    // auto c1 = container.GetShared<CImpl>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_all_beans_with_creator_function_mixed_with_interface_and_impl) {
    using namespace NormalCase;
    auto container = ContainerBuilder<>().WithCreators<createCImpl, createA, createB>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_EQ(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_unique_ne_shared) {
    using namespace NormalCase;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    auto a1 = container.GetUnique<A>();
    auto b1 = container.GetUnique<B>();
    auto c1 = container.GetUnique<C>();
    EXPECT_NE(a.get(), a1.get());
    EXPECT_NE(b.get(), b1.get());
    EXPECT_NE(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_with_unique_ptr) {
    using namespace FactoryCreateCase;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl, DImpl>().Build();
    auto a = container.GetShared<A>();
    auto b = container.GetShared<B>();
    auto c = container.GetShared<C>();
    auto d = container.GetShared<D>();
    auto a1 = container.GetShared<AImpl>();
    auto b1 = container.GetShared<BImpl>();
    auto c1 = container.GetShared<CImpl>();
    auto d1 = container.GetShared<DImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_EQ(d.get(), d1.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_constructor) {
    using namespace BeansWithListConstructor;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto c = container.GetShared<C>();
    auto b = container.GetShared<BImpl>();
    auto b2 = container.GetShared<B2Impl>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_TRUE(c1->bs.front().get() == b.get());
    EXPECT_TRUE(c1->bs.back().get() == b2.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_constructor_using_getList) {
    using namespace BeansWithListConstructor;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto bs = container.GetSharedList<B>();
    auto bs1 = container.GetUniqueList<B>();
    EXPECT_EQ(2, bs.size());
    EXPECT_EQ(2, bs1.size());
}

TEST_F(BeanTest, test_construct_beans_with_list_unique_ptr_constructor) {
    using namespace BeansWithListUniquePtrConstructor;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto c = container.GetShared<C>();
    auto b = container.GetShared<BImpl>();
    auto b2 = container.GetShared<B2Impl>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_FALSE(std::find_if(c1->bs.begin(), c1->bs.end(), [&](const std::unique_ptr<B>& ptr) {
                     return ptr.get() == b.get();
                 }) != c1->bs.end());
    EXPECT_FALSE(std::find_if(c1->bs.begin(), c1->bs.end(), [&](const std::unique_ptr<B>& ptr) {
                     return ptr.get() == b2.get();
                 }) != c1->bs.end());
}

TEST_F(BeanTest, test_construct_beans_with_raw_pointer_constructor) {
    using namespace BeansWithRawPointerConstructor;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto a = container.GetShared<AImpl>();
    auto c = container.GetShared<C>();
    auto b = container.GetShared<BImpl>();
    auto b2 = container.GetShared<B2Impl>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_TRUE(c1->bs.front() == b.get());
    EXPECT_TRUE(c1->bs.back() == b2.get());
    EXPECT_TRUE(c1->a == a.get());
}

TEST_F(BeanTest, test_construct_beans_with_unique_but_has_nested_shared_ptr_constructor) {
    using namespace BeansWithUniqueButHasSharedPtrConstructor;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = container.GetShared<AImpl>();
    auto c = container.GetShared<C>();
    auto b = container.GetShared<BImpl>();
    auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_NE(b.get(), c1->b.get());
}

TEST_F(BeanTest, test_construct_beans_without_interface_heritated) {
    using namespace BeansWithoutInterfaceHeritated;
    auto container = ContainerBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = container.GetShared<AImpl>();
    auto b = container.GetShared<BImpl>();
    auto c = container.GetShared<CImpl>();
    EXPECT_NE(b.get(), c->b.get());
    EXPECT_EQ(a.get(), b->a.get());
}