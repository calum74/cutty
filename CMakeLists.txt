cmake_minimum_required(VERSION 3.28)
project(cutty)
set(CMAKE_CXX_STANDARD 23)
enable_testing()

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/src/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# --- clang-format support ---
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

if(NOT CLANG_FORMAT_EXECUTABLE)
    message(WARNING "clang-format not found! Formatting targets disabled.")
else()
    # List all source files you want formatted
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    )

    # --- Target: format (modifies files in place) ---
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
            -i
            -style=Microsoft
            ${ALL_CXX_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting all source files with clang-format (Microsoft style)"
    )

    # --- Target: format-check (fails on misformatted files) ---
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
            --dry-run
            --Werror
            -style=Microsoft
            ${ALL_CXX_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking C++ formatting (Microsoft style)"
    )
endif()

include_directories(include)

add_library(cutty
    include/cutty/check.hpp
    src/check.cpp
    src/cutty.cpp
    src/persist.cpp
    src/shared_memory.cpp
    src/test.cpp
    src/dynamic/dynamic.cpp
    src/dynamic/containers.cpp
    src/dynamic/containers2.cpp
    src/dynamic/containers3.cpp
    src/dynamic/functions.cpp
    src/dynamic/reference.cpp
    src/dynamic/types.cpp
    src/dynamic/types2.cpp)

link_libraries(cutty)

add_executable(approx_sample samples/approx.cpp)
add_executable(mixins samples/mixins.cpp)
add_executable(with samples/with.cpp)
add_executable(dynamic_functions_test test/dynamic/functions.cpp)
add_executable(dynamic_test test/dynamic/test.cpp)
add_executable(dynamic_bools samples/dynamic/bools.cpp)
add_executable(dynamic_empty samples/dynamic/empty.cpp)
add_executable(dynamic_functions samples/dynamic/functions.cpp)
add_executable(dynamic_user_type samples/dynamic/enable_user_type.cpp samples/dynamic/use_user_type.cpp)
add_executable(dynamic_hello samples/dynamic/hello.cpp)
add_executable(dynamic_maps samples/dynamic/maps.cpp)
add_executable(dynamic_tutorial samples/dynamic/tutorial.cpp)
add_executable(dynamic_short_tutorial samples/dynamic/short_tutorial.cpp)
add_executable(fixed_string_test test/fixed_string.cpp)
add_executable(pretty_type test/pretty_type.cpp)
add_executable(pretty_type_sample samples/pretty_type.cpp)
add_executable(scope_hooks samples/scope_hooks.cpp)
add_executable(persist_test test/persist_test.cpp)
add_executable(print_sample samples/print.cpp)
add_executable(print_test test/print.cpp)
add_executable(property_test test/property.cpp)
add_executable(property_sample samples/property.cpp)
add_executable(separator_sample samples/separator.cpp)
add_executable(sequence_test test/sequence/test_sequence.cpp)
add_executable(sequence_test2 test/sequence/test_sequence2.cpp)
add_executable(sequence_creation samples/sequence/creation.cpp)
add_executable(sequence_csvreader samples/sequence/csvreader.cpp)
add_executable(sequence_example1 samples/sequence/example1.cpp)
add_executable(sequence_example2 samples/sequence/example2.cpp)
add_executable(sequence_functions samples/sequence/functions.cpp)
add_executable(sequence_operations samples/sequence/operations.cpp)
add_executable(sequence_primes samples/sequence/primes.cpp)
add_executable(sequence_templates samples/sequence/templates.cpp)
add_executable(sequence_transformations samples/sequence/transformations.cpp)
add_executable(sequence_writers samples/sequence/writers.cpp)
add_executable(shared_memory_test test/shared_memory_test.cpp)
add_executable(tag_test test/tags.cpp)
add_executable(tag_sample samples/tags.cpp)
add_executable(tensor_test test/tensor.cpp)
add_executable(unit_test test/units.cpp)
add_executable(unit_sample samples/units.cpp)
add_executable(test_sample samples/test.cpp)
add_executable(test_test test/test.cpp)

add_test(approx approx_sample)
add_test(mixins mixins)
add_test(with with)
add_test(dynamic_bools dynamic_bools)
add_test(dynamic_empty dynamic_empty)
add_test(dynamic_functions dynamic_functions)
add_test(dynamic_functions_test dynamic_functions_test)
add_test(dynamic_user_type dynamic_user_type)
add_test(dynamic_hello dynamic_hello)
add_test(dynamic_maps dynamic_maps)
add_test(dynamic_tutorial dynamic_tutorial)
add_test(dynamic_short_tutorial dynamic_short_tutorial)
add_test(dynamic_test dynamic_test)
add_test(fixed_string_test fixed_string_test)
add_test(persist persist_test)
add_test(pretty_type pretty_type)
add_test(pretty_type_sample pretty_type_sample)
add_test(print_sample print_sample)
add_test(print_test print_test)
add_test(scope_hooks scope_hooks)
add_test(property_test property_test)
add_test(property_sample property_sample)
add_test(separator_sample separator_sample)
add_test(sequence sequence_test)
add_test(sequence2 sequence_test2)
add_test(sequence_creation sequence_creation)
add_test(sequence_csvreader sequence_csvreader)
add_test(sequence_example1 sequence_example1)
add_test(sequence_example2 sequence_example2)
add_test(sequence_functions sequence_functions)
add_test(sequence_operations sequence_operations)
add_test(sequence_primes sequence_primes)
add_test(sequence_templates sequence_templates)
add_test(sequence_transformations sequence_transformations)
add_test(sequence_writers sequence_writers)
add_test(shared_memory shared_memory_test)
add_test(tag_test tag_test)
add_test(tag_sample tag_sample)
add_test(tensor_test tensor_test)
add_test(test_test test_test)
add_test(test_sample test_sample)
add_test(unit_test unit_test)
add_test(unit_sample unit_sample)