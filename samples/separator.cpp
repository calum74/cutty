#include <cutty/separator.hpp>

#include <initializer_list>
#include <iostream>

namespace cy = cutty;

int main()
{
    cy::separator sep;
    for (auto i : {1, 2, 3})
    {
        std::cout << sep << i;
    }
    std::cout << std::endl;

    // Outputs 1; 2; 3
    sep = cy::separator("; ");
    for (auto i : {1, 2, 3})
    {
        std::cout << sep << i;
    }
    std::cout << std::endl;

    /*
    Output:
        1, 2, 3
        1;2;3
    */
}