#pragma once

#include "colour.hpp"

namespace cutty::ansi
{

struct style
{
    colour fg = terminal_default;
    colour bg = terminal_default;
    bool bold = false;
    bool faint = false;
    bool underline = false;
    bool strikethrough = false;

    bool operator==(const style &) const = default;
};

} // namespace cutty::ansi
