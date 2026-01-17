#include <cutty/dynamic.hpp>
#include <iostream>

namespace cy = cutty;

int main()
{
    // Create a map
    // Maps must contain keys of the same type,
    // with the exception that numbers and strings can be compared (and compared with each other)
    auto m = cy::dynamic::map();

    // Dictionaries can contain keys of any type
    // The main difference between dict() and map() are that map() is ordered.
    // A dict is stored as a hash table.
    auto d = cy::dynamic::dict();

    m[1] = 2;
    d["hello"] = "fred";
    d[true] = "bob";

    // Output a list of contents
    std::cout << m << std::endl;
    std::cout << d << std::endl;

    // Dictionaries can be initialised
    d = cy::dynamic::dict({ {"Fred", 42}, {"Joe", 89} });

    for(auto item : d)
    {
        std::cout << item.first() << ": " << item.second() << std::endl;
    }

    // Structured declarations
    for(auto [k,v] : d)
    {
        std::cout << k << ": " << v << std::endl;
    }

    d["Fred"] = 43;
    std::cout << "Fred is " << d["Fred"]  << std::endl;
    d["Sam"] = 0;
    std::cout << "There are " << d.size() << " items\n";
}
