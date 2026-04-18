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

#ifndef SUMMER_UTIL_COLLECTION
#define SUMMER_UTIL_COLLECTION

#include "boost/hana.hpp"
#include "boost/mp11.hpp"

namespace summer::util::collection::tuple {
using namespace boost;

struct MergeTupleFuncType {
    static constexpr auto Merge() {
        return boost::hana::make_tuple();
    }

    template <typename Tuple0>
    static constexpr auto Merge(const Tuple0& tuple0) {
        return tuple0;
    }

    template <typename Tuple0, typename Tuple1, typename... Tuplen>
    static constexpr auto Merge(const Tuple0& tuple0, const Tuple1& tuple1,
                                const Tuplen&... tuplen) {
        auto tuple = boost::hana::concat(tuple0, tuple1);
        return Merge(tuple, tuplen...);
    }
};

constexpr auto Merge = [](const auto&... xs) { return MergeTupleFuncType::Merge(xs...); };

static constexpr auto RemoveDuplicates = [](auto&& tuple) {
    using Type = typename decltype(hana::typeid_(tuple))::type;
    return mp11::mp_unique<Type>{};
};

static constexpr auto TupleMinus = [](auto&& tuple0, auto&& tuple1) {
    return hana::remove_if(tuple0, [&](const auto& item) { return hana::contains(tuple1, item); });
};
}  // namespace summer::util::collection::tuple

#endif /* SUMMER_UTIL_COLLECTION */
