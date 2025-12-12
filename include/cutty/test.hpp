#pragma once

#include <initializer_list>

namespace cutty
{
    using test_function = void(*)();

    struct test_suite
    {
        test_suite(test_function);
        test_suite(const char*, test_function);

        test_function function;
        const char * name;
    };

    int test(int argc, const char**argv, std::initializer_list<test_suite>);
    int test(int argc, char**argv, std::initializer_list<test_suite>);
    int test(std::initializer_list<test_suite>);
}