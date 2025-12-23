#pragma once

#include "approx.hpp"
#include "check.hpp"

#include <initializer_list>

namespace cutty
{
    struct test_fixture
    {
        using test_function = void(*)();

        test_fixture(test_function);
        test_fixture(const char*, test_function);

        test_function function;
        const char * name;
    };

    int test(int argc, const char**argv, std::initializer_list<test_fixture>);
    int test(std::initializer_list<test_fixture>);
}