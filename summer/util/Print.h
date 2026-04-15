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

#ifndef PRINT_H
#define PRINT_H

#include "boost/core/demangle.hpp"

namespace summer::util::print {

static constexpr auto ToString = [](const auto& item) {
    using T = typename std::decay_t<decltype(item)>;
    return boost::core::demangle(typeid(T).name());
};

template <typename T>
struct ErrorAssert : std::false_type {};

template <typename T>
struct DependencyErrorPrint {
    static void Print() {
        static_assert(ErrorAssert<T>::value,
                      "There is a dependency cycle or missing bean in the bean graph, bean type is "
                      "shown above.");
    }
};

}  // namespace summer::util::print

#endif  // PRINT_H