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
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "boost/hana.hpp"
#include "summer/util/Print.h"

namespace summer::bean::constructor {
namespace hana = boost::hana;

template <typename BeanType>
class BeanCreator {
  public:
    explicit BeanCreator(std::function<BeanType*()> creator) : m_creator(creator) {}

    std::shared_ptr<BeanType> GetShared() {
        if (m_sharedInstance == nullptr) {
            std::cout << "start to create shared " << boost::core::demangle(typeid(BeanType).name())
                      << std::endl;
            m_sharedInstance = std::shared_ptr<BeanType>(m_creator());
        }
        return m_sharedInstance;
    }

    std::unique_ptr<BeanType> GetUnique() {
        std::cout << "start to create unique " << boost::core::demangle(typeid(BeanType).name())
                  << std::endl;
        return std::unique_ptr<BeanType>(m_creator());
    }

  private:
    std::function<BeanType*()> m_creator;

    static std::shared_ptr<BeanType> m_sharedInstance;
};

template <typename BeanType>
std::shared_ptr<BeanType> BeanCreator<BeanType>::m_sharedInstance = nullptr;

template <typename ArgType>
struct SingletonInvokerHelper {
    template <typename BeanCreators>
    static constexpr std::shared_ptr<ArgType> Invoke(const BeanCreators& creators) {
        assert(hana::size(creators) == hana::size_c<1> &&
               "Multiple creators found for single instance.");
        auto creator = creators[hana::size_c<0>];
        return creator.GetShared();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker {
    template <typename BeanCreators>
    static constexpr ArgType Invoke(BeanCreators&& creators) {
        return *SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanCreators>(creators));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<const ArgType> {
    template <typename BeanCreators>
    static constexpr ArgType Invoke(BeanCreators&& creators) {
        return BeanCreatorInvoker<ArgType>::Invoke(std::forward<BeanCreators>(creators));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<ArgType&> {
    template <typename BeanCreators>
    static constexpr ArgType Invoke(BeanCreators&& creators) {
        return BeanCreatorInvoker<ArgType>::Invoke(std::forward<BeanCreators>(creators));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<ArgType*> {
    template <typename BeanCreators>
    static constexpr ArgType* Invoke(BeanCreators&& creators) {
        return SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanCreators>(creators)).get();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::shared_ptr<ArgType>> {
    template <typename BeanCreators>
    static constexpr std::shared_ptr<ArgType> Invoke(BeanCreators&& creators) {
        return SingletonInvokerHelper<ArgType>::Invoke(std::forward<BeanCreators>(creators));
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::unique_ptr<ArgType>> {
    template <typename BeanCreators>
    static constexpr std::unique_ptr<ArgType> Invoke(const BeanCreators& creators) {
        assert(hana::size(creators) == hana::size_c<1> &&
               "Multiple creators found for single instance.");
        auto creator = creators[hana::size_c<0>];
        return creator.GetUnique();
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::list<ArgType>> {
    template <typename BeanCreators>
    static constexpr std::list<ArgType> Invoke(BeanCreators&& creators) {
        std::list<ArgType> result;
        hana::for_each(std::forward<BeanCreators>(creators), [&](auto&& creator) {
            auto instance = BeanCreatorInvoker<ArgType>::Invoke(hana::make_tuple(creator));
            result.push_back(std::move(instance));
        });
        return result;
    }
};

template <typename ArgType>
struct BeanCreatorInvoker<std::vector<ArgType>> {
    template <typename BeanCreators>
    static constexpr std::vector<ArgType> Invoke(BeanCreators&& creators) {
        std::vector<ArgType> result;
        hana::for_each(std::forward<BeanCreators>(creators), [&](auto&& creator) {
            auto instance = BeanCreatorInvoker<ArgType>::Invoke(hana::make_tuple(creator));
            result.push_back(std::move(instance));
        });
        return result;
    }
};
}  // namespace summer::bean::constructor

#endif /* SUMMER_BEAN_CONSTRUCTOR */
