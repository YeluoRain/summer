#include "BeanTest.h"

#include "TestBeanClass.h"
#include "summer/Summer.h"

using namespace boost;
using namespace summer;

TEST_F(BeanTest, test_construct_beans_by_bean_factory) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto a1 = factory.GetShared<AImpl>();
    auto b1 = factory.GetShared<BImpl>();
    auto c1 = factory.GetShared<CImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_EQ(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_with_creator_function) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>().WithBeans<BImpl, AImpl>().WithCreators<createC>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto a1 = factory.GetShared<AImpl>();
    auto b1 = factory.GetShared<BImpl>();
    // auto c1 = container.GetShared<CImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_all_beans_with_creator_function) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>().WithCreators<createC, createA, createB>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    // auto a1 = container.GetShared<AImpl>();
    // auto b1 = container.GetShared<BImpl>();
    // auto c1 = container.GetShared<CImpl>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_all_beans_with_creator_function_mixed_with_interface_and_impl) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>().WithCreators<createCImpl, createA, createB>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto c1 = factory.GetShared<CImpl>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_EQ(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_unique_ne_shared) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto a1 = factory.GetUnique<A>();
    auto b1 = factory.GetUnique<B>();
    auto c1 = factory.GetUnique<C>();
    EXPECT_NE(a.get(), a1.get());
    EXPECT_NE(b.get(), b1.get());
    EXPECT_NE(c.get(), c1.get());
}

TEST_F(BeanTest, test_construct_beans_with_unique_ptr) {
    using namespace FactoryCreateCase;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl, DImpl>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto d = factory.GetShared<D>();
    auto a1 = factory.GetShared<AImpl>();
    auto b1 = factory.GetShared<BImpl>();
    auto c1 = factory.GetShared<CImpl>();
    auto d1 = factory.GetShared<DImpl>();
    EXPECT_EQ(a.get(), a1.get());
    EXPECT_EQ(b.get(), b1.get());
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_EQ(d.get(), d1.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_constructor) {
    using namespace BeansWithListConstructor;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto c = factory.GetShared<C>();
    auto b = factory.GetShared<BImpl>();
    auto b2 = factory.GetShared<B2Impl>();
    auto c1 = factory.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_TRUE(c1->_bs.front().get() == b.get());
    EXPECT_TRUE(c1->_bs.back().get() == b2.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_constructor_using_getList) {
    using namespace BeansWithListConstructor;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto bs = factory.GetSharedList<B>();
    auto bs1 = factory.GetUniqueList<B>();
    EXPECT_EQ(2, bs.size());
    EXPECT_EQ(2, bs1.size());
}

TEST_F(BeanTest, test_construct_beans_with_list_unique_ptr_constructor) {
    using namespace BeansWithListUniquePtrConstructor;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl, B2Impl>().Build();
    auto c = factory.GetShared<C>();
    auto b = factory.GetShared<BImpl>();
    auto b2 = factory.GetShared<B2Impl>();
    auto c1 = factory.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_FALSE(std::find_if(c1->_bs.begin(), c1->_bs.end(), [&](const std::unique_ptr<B>& ptr) {
                     return ptr.get() == b.get();
                 }) != c1->_bs.end());
    EXPECT_FALSE(std::find_if(c1->_bs.begin(), c1->_bs.end(), [&](const std::unique_ptr<B>& ptr) {
                     return ptr.get() == b2.get();
                 }) != c1->_bs.end());
}

TEST_F(BeanTest, test_construct_beans_with_unique_but_has_nested_shared_ptr_constructor) {
    using namespace BeansWithUniqueButHasSharedPtrConstructor;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = factory.GetShared<AImpl>();
    auto c = factory.GetShared<C>();
    auto b = factory.GetShared<BImpl>();
    auto c1 = factory.GetShared<CImpl>();
    EXPECT_EQ(c.get(), c1.get());
    EXPECT_NE(b.get(), c1->_b.get());
}

TEST_F(BeanTest, test_construct_beans_without_interface_heritated) {
    using namespace BeansWithoutInterfaceHeritated;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
    auto a = factory.GetShared<AImpl>();
    auto b = factory.GetShared<BImpl>();
    auto c = factory.GetShared<CImpl>();
    EXPECT_NE(b.get(), c->_b.get());
    EXPECT_EQ(a.get(), b->_a.get());
}

TEST_F(BeanTest, test_construct_beans_with_no_macro) {
    using namespace BeanWithNoMacro;
    auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, A>().Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<BImpl>();
    auto c = factory.GetShared<CImpl>();
    EXPECT_NE(a, nullptr);
    EXPECT_NE(b, nullptr);
    EXPECT_NE(c, nullptr);
}

TEST_F(BeanTest, test_bean_factory_lifecycle_and_destruction_order) {
    using namespace LifecycleTest;
    g_destructorOrder.clear();
    {
        auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
        auto a = factory.GetShared<A>();
        auto b = factory.GetShared<B>();
        auto c = factory.GetShared<C>();
        EXPECT_NE(a, nullptr);
        EXPECT_NE(b, nullptr);
        EXPECT_NE(c, nullptr);
    }
    ASSERT_EQ(g_destructorOrder.size(), 6);
    EXPECT_EQ(g_destructorOrder[0], "CImpl");
    EXPECT_EQ(g_destructorOrder[1], "C");
    EXPECT_EQ(g_destructorOrder[2], "BImpl");
    EXPECT_EQ(g_destructorOrder[3], "B");
    EXPECT_EQ(g_destructorOrder[4], "AImpl");
    EXPECT_EQ(g_destructorOrder[5], "A");
}

TEST_F(BeanTest, test_bean_factory_lifecycle_shared_ptr_released) {
    using namespace LifecycleTest;
    g_destructorOrder.clear();
    {
        auto factory = FactoryBuilder<>().WithBeans<CImpl, BImpl, AImpl>().Build();
        auto a = factory.GetShared<A>();
        auto b = factory.GetShared<B>();
        auto c = factory.GetShared<C>();
        EXPECT_NE(a, nullptr);
        EXPECT_NE(b, nullptr);
        EXPECT_NE(c, nullptr);
        factory.Reset();
    }
    ASSERT_EQ(g_destructorOrder.size(), 6);
    EXPECT_EQ(g_destructorOrder[0], "CImpl");
    EXPECT_EQ(g_destructorOrder[1], "C");
    EXPECT_EQ(g_destructorOrder[2], "BImpl");
    EXPECT_EQ(g_destructorOrder[3], "B");
    EXPECT_EQ(g_destructorOrder[4], "AImpl");
    EXPECT_EQ(g_destructorOrder[5], "A");
}

TEST_F(BeanTest, test_complex_dag_with_8_beans) {
    using namespace ComplexDagCase;
    auto factory = FactoryBuilder<>()
                       .WithBeans<HImpl, FImpl, GImpl, EImpl, DImpl, BImpl, CImpl, AImpl>()
                       .Build();

    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    auto d = factory.GetShared<D>();
    auto e = factory.GetShared<E>();
    auto f = factory.GetShared<F>();
    auto g = factory.GetShared<G>();
    auto h = factory.GetShared<H>();
    auto hImpl = factory.GetShared<HImpl>();

    EXPECT_NE(a, nullptr);
    EXPECT_NE(b, nullptr);
    EXPECT_NE(c, nullptr);
    EXPECT_NE(d, nullptr);
    EXPECT_NE(e, nullptr);
    EXPECT_NE(f, nullptr);
    EXPECT_NE(g, nullptr);
    EXPECT_NE(h, nullptr);
    EXPECT_NE(hImpl, nullptr);
    EXPECT_EQ(h.get(), hImpl.get());

    auto aList = factory.GetSharedList<A>();
    EXPECT_EQ(aList.size(), 1);
    EXPECT_EQ(aList.front().get(), a.get());
}

TEST_F(BeanTest, test_construct_beans_with_creator_returning_shared_ptr) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>()
                       .WithCreators<createAShared, createBShared, createCShared>()
                       .Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_beans_with_creator_returning_unique_ptr) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>()
                       .WithCreators<createAUnique, createBUnique>()
                       .WithBeans<CImpl>()
                       .Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_beans_mixed_raw_and_smart_ptr_factory) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>()
                       .WithCreators<createA, createBShared, createCImpl>()
                       .Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
}

TEST_F(BeanTest, test_construct_beans_with_shared_ptr_and_unique_ptr_creator) {
    using namespace NormalCase;
    auto factory = FactoryBuilder<>()
                       .WithBeans<AImpl>()
                       .WithCreators<createBShared, createCShared>()
                       .Build();
    auto a = factory.GetShared<A>();
    auto b = factory.GetShared<B>();
    auto c = factory.GetShared<C>();
    EXPECT_NE(a.get(), nullptr);
    EXPECT_NE(b.get(), nullptr);
    EXPECT_NE(c.get(), nullptr);
    EXPECT_EQ(a.get(), factory.GetShared<AImpl>().get());
}