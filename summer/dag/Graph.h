//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef GRAPH_H
#define GRAPH_H

#include "boost/hana.hpp"

namespace summer::dag::graph {
    using namespace boost;

    template<typename T>
    struct Vertex {
        using NodeType = T;
        using BeanType = typename NodeType::BeanType;
        constexpr static auto InList = NodeType::ImplementOf;
        constexpr static auto OutList = NodeType::DependOn;
        constexpr static auto InDegree = hana::size(InList);
        constexpr static auto OutDegree = hana::size(OutList);
    };
}


#endif //GRAPH_H
