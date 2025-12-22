#pragma once

#include <iosfwd>

namespace cutty
{
struct separator
{
    const char *str = ", ";
    bool printed = false;
};

std::ostream &operator<<(std::ostream &os, separator &sep);
}; // namespace cutty
