#pragma once

#include "../dynamic.hpp"

struct cutty::dynamic::empty
{
};

template<> struct std::hash<cutty::dynamic::empty>
{
    std::size_t operator()(const cutty::dynamic::empty&) const;
};
