#include <cutty/dynamic.hpp>
#include "dynamic_user_type.hpp"
#include <iostream>

int main()
{
    dynamic d1 = MyStruct(4), d2 = d1;

    std::cout << "d1 is " << d1 << std::endl;
    std::cout << "d2 is " << d2 << std::endl;

    // Implemented by implicit support for MyStruct+MyStruct
    std::cout << "d1+d2 is " << (d1+d2) << std::endl;

    // Implemented by explicit specialization of dynamic_ext::traits
    std::cout << "d1*d2 is " << (d1*d2) << std::endl;
}
