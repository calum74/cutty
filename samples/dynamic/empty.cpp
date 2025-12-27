#include <cutty/dynamic.hpp>
#include <cutty/test.hpp>
#include <iostream>

namespace cy = cutty;

int main()
{
    // Default-constructing a dynamic creates an "empty" type
    cy::dynamic empty;
    cy::dynamic empty2 = {};  // ?? Is this a list now

    if(empty == empty2)
    {
        std::cout << "Empty is equal to other empties\n";
    }

    // Output
    std::cout << "Empty is written as " << empty << std::endl;

    // Convert to bool

    // Test for empty

    // The type of empty is dynamic::empty
    std::cout << "The type of empty is " << empty.type_str() << std::endl;
}