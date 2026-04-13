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

#ifndef SUMMER_DAG_GRAPH
#define SUMMER_DAG_GRAPH

#include "boost/hana.hpp"

namespace summer::dag::graph {
namespace hana = boost::hana;

template <typename T>
struct Vertex {
    using NodeType = T;
    using BeanType = typename NodeType::BeanType;
    constexpr static auto InList = hana::append(NodeType::ImplementOf, hana::type_c<BeanType>);
    constexpr static auto OutList = NodeType::DependOn;
    constexpr static auto InDegree = hana::size(InList);
    constexpr static auto OutDegree = hana::size(OutList);
};
}  // namespace summer::dag::graph

#endif /* SUMMER_DAG_GRAPH */
