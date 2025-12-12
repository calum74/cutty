#include <cutty/check.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;

void test_1_equals_1()
{
    cy::check_equal(1,1);
}

void test_2_greater_1()
{
    cy::check(2>1);
}

int main(int argc, char**argv)
{
    return cy::test(argc, argv, {
        {"t1", test_1_equals_1},
        {"t2", test_2_greater_1}
    });
}
