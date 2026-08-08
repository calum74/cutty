#pragma once

#include "style.hpp"

namespace cutty::ansi
{
    struct theme
    {
        // TODO: These defaults don't belong here
        style window {.bg=blue1};
        style title {.bg=blue1};
        style text {.fg=white, .bg=blue1};
        style data_text {.fg=yellow, .bg=black};
        style text_input {.fg=white, .bg=black};
        style button_normal {.fg=red, .bg=white};
        style button_focus{.fg=red, .bg=yellow, .bold=true};
        style status { .fg=blue1, .bg=white};
        style x_normal { .fg = white, .bg = red };
        style x_highlight { .fg = white, .bg = red, .bold=true };
        style disabled_text { .faint=true };
    };

    const theme &default_theme();
    const theme &ascii_theme();
}
