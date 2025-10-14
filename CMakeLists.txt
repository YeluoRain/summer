cmake_minimum_required(VERSION 3.23)
project(summer)

set(CMAKE_CXX_STANDARD 17)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(BOOST_ROOT $ENV{BOOST_ROOT})
set(GTEST_ROOT $ENV{GTEST_ROOT})

message(STATUS "BOOST_ROOT: ${BOOST_ROOT}")
message(STATUS "GTEST_ROOT: ${GTEST_ROOT}")

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(test)