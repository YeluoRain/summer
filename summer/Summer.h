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
template <typename... ArgTypes>
class BeanFactory {
  public:
    template <typename BeanType>
    static std::shared_ptr<BeanType> GetShared() {
        static_assert(hana::contains(container, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        assert(hana::value(hana::size(creators) == hana::size_c<1>) &&
               "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetShared();
    }

    template <typename BeanType>
    static std::list<std::shared_ptr<BeanType>> GetSharedList() {
        static_assert(hana::contains(container, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        std::list<std::shared_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetShared()); });
        return result;
    }

    template <typename BeanType>
    static std::list<std::unique_ptr<BeanType>> GetUniqueList() {
        static_assert(hana::contains(container, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        std::list<std::unique_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetUnique()); });
        return result;
    }

    template <typename BeanType>
    static std::unique_ptr<BeanType> GetUnique() {
        static_assert(hana::contains(container, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        assert(hana::value(hana::size(creators) == hana::size_c<1>) &&
               "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetUnique();
    }

  private:
    using ContainerType =
        decltype(bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>));
    static auto GetBeanContainer() {
        return bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>);
    }

    static ContainerType container;
};

template <typename... ArgTypes>
decltype(bean::factory::Beans::CreateFactory(
    boost::hana::tuple_t<ArgTypes...>)) BeanFactory<ArgTypes...>::container =
    bean::factory::Beans::CreateFactory(boost::hana::tuple_t<ArgTypes...>);

}  // namespace summer

#endif /* SUMMER_SUMMER */
