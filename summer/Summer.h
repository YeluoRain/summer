//
// Created by Zhongwen Wang on 2023/11/18.
//

#ifndef SUMMER_SUMMER
#define SUMMER_SUMMER

#include "summer/bean/Factory.h"

namespace summer {
namespace hana = boost::hana;

/**
 * @brief BeanFactory is used to access beans created by bean creators.
 *
 * @tparam AllBeanContainer A Boost.Hana tuple containing all bean creators and bean types.
 */
template <typename AllBeanContainer>
class BeanFactory {
  public:
    /**
     * @brief Get the Shared object from the factory.
     *
     * @tparam BeanType The type of the bean to retrieve.
     * @return std::shared_ptr<BeanType> The shared pointer to the bean instance.
     */
    template <typename BeanType>
    std::shared_ptr<BeanType> GetShared() {
        auto creators = FindCreators<BeanType>();
        static_assert(hana::size(creators) == hana::size_c<1>,
                      "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetShared();
    }

    /**
     * @brief Get the Unique object from the factory.
     *
     * @tparam BeanType The type of the bean to retrieve.
     * @return std::unique_ptr<BeanType> The unique pointer to the bean instance.
     */
    template <typename BeanType>
    std::unique_ptr<BeanType> GetUnique() {
        auto creators = FindCreators<BeanType>();
        static_assert(hana::size(creators) == hana::size_c<1>,
                      "There should be only one creator for a bean type.");
        return creators[hana::size_c<0>].GetUnique();
    }

    /**
     * @brief Get the Shared List object from the factory.
     *
     * @tparam BeanType The type of the bean to retrieve.
     * @return std::list<std::shared_ptr<BeanType>> The list of shared pointers to the bean
     * instances.
     */
    template <typename BeanType>
    std::list<std::shared_ptr<BeanType>> GetSharedList() {
        auto creators = FindCreators<BeanType>();
        std::list<std::shared_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetShared()); });
        return result;
    }

    /**
     * @brief Get the Unique List object from the factory.
     *
     * @tparam BeanType The type of the bean to retrieve.
     * @return std::list<std::unique_ptr<BeanType>> The list of unique pointers to the bean
     * instances.
     */
    template <typename BeanType>
    std::list<std::unique_ptr<BeanType>> GetUniqueList() {
        auto creators = FindCreators<BeanType>();
        std::list<std::unique_ptr<BeanType>> result;
        hana::for_each(creators, [&](auto creator) { result.push_back(creator.GetUnique()); });
        return result;
    }

  private:
    template <typename BeanType>
    auto FindCreators() {
        static_assert(hana::contains(container, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        return container[hana::type_c<BeanType>];
    }

    inline static decltype(bean::factory::Beans::CreateFactory(AllBeanContainer{})) container =
        bean::factory::Beans::CreateFactory(AllBeanContainer{});
};

/**
 * @brief Bean Factory Builder
 *
 * @tparam BeanContainer Container of bean types
 * @tparam CreatorContainer Container of bean creators
 */
template <typename BeanContainer = decltype(hana::make_tuple()),
          typename CreatorContainer = decltype(hana::make_tuple())>
class FactoryBuilder {
  public:
    /**
     * @brief Include bean types into the container
     *
     * @tparam BeanTypes Types of beans to include
     * @return constexpr auto New FactoryBuilder with added bean types
     */
    template <typename... BeanTypes>
    constexpr auto WithBeans() {
        return FactoryBuilder<decltype(hana::concat(beanContainer, hana::tuple_t<BeanTypes...>)),
                              CreatorContainer>{};
    }

    /**
     * @brief Include bean creators into the container
     *
     * @tparam Creators Types of bean creators to include
     * @return constexpr auto New FactoryBuilder with added bean creators
     */
    template <auto... Creators>
    constexpr auto WithCreators() {
        return FactoryBuilder<BeanContainer,
                              decltype(hana::concat(
                                  creatorContainer,
                                  hana::tuple_t<bean::traits::CreatorWrapper<Creators>...>))>{};
    }

    /**
     * @brief Build the BeanFactory
     *
     * @return constexpr auto BeanFactory instance
     */
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
