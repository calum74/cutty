#pragma once

#include "style.hpp"

namespace cutty::ansi
{

struct character
{
    ansi::style style;
    char32_t ch = ' ';
};

}
