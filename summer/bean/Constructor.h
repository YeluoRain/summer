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

#ifndef SUMMER_BEAN_CONSTRUCTOR
#define SUMMER_BEAN_CONSTRUCTOR

#include <functional>
#include <list>
#include <memory>
#include <vector>

#include "boost/hana.hpp"

namespace summer::bean::constructor {
namespace hana = boost::hana;

template <typename BeanType>
class BeanCreator {
  public:
    explicit BeanCreator(std::function<BeanType*()> creator)
        : m_creator(creator), m_sharedInstance(nullptr) {}

    std::shared_ptr<BeanType> GetShared() {
        if (m_sharedInstance == nullptr) {
            m_sharedInstance = std::shared_ptr<BeanType>(m_creator());
            if (m_sharedInstance == nullptr) {
                throw std::runtime_error("Failed to create shared instance.");
            }
        }
        return m_sharedInstance;
    }

    std::unique_ptr<BeanType> GetUnique() {
        auto* rawPointer = m_creator();
        if (rawPointer == nullptr) {
            throw std::runtime_error("Failed to create unique instance.");
        }
        return std::unique_ptr<BeanType>(rawPointer);
    }

    void Reset() {
        m_sharedInstance.reset();
    }

  private:
    std::function<BeanType*()> m_creator;
    std::shared_ptr<BeanType> m_sharedInstance;
};

template <typename ArgType>
struct SingletonInvokerHelper {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr std::shared_ptr<ArgType> Invoke(BeanResolvers&& resolvers,
                                                     CreatorMap&& creatorMap) {
        assert(hana::size(resolvers) == hana::size_c<1> &&
               "Multiple resolvers found for single instance.");
        auto resolver = resolvers[hana::size_c<0>];
        auto creator = creatorMap[resolver];
        return creator->GetShared();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr ArgType Invoke(BeanResolvers&& resolvers, CreatorMap&& creatorMap) {
        return *SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanResolvers>(resolvers),
                                                        std::forward<CreatorMap>(creatorMap));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<const ArgType> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr ArgType Invoke(BeanResolvers&& resolvers, CreatorMap&& creatorMap) {
        return BeanCreatorInvoker<ArgType>::Invoke(std::forward<BeanResolvers>(resolvers),
                                                   std::forward<CreatorMap>(creatorMap));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<ArgType&> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr ArgType Invoke(BeanResolvers&& resolvers, CreatorMap&& creatorMap) {
        return BeanCreatorInvoker<ArgType>::Invoke(std::forward<BeanResolvers>(resolvers),
                                                   std::forward<CreatorMap>(creatorMap));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<ArgType*> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr ArgType* Invoke(BeanResolvers&& resolvers, CreatorMap&& creatorMap) {
        return SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanResolvers>(resolvers),
                                                       std::forward<CreatorMap>(creatorMap))
            .get();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::shared_ptr<ArgType>> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr std::shared_ptr<ArgType> Invoke(BeanResolvers&& resolvers,
                                                     CreatorMap&& creatorMap) {
        return SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanResolvers>(resolvers),
                                                       std::forward<CreatorMap>(creatorMap));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::unique_ptr<ArgType>> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr std::unique_ptr<ArgType> Invoke(BeanResolvers&& resolvers,
                                                     CreatorMap&& creatorMap) {
        assert(hana::size(resolvers) == hana::size_c<1> &&
               "Multiple resolvers found for single instance.");
        auto resolver = resolvers[hana::size_c<0>];
        auto creator = creatorMap[resolver];
        return creator->GetUnique();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::list<ArgType>> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr std::list<ArgType> Invoke(BeanResolvers&& resolvers, CreatorMap&& creatorMap) {
        std::list<ArgType> result;
        hana::for_each(std::forward<BeanResolvers>(resolvers), [&](auto&& resolver) {
            auto instance = BeanCreatorInvoker<ArgType>::Invoke(
                hana::make_tuple(resolver), std::forward<CreatorMap>(creatorMap));
            result.push_back(std::move(instance));
        });
        return result;
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::vector<ArgType>> {
    template <typename BeanResolvers, typename CreatorMap>
    static constexpr std::vector<ArgType> Invoke(BeanResolvers&& resolvers,
                                                 CreatorMap&& creatorMap) {
        std::vector<ArgType> result;
        hana::for_each(std::forward<BeanResolvers>(resolvers), [&](auto&& resolver) {
            auto instance = BeanCreatorInvoker<ArgType>::Invoke(
                hana::make_tuple(resolver), std::forward<CreatorMap>(creatorMap));
            result.push_back(std::move(instance));
        });
        return result;
    }
};
}  // namespace summer::bean::constructor

#endif /* SUMMER_BEAN_CONSTRUCTOR */
