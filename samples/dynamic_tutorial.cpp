// The C++ "dynamic" tutorial

// 1. Header file
#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>

#include <iostream>
#include <ranges>

namespace cy = cutty;

int main()
{
    // 2. Creation

    // 2.1 The empty type
    cy::dynamic d21;

    // 2.2 Create different types by assignment
    cy::dynamic d22a = 1;
    cy::dynamic d22b = true;
    cy::dynamic d22c = "abc";
    cy::dynamic d22d = 3.14;

    // 2.3 Assignment operator

    // Assigning C++ values to dynamic
    cy::dynamic d23;
    d23 = 2.217;

    // 2.4 Dynamic literals have the _d suffix
    // Creating dynamic values using literals
    auto d24a = 12_d;
    auto d24b = 3.14_d;
    auto d24c = "abc"_d;
    auto d24e = 'x'_d;

    // 3. Properties

    // 3.1 Check if d1 is empty
    cy::dynamic d31;
    if(d31 == cy::dynamic())
    {
        std::cout << "d1 is empty\n";
    }

    // 3.2 Output to std::ostream
    std::cout << "3.14: " << 3.14_d << std::endl;

    // 3.3 Conversion to std::string
    // The default implementation uses the C++ representation
    std::cout << "d3 is 1: " << (1_d).str() << std::endl;

    // 3.4 Look at the internal type
    std::cout << "The type of a string is " << ("abc"_d).type_str() << std::endl;

    // 4. Conversions
    // 4.1 Conversion to dynamic

    // Binary operators taking two dynamic operands
    std::cout << "1+2 = " << (1_d + 2) << std::endl;
    
    // 4.2 Explicit conversion to bool

    if (1_d)
    {
        std::cout << "1 is indeed true\n";
    }

    // 4.3 Access internal data type

    // try_get returns a pointer to the data (or nullptr)
    auto d43 = "abc"_d;
    if(std::string *s = d43.try_get<std::string>())
    {
        std::cout << "d4 is " << *s << std::endl;
    }

    // as<>() gets the specific type, or throws an exception.
    std::cout << "d4 is: " << d43.as<std::string>() << std::endl;

    // 5. Containers

    // 5.1 Strings

    // Constructing a string
    cy::dynamic d51a = "abc";  // By conversion
    auto d51b = "def"_d;   // By literal

    // Operators on strings
    std::cout << "abcdef: " << (d51a + d51b) << std::endl;

    // Iterating the contents of a string
    for(auto [index, ch] : std::views::zip(std::views::iota(0), d51a))
    {
        std::cout << "Character at postion " << index << " is " << ch << std::endl;
    }

    // Modifying the contents via an iterator
    for(auto [index, ch] : std::views::zip(std::views::iota(0), d51a))
    {
        if (index&1) ch = ' ';
    }
    std::cout << "The string is now " << d51a << std::endl;

    // String size
    std::cout << "The size of abc = " << d51a.size() << std::endl;

    // Access string items
    std::cout << "String is abc: " << d51a.front() << d51a[1] <<  d51a.back() << std::endl;

    // Modify string items
    d51a.front() = 'A';
    d51a[1] = 'B';
    d51a.back() = 'C';
    std::cout << "String is ABC: " << d51a << std::endl;

    // 5.2 Lists

    // Create an empty list
    cy::dynamic d52a = cy::dynamic::list();

    // Create a list from initializer
    cy::dynamic d52b = cy::dynamic::list({1,2,3});

    // Append to a list
    d52b.push_back("Item");

    // Pop from list
    d52b.pop_back();

    // Print list contents
    std::cout << "The list is " << d52b << std::endl;

    // Access and assign list items
    std::cout << "The list contains 1, 2, 3: " << d52b.front() << ", " << d52b[1] << ", " << d52b.back() << std::endl;
    d52b.front() = "x";
    d52b[1] = "y";
    d52b.back() = "z";
    std::cout << "The list [x,y,z]: " << d52b << std::endl;

    // List properties
    std::cout << "The list is " << d52b.size() << std::endl;
    // std::cout << "The list is not empty: " << d52b.empty() << std::endl;

    // 5.3 Dictionary

    // Create an empty dictionary
    cy::dynamic a53a = cy::dynamic::dict();

    // Create a dictionary from an initializer list
    cy::dynamic a53b = cy::dynamic::dict({{"Fred", 10}, {"Joe", 12}});

    // Lookup items
    std::cout << "The item for Fred is 10: " << a53b["Fred"] << std::endl;
    std::cout << "The item for Joe is 12: " << a53b["Joe"] << std::endl;

    // Assign items
    a53b["Fred"] = 11;
    a53b["Bob"] = 87;

    // Display the dict
    std::cout << "The dict contains: " << a53b << std::endl;

    // 5.4 Map
    // Maps and dicts are very similar, but maps are sorted

    cy::dynamic a54a = cy::dynamic::map();

    // Create a dictionary from an initializer list
    cy::dynamic a54b = cy::dynamic::map({{"Fred", 10}, {"Joe", 12}});

    // Lookup items
    std::cout << "The item for Fred is 10: " << a54b["Fred"] << std::endl;
    std::cout << "The item for Joe is 12: " << a54b["Joe"] << std::endl;

    // Assign items
    a54b["Fred"] = 11;
    a54b["Bob"] = 87;

    // Display the dict
    std::cout << "The map contains: " << a54b << std::endl;

    // 5.5 Set

    // Construct set
    cy::dynamic d55a = cy::dynamic::set();
    cy::dynamic d55b = cy::dynamic::set({'a','b','c'});

    std::cout << "Here is the set: " << d55b << std::endl;

    // Insert into set
    d55b.insert('a');
    std::cout << "The set still contains 3 items: " << d55b.size() << std::endl;

    // !! Remove
    // !! count

    // 5.6 Queue
    auto d56a = cy::dynamic::queue();
    auto d56b = cy::dynamic::queue({1,2,3});

    while(d56b.size()>0)
    {
        std::cout << "The front of the queue is " << d56b.front() << std::endl;
        d56b.pop_front();
    }
    
    // 6. Operators

    // 6.1 Unary operators
    // 6.2 Arithmetic operators
    // 6.3 Comparison operators
    // Total ordering
    // 6.4 Assignment operators

    // 7. References
    // 7.1 References to dynamic objects
    cy::dynamic d71a = 5;
    cy::dynamic d71b = d71a.ref();
    d71b = 6;
    std::cout << "d71a now has the value 6: " << d71a << std::endl;

    // 7.2 References to C++ values

    // References to C++ values
    int int_value = 10;

    // Create a reference to C++ value
    auto d72 = cy::dynamic::reference(int_value);
    // dynamic d72(int_value, dynamic::by_reference_tag());

    // Assigning a reference writes the underlying value
    d72 = 5;
    std::cout << "value is now 5: " << int_value << std::endl;

    // 8. Shared pointers
    // 8.1 Shared pointers

    // Create a shared reference
    cy::dynamic d81a = ("abc"_d).shared_ref();
    cy::dynamic d81b = d81a;  // d82 and d81 share the same string
    d81b.push_back('d');
    std::cout << "d81a is abcd: " << d81a << std::endl;

    // 8.2 Weak pointers

    // Create weak pointers using weak_ref() on a shared pointer
    cy::dynamic d82a = d81a.weak_ref();

    // You must convert the weak_pointer to a shared pointer before using it:
    cy::dynamic d82c = d82a.shared_ref();
    std::cout << "d82b is abcd: " << d82c << std::endl;

    // Attempts to create a shared pointer from an expired weak pointer
    d81a = d81b = d82c = cy::dynamic();
    // d82a.shared_ref();  // Throws exception "Expired weak pointer"

    // 9. Functions
    // 9.1. Creating functors using dynamic::function
    cy::dynamic d91a = cy::dynamic::function([](int x) { return x*x; });
    cy::dynamic d91b = cy::dynamic::function([](cy::dynamic x) { return x*x; });

    // Note the following do NOT work:
    // dynamic d91b = [](int x) { return x; };
    // dynamic d91b = dynamic::function([](auto x) { return x; });
    // 
    // In the first case, it would work if you #include <dynamic/instantiate.hpp>
    // In the second case, dynamic cannot deduce the argument type
    // (use dynamic argument type instead).

    // Calling a function
    std::cout << "Call a function: " << d91a(9) << std::endl;

    // Calling a function with the wrong number of arguments
    
}