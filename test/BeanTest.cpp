#include "BeanTest.h"
#include "Summer.h"
#include "TestBeanClass.h"

using namespace boost;
using namespace summer;

TEST_F(BeanTest, test_construct_beans_by_bean_factory) {
  using namespace NormalCase;
  using Factory = BeanFactory<CImpl, BImpl, AImpl>;
  auto a = Factory::GetShared<A>();
  auto b = Factory::GetShared<B>();
  auto c = Factory::GetShared<C>();
  auto a1 = Factory::GetShared<AImpl>();
  auto b1 = Factory::GetShared<BImpl>();
  auto c1 = Factory::GetShared<CImpl>();
  EXPECT_EQ(a.get(), a1.get());
  EXPECT_EQ(b.get(), b1.get());
  EXPECT_EQ(c.get(), c1.get());
}

// todo: not support reference in constructor
// TEST_F(BeanTest, test_construct_beans_with_ref_constructor) {
//   using namespace BeanWithRefConstructor;
//   using Factory = BeanFactory<CImpl, BImpl, AImpl>;
//   auto a = Factory::GetShared<A>();
//   auto b = Factory::GetShared<B>();
//   auto c = Factory::GetShared<C>();
//   auto a1 = Factory::GetShared<AImpl>();
//   auto b1 = Factory::GetShared<BImpl>();
//   auto c1 = Factory::GetShared<CImpl>();
//   EXPECT_EQ(a.get(), a1.get());
//   EXPECT_EQ(b.get(), b1.get());
//   EXPECT_EQ(c.get(), c1.get());
// }

TEST_F(BeanTest, test_construct_beans_with_unique_ptr) {
  using namespace FactoryCreateCase;
  using Factory = BeanFactory<CImpl, BImpl, AImpl, DImpl>;
  auto a = Factory::GetShared<A>();
  auto b = Factory::GetShared<B>();
  auto c = Factory::GetShared<C>();
  auto d = Factory::GetShared<D>();
  auto a1 = Factory::GetShared<AImpl>();
  auto b1 = Factory::GetShared<BImpl>();
  auto c1 = Factory::GetShared<CImpl>();
  auto d1 = Factory::GetShared<DImpl>();
  EXPECT_EQ(a.get(), a1.get());
  EXPECT_EQ(b.get(), b1.get());
  EXPECT_EQ(c.get(), c1.get());
  EXPECT_EQ(d.get(), d1.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_constructor) {
  using namespace BeansWithListConstructor;
  using Factory = BeanFactory<CImpl, BImpl, AImpl, B2Impl>;
  auto c = Factory::GetShared<C>();
  auto b = Factory::GetShared<BImpl>();
  auto b2 = Factory::GetShared<B2Impl>();
  auto c1 = Factory::GetShared<CImpl>();
  EXPECT_EQ(c.get(), c1.get());
  EXPECT_TRUE(c1->bs.front().get() == b.get());
  EXPECT_TRUE(c1->bs.back().get() == b2.get());
}

TEST_F(BeanTest, test_construct_beans_with_list_unique_ptr_constructor) {
  using namespace BeansWithListUniquePtrConstructor;
  using Factory = BeanFactory<CImpl, BImpl, AImpl, B2Impl>;
  auto c = Factory::GetShared<C>();
  auto b = Factory::GetShared<BImpl>();
  auto b2 = Factory::GetShared<B2Impl>();
  auto c1 = Factory::GetShared<CImpl>();
  EXPECT_EQ(c.get(), c1.get());
  // 由于结构体入参为std::list<std::unique_ptr<B>>，所以b和b2实例与Shared不一致
  EXPECT_FALSE(std::find_if(c1->bs.begin(), c1->bs.end(),
                            [&](const std::unique_ptr<B> &ptr) {
                              return ptr.get() == b.get();
                            }) != c1->bs.end());
  EXPECT_FALSE(std::find_if(c1->bs.begin(), c1->bs.end(),
                            [&](const std::unique_ptr<B> &ptr) {
                              return ptr.get() == b2.get();
                            }) != c1->bs.end());
}

TEST_F(BeanTest, test_construct_beans_with_raw_pointer_constructor) {
  using namespace BeansWithRawPointerConstructor;
  using Factory = BeanFactory<CImpl, BImpl, AImpl, B2Impl>;
  auto a = Factory::GetShared<AImpl>();
  auto c = Factory::GetShared<C>();
  auto b = Factory::GetShared<BImpl>();
  auto b2 = Factory::GetShared<B2Impl>();
  auto c1 = Factory::GetShared<CImpl>();
  EXPECT_EQ(c.get(), c1.get());
  EXPECT_TRUE(c1->bs.front() == b.get());
  EXPECT_TRUE(c1->bs.back() == b2.get());
  EXPECT_TRUE(c1->a == a.get());
}

// TEST_F(BeanTest, test_construct_beans_lack_of_impl) {
//   using namespace NormalCase;
//   using Factory = BeanFactory<CImpl, AImpl>;
//   auto a = Factory::GetBean<A>();
//   auto c = Factory::GetBean<C>();
//   auto a1 = Factory::GetBean<AImpl>();
//   auto c1 = Factory::GetBean<CImpl>();
//   EXPECT_EQ(a.get(), a1.get());
//   EXPECT_EQ(c.get(), c1.get());
// }