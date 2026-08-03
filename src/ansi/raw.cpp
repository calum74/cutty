#include <cutty/ansi/raw.hpp>

#include <iostream>

namespace ancy = cutty::ansi;

void ancy::change_style(const style &old_style, const style &new_style, colour_space cs, std::ostream &os)
{
    if (new_style == old_style)
    {
        return;
    }

    bool output = false;

    auto next = [&] {
        if (output)
        {
            sgr_next(os);
        }
        else
        {
            sgr_start(os);
            output = true;
        }
    };

    if (new_style.bold != old_style.bold)
    {
        next();
        if (new_style.bold)
        {
            sgr_bold_on(os);
        }
        else
        {
            sgr_bold_off(os);
        }
    }

    if (new_style.fg != old_style.fg)
    {
        next();
        sgr_fg(new_style.fg, cs, os);
    }

    if (new_style.bg != old_style.bg)
    {
        next();
        sgr_bg(new_style.bg, cs, os);
    }

    sgr_finish(os);
}

void ancy::start_of_line(std::ostream &os)
{
    os << '\r';
}


void ancy::up(std::ostream &os)
{
    os << "\x1b[A";
}

void ancy::down(std::ostream &os)
{
    os << "\x1b[B";
}

void ancy::left(std::ostream &os)
{
    os << "\x1b[D";
}

void ancy::right(std::ostream &os)
{
    os << "\x1b[C";
}

void ancy::up(int n, std::ostream &os)
{
    os << "\x1b[" << n << "A";
}

void ancy::down(int n, std::ostream &os)
{
    os << "\x1b[" << n << "B";
}

void ancy::left(int n, std::ostream &os)
{
    os << "\x1b[" << n << "D";
}

void ancy::right(int n, std::ostream &os)
{
    os << "\x1b[" << n << "C";
}

// x = column (1-based), y = row (1-based)
void ancy::move_to_absolute(position p, std::ostream &os)
{
    // ?? Is this the right way round ??
    os << "\x1b[" << p.y << ";" << p.x << "H";
}

// x = column (1-based)
void ancy::move_to_column(int x, std::ostream &os)
{
    os << "\x1b[" << x << "G";
}

void ancy::home(std::ostream &os)
{
    os << "\x1b[H";
}

void ancy::bold_on(std::ostream &os)
{
    os << "\x1b[1m";
}

void ancy::bold_off(std::ostream &os)
{
    os << "\x1b[22m";
}

void ancy::reset(std::ostream &os)
{
    os << "\x1b[0m";
}

void ancy::fg_colour(colour c, colour_space cs, std::ostream &os)
{
    sgr_start(os);
    sgr_fg(c, cs, os);
    sgr_finish(os);
}

void ancy::bg_colour(colour c, colour_space cs, std::ostream &os)
{
    sgr_start(os);
    sgr_bg(c, cs, os);
    sgr_finish(os);
}

void ancy::sgr_reset(std::ostream &os)
{
    os << '0';
}

void ancy::sgr_finish(std::ostream &os)
{
    os << 'm';
}

void ancy::sgr_fg(colour c, colour_space cs, std::ostream &os)
{
    if (c == terminal_default)
    {
        os << "39";
    }
    else
    {
        os << "38;2;" << int(c.r) << ";" << int(c.g) << ";" << int(c.b);
    }
}

void ancy::sgr_bg(colour c, colour_space cs, std::ostream &os)
{
    if (c == terminal_default)
    {
        os << "49";
    }
    else
    {
        os << "48;2;" << int(c.r) << ";" << int(c.g) << ";" << int(c.b);
    }
}

void ancy::sgr_bold_on(std::ostream &os)
{
    os << '1';
}

void ancy::sgr_bold_off(std::ostream &os)
{
    os << "22";
}

void ancy::sgr_start(std::ostream &os)
{
    os << "\x1b[";
}

void ancy::sgr_next(std::ostream &os)
{
    os << ';';
}

void ancy::wrap_off(std::ostream &os)
{
    os << "\x1b[?7l";
}

void ancy::wrap_on(std::ostream &os)
{
    os << "\x1b[?7h";
}

void ancy::cursor_hide(std::ostream &os)
{
    os << "\x1b[?25l";
}

void ancy::cursor_show(std::ostream &os)
{
    os << "\x1b[?25h";
}

void ancy::alternate_screen_on(std::ostream &os)
{
    os << "\x1b[?1049h";
}

void ancy::alternate_screen_off(std::ostream &os)
{
    os << "\x1b[?1049l";
}

