#pragma once

#include "style.hpp"

namespace cutty::ansi
{
    struct theme
    {
        style window {.bg=blue1};
        style title {.bg=blue1};
        style text {.fg=white, .bg=blue1};
        style data_text {.fg=yellow, .bg=black};
        style text_input {.fg=white, .bg=black};
        style button_normal {.fg=red, .bg=white};
        style button_focus{.fg=red, .bg=yellow, .bold=true};
        style status { .fg=blue1, .bg=white};
    };

    extern theme default_theme;
    extern theme ascii_theme;
}
