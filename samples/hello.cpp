#include <dynamic.hpp>
#include <iostream>

int main()
{
    dynamic s = "Hello,";
    s += "world!";
    std::cout << s << std::endl;
}
