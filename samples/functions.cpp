#include <dynamic.hpp>
#include <iostream>

// You must include this for this additional functionality
#include <dynamic/function.hpp>

dynamic add(dynamic x, dynamic y)
{
    return x+y;
}

int main()
{
    auto f1 = dynamic::function([&](int x) { return x+1; });
    auto f2 = dynamic::function(add);

    std::cout << f1(1) << ", " << f2(4,5) << std::endl;

    // Void functions return empty
    auto f3 = dynamic::function([] { std::cout << "Hello!\n"; });
    std::cout << f3() << std::endl;
}