#include <cutty/ansi/raw.hpp>

#include <iostream>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace ancy = cutty::ansi;

void ancy::change_style(const style &old_style, const style &new_style, std::ostream &os)
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

    if (new_style.weight != old_style.weight)
    {
        if (new_style.weight == weight::heavy)
        {
            if (old_style.weight != weight::normal)
            {
                // Strangely, you need to cancel "light weight"
                next();
                sgr_apply(sgr_style::normal_weight, os);
            }
            next();
            sgr_apply(sgr_style::heavy_weight, os);
        }
        else if (new_style.weight == weight::light)
        {
            if (old_style.weight != weight::normal)
            {
                // Strangely, you need to cancel the previous weight
                next();
                sgr_apply(sgr_style::normal_weight, os);
            }
            next();
            sgr_apply(sgr_style::light_weight, os);
        }
        else
        {
            next();
            sgr_apply(sgr_style::normal_weight, os);
        }
    }

    if (new_style.fg != old_style.fg)
    {
        next();
        sgr_fg(new_style.fg, os);
    }

    if (new_style.bg != old_style.bg)
    {
        next();
        sgr_bg(new_style.bg, os);
    }

    if(new_style.underline != old_style.underline)
    {
        next();
        if(new_style.underline == underline::single_line)
        {
            sgr_apply(sgr_style::underline, os);
        }
        else if(new_style.underline == underline::double_line)
        {
            sgr_apply(sgr_style::double_underline, os);
        }
        else
        {
            sgr_apply(sgr_style::no_underline, os);
        }
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


void ancy::apply(sgr_style s, std::ostream &os)
{
    sgr_start(os);
    sgr_apply(s, os);
    sgr_finish(os);
}

void ancy::reset(std::ostream &os)
{
    apply(sgr_style::reset, os);
}

void ancy::fg_colour(colour c, std::ostream &os)
{
    sgr_start(os);
    sgr_fg(c, os);
    sgr_finish(os);
}

void ancy::bg_colour(colour c, std::ostream &os)
{
    sgr_start(os);
    sgr_bg(c, os);
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

void ancy::sgr_fg(colour c, std::ostream &os)
{
    c.sgr_fg(os);
}

void ancy::sgr_bg(colour c, std::ostream &os)
{
    c.sgr_bg(os);
}

void ancy::sgr_apply(sgr_style s, std::ostream &os)
{
    os << static_cast<int>(s);
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

ancy::size ancy::get_terminal_size()
{
    winsize ws{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        throw std::runtime_error("Failed to get terminal size");

    return {static_cast<int>(ws.ws_col), static_cast<int>(ws.ws_row)};
}

ancy::position ancy::read_position(std::ostream &os, std::istream &is)
{
    std::cout << std::flush << "\x1b[6n" << std::flush;
    // Response:
    // ESC [ row ; column R

    char c;
    int n = 0;
    int row = 0;
    std::string debug;
    while (read(STDIN_FILENO, &c, 1) == 1)  // TODO: Use `is`
    {
        debug += c;
        if (std::isdigit(c))
        {
            n = n * 10 + c - '0';
        }
        else if (c == ';')
        {
            row = n;
            n = 0;
        }
        else if (c == 'R')
        {
            return {n, row};
        }
        else if (c == 27 || c == '[')
        {
            // ok
        }
        else
        {
            // Failure
            return {0, 0};
        }
    }
    return {0, 0};
}

ancy::position ancy::operator-(position lhs, position rhs)
{
    return {lhs.x-rhs.x, lhs.y-rhs.y};
}

ancy::position ancy::operator+(position lhs, position rhs)
{
    return {lhs.x+rhs.x, lhs.y+rhs.y};
}
