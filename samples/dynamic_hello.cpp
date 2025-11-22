#include <cutty/dynamic.hpp>
#include <iostream>

namespace cy = cutty;

int main()
{
    cy::dynamic s = "Hello,";
    // s += "world!";  TODO
    s = s + "world!";  
    std::cout << s << std::endl;
}
