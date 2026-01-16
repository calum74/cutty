#include <cutty/dynamic.hpp>
#include <cutty/test.hpp>
#include <iostream>

namespace cy = cutty;

int main()
{
    // Default-constructing a dynamic creates an "empty" type
    cy::dynamic empty;
    cy::dynamic empty2 = {};

    if(empty == empty2)
    {
        std::cout << "Empty is equal to other empties\n";
    }

    // Output
    std::cout << "Empty is written as " << empty << std::endl;

    // Test for empty
    if(empty == cy::dynamic())
    {
        std::cout << "Yes, it's empty!\n";
    }

    if(!empty.has_value())
    {
        std::cout << "It's still empty!\n";
    }

    // The type of empty is dynamic::empty
    std::cout << "The type of empty is " << empty.type_str() << std::endl;
}