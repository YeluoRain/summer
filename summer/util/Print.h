//
// Created by Zhongwen Wang on 2023/12/11.
//

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
