#include <cutty/test.hpp>

namespace cy = cutty;

cy::test_suite::test_suite(test_function fn) : function(fn), name()
{
}

cy::test_suite::test_suite(const char * name, test_function fn) : function(fn), name(name)
{
}

int cy::test(int argc, const char**argv, std::initializer_list<test_suite>)
{
    // !!
    return 0;
}

int cy::test(int argc, char**argv, std::initializer_list<test_suite>)
{
    // !!
    return 0;
}

int cy::test(std::initializer_list<test_suite> list)
{
    // !!
    return 0;
}