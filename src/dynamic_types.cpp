#include <cutty/dynamic/enable.hpp>

#include <iostream>

namespace cy=cutty;

#define BYVALUE_TYPE(T) template void cy::dynamic::enable<T>();

bool operator==(cy::dynamic::empty, cy::dynamic::empty)
{
    return true;
}

bool operator<(cy::dynamic::empty, cy::dynamic::empty)
{
    return false;
}

std::ostream &operator<<(std::ostream &os, cy::dynamic::empty)
{
    return os << "(empty)";
}

BYVALUE_TYPE(cy::dynamic::empty)
BYVALUE_TYPE(bool)
BYVALUE_TYPE(signed char)
BYVALUE_TYPE(unsigned char)
BYVALUE_TYPE(char)

BYVALUE_TYPE(short)
BYVALUE_TYPE(unsigned short)
BYVALUE_TYPE(int)
BYVALUE_TYPE(unsigned int)
BYVALUE_TYPE(long)
BYVALUE_TYPE(unsigned long)
BYVALUE_TYPE(long long)
BYVALUE_TYPE(unsigned long long)

BYVALUE_TYPE(float)
BYVALUE_TYPE(double)

BYVALUE_TYPE(std::string)
BYVALUE_TYPE(char *)
BYVALUE_TYPE(std::string_view)
