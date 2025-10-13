cmake_minimum_required(VERSION 3.23)
project(summer)

set(CMAKE_CXX_STANDARD 17)
set(REPO_DIR "~/repo")
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-Wunused-variable -Wunused-function -Wunused-parameter -Wunused-local-typedefs)
add_subdirectory(test)