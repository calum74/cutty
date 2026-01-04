#include <cutty/dynamic/instantiate.hpp>
#include <cutty/dynamic/empty.hpp>

#include <iostream>

namespace cy = cutty;

bool operator==(cy::dynamic::empty, cy::dynamic::empty)
{
    return true;
}

bool operator<(cy::dynamic::empty, cy::dynamic::empty)
{
    return false;
}

std::ostream &cy::operator<<(std::ostream &os, cy::dynamic::empty)
{
    return os << "(empty)";
}

CY_INSTANTIATE(cy::dynamic::empty)
CY_INSTANTIATE(std::string)
CY_INSTANTIATE(char *)
CY_INSTANTIATE(std::string_view)
