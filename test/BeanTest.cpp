#include "BeanTest.h"
#include "Summer.h"
#include "TestBeanClass.h"

using namespace boost;
using namespace summer;

TEST_F(BeanTest, test_construct_beans) {
  auto beans = hana::tuple_t<BImpl, AImpl, CImpl>;
  auto result = bean::factory::Beans::CreateFactory(beans);
  auto a = result[hana::type_c<A>];
  auto a1 = result[hana::type_c<AImpl>];
  auto ap0 = hana::at_c<0>(a);
  auto ap1 = hana::at_c<0>(a1);
  EXPECT_EQ(ap0.get(), ap1.get());
}

TEST_F(BeanTest, test_construct_beans_by_bean_factory) {
  using Factory = BeanFactory<CImpl, BImpl, AImpl>;
  auto a = Factory::GetBean<A>();
  auto b = Factory::GetBean<B>();
  auto c = Factory::GetBean<C>();
  auto a1 = Factory::GetBean<AImpl>();
  auto b1 = Factory::GetBean<BImpl>();
  auto c1 = Factory::GetBean<CImpl>();
  EXPECT_EQ(a.get(), a1.get());
  EXPECT_EQ(b.get(), b1.get());
  EXPECT_EQ(c.get(), c1.get());
  a->testA();
}