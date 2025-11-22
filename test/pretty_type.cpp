#include <cutty/check.hpp>
#include <cutty/pretty_type.hpp>

#include <iostream>

namespace cy = cutty;

int main()
{
    std::cout << "The pretty type is " << cy::pretty_type<bool>() << std::endl;
    cy::check(cy::pretty_type<bool>() == "bool");
}
