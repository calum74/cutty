cmake_minimum_required(VERSION 3.28)
project(cutty)
set(CMAKE_CXX_STANDARD 23)
enable_testing()

include_directories(include)

add_executable(mixins_demo samples/mixins_demo.cpp)
add_executable(satellite_demo samples/satellite_demo.cpp)

add_test(mixins mixins_demo)
add_test(satellite satellite_demo)

