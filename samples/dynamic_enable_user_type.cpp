#include "dynamic_user_type.hpp"
#include <cutty/dynamic/instantiate.hpp>

namespace cy = cutty;

template<> struct cy::dynamic::traits<MyStruct> : public dynamic::default_traits<MyStruct>
{
    static dynamic op_mul(const MyStruct &x, const dynamic &y)
    {
        return "Multiplication is not supported"_d;
    }
};

template const cy::dynamic::types & cy::dynamic::instantiate<MyStruct>();