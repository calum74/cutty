#include <cutty/check.hpp>
#include <cutty/test.hpp>

#include <numbers>

namespace cy = cutty;

void test_1_equals_1()
{
    cy::check_equal(1, 1);
}

void test_2_greater_1()
{
    cy::check(2 > 1);
}

void test_pi()
{
    cy::check_equal(std::numbers::pi, cy::approx(3.14159265));
}

int main()
{
    // Run a series of tests, returning 0 on success:
    return cy::test({test_1_equals_1, test_2_greater_1, test_pi});

    // Run tests, naming each test
    return cy::test({{"t1", test_1_equals_1}, {"t2", test_2_greater_1}, {"pi", test_pi}});
}
