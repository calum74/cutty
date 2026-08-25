#pragma once

#include <iosfwd>
#include "common.hpp"
#include "style.hpp"

namespace cutty::ansi
{
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
void bold_on(std::ostream &);
void bold_off(std::ostream&);
void fg_colour(colour c, std::ostream &os);
void bg_colour(colour c, std::ostream &os);
void invert(std::ostream &os);

// SGR - assemblage
void sgr_start(std::ostream &);
void sgr_next(std::ostream &);
void sgr_end(std::ostream &);
void sgr_reset(std::ostream &);
void sgr_bold_on(std::ostream &os);
void sgr_bold_off(std::ostream &os);
void sgr_faint_on(std::ostream &os);
void sgr_faint_off(std::ostream &os);
void sgr_underline_on(std::ostream &os);
void sgr_underline_off(std::ostream &os);
void sgr_fg(colour, std::ostream &);
void sgr_bg(colour, std::ostream &);
void sgr_invert(std::ostream &);
void sgr_finish(std::ostream &os);

void change_style(const style &old_style, const style &new_style, std::ostream &os);
}