#pragma once

#include "../dynamic.hpp"

struct cutty::dynamic::empty_type
{
};

template<> struct std::hash<cutty::dynamic::empty_type>
{
    std::size_t operator()(const cutty::dynamic::empty_type&) const;
};
