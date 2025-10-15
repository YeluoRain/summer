cmake_minimum_required(VERSION 3.23)
project(summer)

set(CMAKE_CXX_STANDARD 17)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(test)