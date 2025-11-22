cmake_minimum_required(VERSION 3.28)
project(cutty)
set(CMAKE_CXX_STANDARD 23)
enable_testing()

include_directories(include)

add_library(cutty
    src/check.cpp
    src/dynamic.cpp
    src/dynamic_containers.cpp
    src/dynamic_functions.cpp
    src/dynamic_reference.cpp
    src/dynamic_types.cpp)

link_libraries(cutty)

add_executable(mixins samples/mixins.cpp)
add_executable(satellite samples/satellite.cpp)
add_executable(dynamic_functions test/dynamic_functions.cpp)
add_executable(dynamic_test1 test/dynamic_test1.cpp)
add_executable(dynamic_bools samples/dynamic_bools.cpp)
add_executable(dynamic_empty samples/dynamic_empty.cpp)
add_executable(dynamic_user_type samples/dynamic_enable_user_type.cpp samples/dynamic_use_user_type.cpp)
add_executable(dynamic_hello samples/dynamic_hello.cpp)
add_executable(dynamic_maps samples/dynamic_maps.cpp)
add_executable(dynamic_tutorial samples/dynamic_tutorial.cpp)
add_executable(pretty_type test/pretty_type.cpp)
add_executable(scope_hooks samples/scope_hooks.cpp)

add_test(mixins mixins)
add_test(satellite satellite)
add_test(dynamic_bools dynamic_bools)
add_test(dynamic_empty dynamic_empty)
add_test(dynamic_functions dynamic_functions)
add_test(dynamic_user_type dynamic_user_type)
add_test(dynamic_hello dynamic_hello)
add_test(dynamic_maps dynamic_maps)
add_test(dynamic_tutorial dynamic_tutorial)
add_test(pretty_type pretty_type)
add_test(scope_hooks scope_hooks)
