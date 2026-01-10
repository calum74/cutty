
#include <cutty/check.hpp>
#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>

#include <iostream>

namespace cy = cutty;

int main()
{
    {
        auto f = cy::dynamic::function([&](int x) { return x + 1; });
        cy::check(f(2) == 3);
    }

    // This (correctly) does not compile because the arguments cannot be deduced
    // dynamic::function([](auto x) { return x; });

    {
        // Function with no arguments
        auto f1 = cy::dynamic::function([] { return 1; });
        cy::check(f1() == 1);

        auto f2 = cy::dynamic::function([] { std::cout << "Hello!\n"; });
        f2(); // "Hello!"
        cy::check(f2() == cy::dynamic());
    }

    // Functions that return dynamic
    {
        auto f1 = cy::dynamic::function([] { return cy::dynamic(42); });
        cy::check(f1() == 42);
    }

    // Function that takes dynamic arguments
    {
        auto f = cy::dynamic::function([](const cy::dynamic &x) { return x + x; });
        cy::check(f(2) == 4);

        auto g = cy::dynamic::function([](const cy::dynamic &x, const cy::dynamic &y, const cy::dynamic &z) { return x + y + z; });
        cy::check(g(1, 2, 3) == 6);

        f = cy::dynamic::function([](cy::dynamic x) { return x + x; });
        cy::check(f(2) == 4);
    }

    // Not callable
    {
        cy::dynamic i = 10;
        cy::check_throws([&]{i(1,2,3);}, "Unsupported operation '()' on value of type int");
    }

    {
        cy::dynamic f = cy::dynamic::function([](int x, double y) { return x + y; });
        cy::check(f(1, 2.0) == 3.0);
        cy::check_throws([&]{f();}, "Function called with wrong number of arguments, expected 2 but got 0");
        cy::check_throws([&]{f(1,2);}, "Wrong argument type to function");
    }
}
