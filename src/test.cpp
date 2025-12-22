#include <cutty/test.hpp>
#include <iostream>

namespace cy = cutty;

cy::test_fixture::test_fixture(test_function fn) : function(fn), name("test")
{
}

cy::test_fixture::test_fixture(const char * name, test_function fn) : function(fn), name(name)
{
}

int cy::test(int argc, const char**argv, std::initializer_list<test_fixture> tests)
{
    return test(tests);
}

int cy::test(int argc, char**argv, std::initializer_list<test_fixture>tests)
{
    return test(tests);
}

int cy::test(std::initializer_list<test_fixture> list)
{
    int passed = 0, failed = 0;
    std::cout << "Running " << list.size() << " tests\n";
    for(auto & test : list)
    {
        std::cout << "  " << test.name << ": " << std::flush;
        try
        {
            test.function();
            std::cout << "PASSED\n";
            ++passed;
        }
        catch(std::exception &ex)
        {
            std::cout << "FAILED: " << ex.what() << std::endl;
            ++failed;
        }
        catch(...)
        {
            std::cout << "FAILED (exception)\n";
            ++failed;
        }
    }
    if(failed==0)
    {
        std::cout << "Tests PASSED\n";
        return 0;
    }
    else
    {
        std::cout << failed << " tests FAILED\n";
        return 1;
    }
}