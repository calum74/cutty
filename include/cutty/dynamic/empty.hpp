#pragma once

#include "../dynamic.hpp"

struct cutty::dynamic::empty
{
};

bool operator==(cutty::dynamic::empty, cutty::dynamic::empty);
bool operator<(cutty::dynamic::empty, cutty::dynamic::empty);
std::ostream &operator<<(std::ostream &os, cutty::dynamic::empty);
