#pragma once

#include "style.hpp"

namespace cutty::ansi
{
    struct theme
    {
        // TODO: These defaults don't belong here
        style window {.bg=colour::blue};
        style title {.bg=colour::blue};
        style text {.fg=colour::white, .bg=colour::blue};
        style data_text {.fg=colour::yellow, .bg=colour::black};
        style text_input {.fg=colour::white, .bg=colour::black};
        style button_normal {.fg=colour::red, .bg=colour::white};
        style button_focus{.fg=colour::red, .bg=colour::yellow, .bold=true};
        style status { .fg=colour::blue, .bg=colour::white};
        style x_normal { .fg = colour::white, .bg = colour::red };
        style x_highlight { .fg = colour::white, .bg = colour::red, .bold=true };
        style disabled_text { .faint=true };
    };

    const theme &default_theme();
    const theme &ascii_theme();
}
