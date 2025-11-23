// This example demonstrates the various ways to create sequences

#include <cutty/sequence.hpp>
#include <iostream>
#include <vector>
#include <sstream>

namespace cy = cutty;

void print(const cy::sequence<const char*> & items)
{
    for(auto i : items)
        std::cout << i << std::endl;
}

int main()
{
    // Create an empty sequence (or an empty list - they are the same thing)
    print(cy::seq<const char*>());
    print(cy::list<const char*>());

    // Create a sequence with the given items
    print(cy::list("hello"));
    print(cy::list("cat", "dog", "parrot"));

    // Create a sequence from a container
    std::vector<const char*> items { "Beethoven", "Chopin", "Liszt"};
    print(cy::seq(items));

    // Create a sequence from iterators
    print(cy::seq(items.rbegin(), items.rend()));

    // Create a sequence from C arrays
    const char *carray[] = { "Red", "White", "Rose" };
    print(cy::seq(carray));
    print(cy::seq(carray+1, 2));

    // Create an integer range
    auto r = cy::seq(1,10);

    // Output: 55
    std::cout << r.sum() << std::endl;

    // Create a sequence of characters in a C string
    std::cout << cy::seq("Bergerac").size() << std::endl;

    // Create a sequence of characters from a stream
    std::stringstream ss("a b c");
    std::cout << cy::seq(ss).front() << std::endl;

    return 0;
}