// This example demonstrates how to pass sequences to functions.

// In the .cpp file itself, you still need to include the full library
#include "functions.hpp"
#include <cutty/sequence.hpp>
#include <iostream>

namespace cy = cutty;

// This is a regular function that processes a sequence.
void process1(const cy::sequence<int> &items)
{
    std::cout << "The sum is " << items.sum() << std::endl;
}

// pointer_sequence<> is ever so slightly faster than sequence<>
// as it assumes that the underlying data is stored in an array.
void process2(const cy::pointer_sequence<int> &items)
{
    process1(items);
}

int main()
{
    process1(cy::list(1, 2, 3));
    process2(cy::list(4, 5, 6));
    return 0;
}
