#pragma once

#include "style.hpp"

namespace cutty::ansi
{
    struct theme
    {
        style window;
        style title;
        style text;
        style button_normal;
        style button_focus;
        style status;
    };

    extern theme default_theme;
    extern theme ascii_theme;
}
