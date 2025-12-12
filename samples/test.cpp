#include <cutty/check.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;

void test1()
{
}

void test2()
{
}

int main(int argc, char**argv)
{
    return cy::test(argc, argv, {test1, {"t2", test2}});
}
