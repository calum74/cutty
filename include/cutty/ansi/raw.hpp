#pragma once

#include <iosfwd>
#include "common.hpp"
#include "style.hpp"

namespace cutty::ansi
{
// Returns the size of the terminal
size get_terminal_size();

position read_position(std::ostream &os, std::istream &is);

// Basic control sequences
void start_of_line(std::ostream &);
void up(std::ostream &);
void down(std::ostream &);
void left(std::ostream &);
void right(std::ostream &);
void up(int n, std::ostream &);
void down(int n, std::ostream &);
void left(int n, std::ostream &);
void right(int n, std::ostream &);
void move_to_absolute(position, std::ostream &);
void move_to_column(int x, std::ostream &);
void home(std::ostream&);

void wrap_off(std::ostream &);
void wrap_on(std::ostream &);
void cursor_hide(std::ostream &);
void cursor_show(std::ostream &);
void alternate_screen_on(std::ostream &);
void alternate_screen_off(std::ostream &);

enum cursor_style
{
    default_cursor,
    blinking_block,
    steady_block,
    blinking_underline,
    steady_underline,
    blinking_bar,
    steady_bar
};

void set_cursor(cursor_style);

// SGR - complete codes
void reset(std::ostream &);
void fg_colour(colour c, std::ostream &os);
void bg_colour(colour c, std::ostream &os);

enum class sgr_style : std::uint8_t
{
    reset             = 0,

    heavy_weight      = 1,
    light_weight      = 2,
    normal_weight     = 22,

    italic            = 3,
    no_italic         = 23,

    underline         = 4,
    double_underline  = 21,
    no_underline      = 24,

    slow_blink        = 5,
    rapid_blink       = 6,
    no_blink          = 25,

    reverse           = 7,
    no_reverse        = 27,

    conceal            = 8,
    no_conceal        = 28,

    strikethrough     = 9,
    no_strikethrough  = 29,

    frame             = 51,
    round_frame       = 52,
    no_frame          = 54,

    overline          = 53,
    no_overline       = 55
};

// Apply single style - standalone sgr sequence
void apply(sgr_style, std::ostream &os);

// SGR - assemble SGR sequence
void sgr_start(std::ostream &);
void sgr_next(std::ostream &);
void sgr_end(std::ostream &);
void sgr_reset(std::ostream &);
void sgr_fg(colour, std::ostream &);
void sgr_bg(colour, std::ostream &);
void sgr_finish(std::ostream &os);
void sgr_apply(sgr_style, std::ostream &os);

void change_style(const style &old_style, const style &new_style, std::ostream &os);
}