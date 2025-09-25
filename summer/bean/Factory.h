#ifndef SUMMER_BEAN_FACTORY
#define SUMMER_BEAN_FACTORY

#include "bean/Define.h"
#include "dag/Graph.h"
#include "dag/Vertex.h"

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
    auto context =
        dag::operation::Vertex::GenerateBeanResolverContext(beanVertexs);
    return context;
  };

  static constexpr auto ToBean = [](auto &&vertex) {
    using namespace boost;
    using VertexType = typename decltype(hana::typeid_(vertex))::type;
    return hana::type_c<typename VertexType::BeanType>;
  };

  static constexpr auto ToBeans = [](auto &&vertexes) {
    return boost::hana::transform(vertexes, ToBean);
  };

  static constexpr auto ConstructBeans = [](auto &&bean) {
    using namespace boost;
    auto context = dag::operation::Vertex::GenerateBeanResolverContext(
        dag::operation::Vertex::ToVertexes(bean));
    auto independentBeans =
        hana::transform(hana::at(context, hana::int_c<0>), ToBean);
    auto instanceMap = hana::make_map();
    return hana::fold_left(
        independentBeans, instanceMap, [](auto &&instanceMap, auto &&bean) {
          using BeanType = typename decltype(hana::typeid_(bean))::type;
          using BeanVertexType =
              dag::graph::Vertex<define::BeanResolver<BeanType>>;
          auto parents =
              dag::operation::Vertex::GetAllParents(hana::make_tuple(bean));
          auto args = hana::transform(
              BeanVertexType::OutList, [&instanceMap](auto &&bean) {
                return hana::eval_if(
                    hana::size(instanceMap) == hana::size_c<0>,
                    [](auto &&) { return hana::make_tuple(); },
                    [&instanceMap, &bean](auto &&) {
                      return instanceMap[bean];
                    });
              });
          auto instance = hana::unpack(args, [](auto &&...args) {
            return std::make_shared<BeanType>(
                std::forward<decltype(args)>(args)...);
          });
          auto map = hana::unpack(
              parents, hana::make_map ^ hana::on ^ [&instance](auto &&parent) {
                return hana::make_pair(parent, instance);
              });
          return hana::union_(instanceMap, map);
        });
  };
};
} // namespace summer::bean::factory

#endif /* SUMMER_BEAN_FACTORY */
