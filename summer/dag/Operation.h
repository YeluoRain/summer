//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef OPERATION_H
#define OPERATION_H

#include "dag/detail/GraphOperation.h"

namespace summer::dag::operation {
    constexpr detail::GetIndependentBeansFuncType GetIndependentBeans{};
    constexpr detail::FillImplementedMapFuncType FillImplementedMap{};
    constexpr detail::RemoveVertexDependencyFuncType RemoveVertexDependency{};
}

#endif //OPERATION_H
