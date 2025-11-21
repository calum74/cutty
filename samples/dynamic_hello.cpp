#include <cutty/dynamic.hpp>
#include <iostream>

int main()
{
    dynamic s = "Hello,";
    // s += "world!";  TODO
    s = s + "world!";  
    std::cout << s << std::endl;
}
