#include <cutty/dynamic.hpp>
#include <iostream>

// You must include this for this additional functionality
#include <cutty/dynamic/function.hpp>

namespace cy = cutty;

cy::dynamic add(cy::dynamic x, cy::dynamic y)
{
    return x+y;
}

int main()
{
    cy::dynamic f1 = [&](int x) { return x+1; };
    cy::dynamic f2 = add;

    std::cout << f1(1) << ", " << f2(4,5) << std::endl;

    // Void functions return empty
    cy::dynamic f3 = [] { std::cout << "Hello!\n"; };
    std::cout << f3() << std::endl;
}