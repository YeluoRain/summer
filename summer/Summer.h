/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        auto resolvers = FindResolvers<BeanType>();
        static_assert(hana::size(resolvers) == hana::size_c<1>,
                      "There should be only one resolver for a bean type.");
        auto creator = FindCreator(resolvers[hana::size_c<0>]);
        return creator->GetShared();
    }

    /**
     * @brief Get the Unique object from the factory.
     *
     * @tparam BeanType The type of the bean to retrieve.
     * @return std::unique_ptr<BeanType> The unique pointer to the bean instance.
     */
    template <typename BeanType>
    std::unique_ptr<BeanType> GetUnique() {
        auto resolvers = FindResolvers<BeanType>();
        static_assert(hana::size(resolvers) == hana::size_c<1>,
                      "There should be only one resolver for a bean type.");
        auto creator = FindCreator(resolvers[hana::size_c<0>]);
        return creator->GetUnique();
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
        auto resolvers = FindResolvers<BeanType>();
        std::list<std::shared_ptr<BeanType>> result;
        hana::for_each(resolvers, [&](auto&& resolver) {
            auto creator = FindCreator(resolver);
            result.push_back(creator->GetShared());
        });
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
        auto resolvers = FindResolvers<BeanType>();
        std::list<std::unique_ptr<BeanType>> result;
        hana::for_each(resolvers, [&](auto&& resolver) {
            auto creator = FindCreator(resolver);
            result.push_back(creator->GetUnique());
        });
        return result;
    }

    /**
     * @brief Reset all shared instances in the factory.
     *
     * This method will reset all shared instances created by the factory.
     * The shared instances will be recreated next time they are requested.
     */
    void Reset() {
        hana::for_each(m_container[hana::size_c<1>], [&](auto&& creator) { creator->Reset(); });
    }

  private:
    template <typename BeanType>
    auto FindResolvers() {
        auto resolverMap = m_container[hana::size_c<0>];
        static_assert(hana::contains(resolverMap, hana::type_c<BeanType>),
                      "BeanType not found in BeanFactory");
        return resolverMap[hana::type_c<BeanType>];
    }

    template <typename ResolverType>
    auto FindCreator(ResolverType&& resolver) {
        auto creatorMap = m_container[hana::size_c<1>];
        return creatorMap[resolver];
    }

    decltype(bean::factory::Beans::CreateFactory(AllBeanContainer{})) m_container =
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
