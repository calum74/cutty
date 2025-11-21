#include <dynamic.hpp>
#include <iostream>

int main()
{
    // An empty list
    dynamic l1 = dynamic::list();

    std::cout << "The empty list: " << l1 << std::endl;

    // A list with 3 items
    dynamic l2 = dynamic::list({1, "foo", 3.1});
    std::cout << "A list with items: " << l2 << std::endl;

    // Iterating a list
    for (dynamic i : l2)
    {
        std::cout << "Items is " << i << std::endl;
    }

    // Set all items to empty
    for (dynamic i : l2)
    {
        i = {};
    }
    std::cout << "All items empty: " << l2 << std::endl;

    // Accessing an index in a list
    l2.front() = 3;
    l2[1] = "new value";
    l2.back() = 1;

    std::cout << "The list is now " << l2 << std::endl;

    // Pushing to the back
    l2.pop_back();
    l2.push_back("x");
    std::cout << "The list is now " << l2 <<  std::endl;
}
