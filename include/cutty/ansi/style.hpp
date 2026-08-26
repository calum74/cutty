#pragma once

#include "colour.hpp"

namespace cutty::ansi
{

    enum class weight
    {
        normal,
        heavy,
        light,
    };

    enum class blink
    {
        none,
        slow,
        fast
    };

    enum class underline
    {
        none,
        single_line,
        double_line
    };

    enum class frame
    {
        none,
        boxed,
        encircled,
    };


struct style
{
    colour fg = colour::terminal_default;
    colour bg = colour::terminal_default;

    ansi::weight weight = ansi::weight::normal;
    ansi::underline underline = ansi::underline::none;
    ansi::blink blink = ansi::blink::none;
    ansi::frame frame = ansi::frame::none;
    bool strikethrough = false;
    bool overline = false;

    bool operator==(const style &) const = default;
};

} // namespace cutty::ansi
