#include <cutty/dynamic/instantiate.hpp>

#include <iostream>

namespace cy = cutty;

bool cutty::operator==(cy::dynamic::empty_type, cy::dynamic::empty_type)
{
    return true;
}

std::strong_ordering cutty::operator<=>(cy::dynamic::empty_type, cy::dynamic::empty_type)
{
    return std::strong_ordering::equal;
}

std::ostream &cy::operator<<(std::ostream &os, cy::dynamic::empty_type)
{
    return os << "(empty)";
}

CY_INSTANTIATE(cy::dynamic::empty_type)
CY_INSTANTIATE(std::string)
CY_INSTANTIATE(char *)
CY_INSTANTIATE(std::string_view)
