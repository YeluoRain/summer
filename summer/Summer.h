//
// Created by Zhongwen Wang on 2023/11/18.
//

#ifndef SUMMER_SUMMER
#define SUMMER_SUMMER

#include "bean/Define.h"
#include "bean/Factory.h"
#include "bean/Traits.h"
#include "dag/Graph.h"
#include "dag/Vertex.h"
#include "util/Collection.h"
#include "util/Print.h"

namespace summer {
namespace hana = boost::hana;
template <typename... ArgTypes> class BeanFactory {
public:
  template <typename BeanType> static auto GetBean() {
    static_assert(hana::contains(container, hana::type_c<BeanType>),
                  "BeanType not found in BeanFactory");
    return container[hana::type_c<BeanType>][hana::size_c<0>];
  }

private:
  using ContainerType = decltype(
      bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>));
  static auto GetBeanContainer() {
    return bean::factory::Beans::CreateFactory(
        boost::hana::tuple_t<ArgTypes...>);
  }

  static ContainerType container;
};

template <typename... ArgTypes>
decltype(bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>))
    BeanFactory<ArgTypes...>::container =
        bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>);

} // namespace summer

#endif /* SUMMER_SUMMER */
