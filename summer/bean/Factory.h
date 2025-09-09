#ifndef SUMMER_BEAN_FACTORY
#define SUMMER_BEAN_FACTORY

#include "bean/Define.h"
#include "dag/Graph.h"
#include "dag/Operation.h"

namespace summer::bean::factory {
struct Beans {
  static constexpr auto Inject = [](auto &&beans) {
    using namespace boost;
    auto beanVertexs = hana::transform(beans, [](auto &bean) {
      using BeanType = typename decltype(hana::typeid_(bean))::type;
      auto beanVertex =
          hana::type_c<dag::graph::Vertex<define::BeanResolver<BeanType>>>;
      return beanVertex;
    });
    auto context = dag::operation::GenerateBeanResolverContext(beanVertexs);
    return context;
  };
};
} // namespace summer::bean::factory

#endif /* SUMMER_BEAN_FACTORY */
