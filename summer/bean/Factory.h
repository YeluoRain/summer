#ifndef SUMMER_BEAN_FACTORY
#define SUMMER_BEAN_FACTORY

#include "bean/Constructor.h"
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
    return dag::operation::Vertex::GetBeansInOrder(beanVertexs);
  };

  static constexpr auto ToBean = [](auto &&vertex) {
    using namespace boost;
    using VertexType = typename decltype(hana::typeid_(vertex))::type;
    return hana::type_c<typename VertexType::BeanType>;
  };

  static constexpr auto ToBeans = [](auto &&vertexes) {
    return boost::hana::transform(vertexes, ToBean);
  };

  static constexpr auto MergeBeanMap = [](auto &&beanMap, auto &&parents,
                                          auto &&instance) {
    using namespace boost;
    return hana::fold_left(
        parents, beanMap, [&](auto &&curBeanMap, auto &&parent) {
          auto instances =
              hana::find(curBeanMap, parent).value_or(hana::make_tuple());
          auto pair =
              hana::make_pair(parent, hana::append(instances, instance));
          auto newMap = hana::erase_key(curBeanMap, parent);
          return hana::insert(newMap, pair);
        });
  };

  static constexpr auto CreateFactory = [](auto &&beans) {
    using namespace boost;
    auto independentVertexes = dag::operation::Vertex::GetBeansInOrder(
        dag::operation::Vertex::ToVertexes(beans));
    auto independentBeans = hana::transform(independentVertexes, ToBean);
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
                      return instanceMap[bean][hana::size_c<0>];
                    });
              });
          auto instance = hana::unpack(args, [](auto &&...args) {
            return std::make_shared<BeanType>(
                std::forward<decltype(args)>(args)...);
          });
          return MergeBeanMap(instanceMap, parents, instance);
        });
  };

  static constexpr auto CreateFactory0 = [](auto &&beans) {
    using namespace boost;
    auto independentVertexes = dag::operation::Vertex::GetBeansInOrder(
        dag::operation::Vertex::ToVertexes(beans));
    auto independentBeans = hana::transform(independentVertexes, ToBean);
    auto beanCreatorMap = hana::make_map();
    return hana::fold_left(
        independentBeans, beanCreatorMap, [](auto &&creatorMap, auto &&bean) {
          using BeanType = typename decltype(hana::typeid_(bean))::type;
          using BeanVertexType =
              dag::graph::Vertex<define::BeanResolver<BeanType>>;
          auto parents =
              dag::operation::Vertex::GetAllParents(hana::make_tuple(bean));
          auto creator = [creatorMap] {
            auto argTypes = traits::ConstructorTraits<decltype(
                &BeanType::_InjectedFactory)>::ARG_TYPES;
            auto args = hana::transform(argTypes, [&creatorMap](auto &&bean) {
              using ArgType = typename decltype(hana::typeid_(bean))::type;
              using RawArgType = typename traits::ArgTypeTraits<ArgType>::type;
              auto creators = hana::find(creatorMap, hana::type_c<RawArgType>)
                                  .value_or(hana::make_tuple());
              return constructor::BeanCreatorInvoker<ArgType>::Invoke(creators);
            });
            return hana::unpack(args, [](auto &&...args) {
              return new BeanType(std::forward<decltype(args)>(args)...);
            });
          };
          return MergeBeanMap(creatorMap, parents,
                              constructor::BeanCreator<BeanType>{creator});
        });
  };
};
} // namespace summer::bean::factory

#endif /* SUMMER_BEAN_FACTORY */
