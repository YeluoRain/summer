//
// Created by Zhongwen Wang on 2023/12/11.
//

#ifndef PRINT_H
#define PRINT_H

#include "boost/core/demangle.hpp"

namespace summer::util::print {
    using namespace boost;

    struct ToStringFuncType {
        template<typename T>
        std::string operator()(const T &) const { return core::demangle(typeid(T).name()); }
    };

    inline ToStringFuncType ToString{};
}

#endif //PRINT_H
