#include <cutty/dynamic/instantiate.hpp>

#include <iostream>

namespace cy = cutty;

bool operator==(cy::dynamic::empty_type, cy::dynamic::empty_type)
{
    return true;
}

bool operator<(cy::dynamic::empty_type, cy::dynamic::empty_type)
{
    return false;
}

std::ostream &cy::operator<<(std::ostream &os, cy::dynamic::empty_type)
{
    return os << "(empty)";
}

CY_INSTANTIATE(cy::dynamic::empty_type)
CY_INSTANTIATE(std::string)
CY_INSTANTIATE(char *)
CY_INSTANTIATE(std::string_view)
