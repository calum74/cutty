// This example demonstrates output sequences

#include <cutty/sequence.hpp>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <list>

namespace cy = cutty;

void moreItems(const cy::output_sequence<std::string> & output)
{
    output << "Item 7";
}

// This is a function that returns a sequence
void getItems(const cy::output_sequence<std::string> & output)
{
    // Call the push_back() function to output a single element
    output.push_back("Item 1");

    // You can also use the << operator
    output << "Item 2";

    // You can send a sequence to an output
    cy::list("Item 3", "Item 4","Item 5").write_to(output);

    // You can also send sequences via the << operator
    output << cy::list("Item 6");

    // You can build up the output over several functions
    moreItems(output);

    // Use std::back_inserter
    std::ranges::copy(cy::list("Item 1", "Item2", "Item3"), std::back_inserter(output));
}

int main()
{
    std::vector<std::string> vec;
    std::set<std::string> set;
    std::list<std::string> list;

    // writer() creates a writer that adds the items to a container
    getItems(cy::writer(vec));

    // It doesn't matter what container you use
    getItems(cy::writer(set));
    getItems(cy::writer(list));

    // receiver() specifies a function that will receive the itens
    getItems(cy::receiver([](const std::string & str) { std::cout << str << std::endl; }));

    return 0;
}
