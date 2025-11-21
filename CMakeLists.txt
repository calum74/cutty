cmake_minimum_required(VERSION 3.28)
project(cutty)
set(CMAKE_CXX_STANDARD 23)
enable_testing()

include_directories(include)

add_library(cutty
    src/check.cpp
    src/dynamic.cpp
    src/reference.cpp
    src/types.cpp)

link_libraries(cutty)

add_executable(mixins_demo samples/mixins_demo.cpp)
add_executable(satellite_demo samples/satellite_demo.cpp)
add_executable(dynamic_functions test/dynamic_functions.cpp)
add_executable(dynamic_test1 test/dynamic_test1.cpp)

add_test(mixins mixins_demo)
add_test(satellite satellite_demo)

