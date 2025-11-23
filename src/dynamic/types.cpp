#include <cutty/dynamic/instantiate.hpp>

#include <iostream>

namespace cy=cutty;

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

CY_INSTANTIATE(cy::dynamic::empty)
CY_INSTANTIATE(bool)
CY_INSTANTIATE(signed char)
CY_INSTANTIATE(unsigned char)
CY_INSTANTIATE(char)

CY_INSTANTIATE(short)
CY_INSTANTIATE(unsigned short)
CY_INSTANTIATE(int)
CY_INSTANTIATE(unsigned int)
CY_INSTANTIATE(long)
CY_INSTANTIATE(unsigned long)
CY_INSTANTIATE(long long)
CY_INSTANTIATE(unsigned long long)

CY_INSTANTIATE(float)
CY_INSTANTIATE(double)

CY_INSTANTIATE(std::string)
CY_INSTANTIATE(char *)
CY_INSTANTIATE(std::string_view)
