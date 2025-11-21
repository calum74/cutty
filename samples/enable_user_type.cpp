#include "user_type.hpp"
#include <dynamic/enable.hpp>

template<> struct dynamic::traits<MyStruct> : public dynamic::default_traits<MyStruct>
{
    static dynamic op_mul(const MyStruct &x, const dynamic &y)
    {
        return "Multiplication is not supported"_d;
    }
};

template void dynamic::enable<MyStruct>();
