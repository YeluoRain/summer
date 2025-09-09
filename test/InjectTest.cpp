#include "InjectTest.h"
#include "Summer.h"
#include "TestBeanClass.h"

using namespace boost;
using namespace summer;

TEST_F(InjectTest, test_inject_and_arrange_construct_sequence) {
  auto beans = hana::tuple_t<BImpl, AImpl, CImpl>;
  auto result = bean::factory::Beans::Inject(beans);
  auto independentBeans = hana::at(result, hana::int_c<0>);
  auto intf2ImplMap = hana::at(result, hana::int_c<1>);
  auto r = hana::transform(independentBeans, [](auto &vertex) {
    using VertexType = typename decltype(hana::typeid_(vertex))::type;
    return hana::type_c<typename VertexType::BeanType>;
  });
  std::cout << util::print::ToString(r) << std::endl;
}