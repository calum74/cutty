
#include <cutty/check.hpp>
#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>

#include <iostream>

namespace cy = cutty;

int main()
{
    {
        auto f = dynamic::function([&](int x) { return x + 1; });
        cy::check(f(2) == 3);
    }

    // This (correctly) does not compile because the arguments cannot be deduced
    // dynamic::function([](auto x) { return x; });

    {
        // Function with no arguments
        auto f1 = dynamic::function([] { return 1; });
        cy::check(f1() == 1);

        auto f2 = dynamic::function([] { std::cout << "Hello!\n"; });
        f2(); // "Hello!"
        cy::check(f2() == dynamic());
    }

    // Functions that return dynamic
    {
        auto f1 = dynamic::function([] { return dynamic(42); });
        cy::check(f1() == 42);
    }

    // Function that takes dynamic arguments
    {
        auto f = dynamic::function([](const dynamic &x) { return x + x; });
        cy::check(f(2) == 4);

        auto g = dynamic::function([](const dynamic &x, const dynamic &y, const dynamic &z) { return x + y + z; });
        cy::check(g(1, 2, 3) == 6);

        f = dynamic::function([](dynamic x) { return x + x; });
        cy::check(f(2) == 4);
    }

    // Not callable
    {
        dynamic i = 10;
        cy::check_throws([&]{i(1,2,3);}, "Unsupported operation '()' on 10");
    }

    {
        dynamic f = dynamic::function([](int x, double y) { return x + y; });
        cy::check(f(1, 2.0) == 3.0);
        cy::check_throws([&]{f();}, "Function called with wrong number of arguments, expected 2 but got 0");
        cy::check_throws([&]{f(1,2);}, "Wrong argument type to function");
    }
}
