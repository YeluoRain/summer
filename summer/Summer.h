//
// Created by Zhongwen Wang on 2023/11/18.
//

#ifndef SUMMER_SUMMER
#define SUMMER_SUMMER

#include "summer/bean/Factory.h"

namespace summer {
namespace hana = boost::hana;

template <typename AllBeanContainer>
class BeanFactory {
  public:
    template <typename BeanType>
    std::shared_ptr<BeanType> GetShared() {
        // static_assert(hana::contains(container, hana::type_c<BeanType>),
        //               "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        assert(hana::value(hana::size(creators) == hana::size_c<1>) &&
               "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetShared();
    }

    template <typename BeanType>
    std::list<std::shared_ptr<BeanType>> GetSharedList() {
        // static_assert(hana::contains(container, hana::type_c<BeanType>),
        //               "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        std::list<std::shared_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetShared()); });
        return result;
    }

    template <typename BeanType>
    std::list<std::unique_ptr<BeanType>> GetUniqueList() {
        // static_assert(hana::contains(container, hana::type_c<BeanType>),
        //               "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        std::list<std::unique_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetUnique()); });
        return result;
    }

    template <typename BeanType>
    std::unique_ptr<BeanType> GetUnique() {
        // static_assert(hana::contains(container, hana::type_c<BeanType>),
        //               "BeanType not found in BeanFactory");
        auto creators = container[hana::type_c<BeanType>];
        assert(hana::value(hana::size(creators) == hana::size_c<1>) &&
               "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetUnique();
    }

  private:
    decltype(bean::factory::Beans::CreateFactory(AllBeanContainer{})) container =
        bean::factory::Beans::CreateFactory(AllBeanContainer{});
};

template <typename BeanContainer = decltype(hana::make_tuple()),
          typename CreatorContainer = decltype(hana::make_tuple())>
class ContainerBuilder {
  public:
    template <typename... BeanTypes>
    constexpr auto WithBeans() {
        return ContainerBuilder<decltype(hana::concat(beanContainer, hana::tuple_t<BeanTypes...>)),
                                CreatorContainer>{};
    }

    template <auto... Creators>
    constexpr auto WithCreators() {
        return ContainerBuilder<BeanContainer,
                                decltype(hana::concat(
                                    creatorContainer,
                                    hana::tuple_t<bean::traits::CreatorWrapper<Creators>...>))>{};
    }

    constexpr auto Build() {
        auto all = hana::concat(beanContainer, creatorContainer);
        return BeanFactory<decltype(all)>{};
    }

  private:
    static constexpr auto beanContainer = BeanContainer{};
    static constexpr auto creatorContainer = CreatorContainer{};
};

}  // namespace summer

#endif /* SUMMER_SUMMER */
