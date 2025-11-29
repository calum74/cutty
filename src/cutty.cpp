#include <cutty/fraction.hpp>

#include <iostream>

std::ostream & cutty::operator<<(std::ostream &os, fraction p)
{
    os << p.numerator;
    if(p.denominator!=1) os << "/" << p.denominator;
    return os;
}
