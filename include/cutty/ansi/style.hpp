#pragma once

#include "colour.hpp"

namespace cutty::ansi
{

struct style
{
    colour fg = colour::terminal_default;
    colour bg = colour::terminal_default;
    bool bold = false;
    bool faint = false;
    bool underline = false;
    bool strikethrough = false;

    bool operator==(const style &) const = default;
};

} // namespace cutty::ansi
