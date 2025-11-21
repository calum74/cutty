#include <dynamic.hpp>
#include <iostream>

int main()
{
    // Creating booleans
    dynamic True = true;
    dynamic False = false;

    // String representation of booleans
    // Since dynamic follows the C++ conventions, this outputs 0 and 1
    std::cout << "The false value: " << False << std::endl;
    std::cout << "The true value: " << True << std::endl;

    // The string representation of a bool is just the C++ stream output
    std::cout << "The false value has string " << False.str() << std::endl;

    std::cout << "The type of a bool is " << False.type_str() << std::endl;

    // You can access the internals
    bool *b = True.try_get<bool>();
    if (b)
    {
        std::cout << "Yes, it's a bool with value " << *b << std::endl;
    }

    // Operations on bools
    if (True == True)
    {
        std::cout << "Everything is ok\n";
    }

    if (True == False)
    {
        std::cout << "Oh no!\n";
    }

    // Conversion to bool

    if (True)
    {
        std::cout << "Conversion to bools\n";
    }

    if(2_d)
    {
        std::cout << "2 is true\n";
    }

    // Throws "Unsupported operation" as strings are not boolean
    // if("abc"_d)
}