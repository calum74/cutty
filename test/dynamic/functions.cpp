#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>
#include <cutty/test.hpp>

#include <iostream>

namespace cy = cutty;

int fn1(int x, int y)
{
    return x + y;
}

cy::dynamic fn2(cy::dynamic x, cy::dynamic y)
{
    return x + y;
}

void category()
{
    cy::dynamic f = [](int x) { return x + 1; };
    cy::check(f.has_function());

    cy::dynamic i = 10;
    cy::check(!i.has_function());
}

void basic()
{
    auto f = cy::dynamic::function([&](int x) { return x + 1; });
    cy::check(f(2) == 3);
}

void assignment()
{
    cy::dynamic f = [](int x) { return x + 1; };
    cy::check_equal(f(2), 3);

    f = [](int x, int y) { return x + y; };
    cy::check_equal(f(2, 3), 5);

    f = fn1;
    f = fn2;
}

void constructors()
{
    // Lambda
    {
        auto fn = [](int x) { return x + 1; };
        cy::dynamic f = fn;
        cy::check_equal(f(2), 3);
    }

    // Function pointer
    {
        cy::dynamic f = fn1;
        cy::check_equal(f(2, 3), 5);
    }

    // Dynamic function pointer
    {
        cy::dynamic f = fn2;
        cy::check_equal(f(2, 3), 5);
    }

    {
        cy::dynamic f1 = [](int x) { return x + 1; };
        cy::dynamic f2 = [](cy::dynamic x) { return x + 1; };
        cy::dynamic f3 = fn1;
        cy::dynamic f4 = &fn1;
        cy::dynamic f5 = fn2;
        cy::dynamic f6 = &fn2;
    }
}

void return_values()
{
    // Function with no arguments
    auto f1 = cy::dynamic::function([] { return 1; });
    cy::check(f1() == 1);

    auto f2 = cy::dynamic::function([] { std::cout << "Hello!\n"; });
    f2(); // "Hello!"
    cy::check(f2() == cy::dynamic());

    auto f3 = cy::dynamic::function([] { return cy::dynamic(42); });
    cy::check(f3() == 42);
}

void dynamic_arguments()
{
    auto f = cy::dynamic::function([](const cy::dynamic &x) { return x + x; });
    cy::check(f(2) == 4);

    auto g = cy::dynamic::function(
        [](const cy::dynamic &x, const cy::dynamic &y, const cy::dynamic &z) { return x + y + z; });
    cy::check(g(1, 2, 3) == 6);

    f = cy::dynamic::function([](cy::dynamic x) { return x + x; });
    cy::check(f(2) == 4);
}

void not_callable()
{
    cy::dynamic i = 10;
    cy::check_throws([&] { i(1, 2, 3); }, "Unsupported operation '()' on value of type int");
}

void wrong_arg_types()
{
    cy::dynamic f = cy::dynamic::function([](int x, double y) { return x + y; });
    cy::check(f(1, 2.0) == 3.0);
    cy::check_throws([&] { f(); }, "Function called with wrong number of arguments, expected 2 but got 0");
    cy::check_throws([&] { f(1, 2); }, "Wrong argument type to function: given 'int', expected 'double'");
}

int main()
{
    return cy::test(
        {basic, dynamic_arguments, not_callable, wrong_arg_types, return_values, category, assignment, constructors});
}
