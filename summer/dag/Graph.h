//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef GRAPH_H
#define GRAPH_H

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

#endif  // GRAPH_H
