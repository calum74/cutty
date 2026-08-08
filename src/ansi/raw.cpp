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

    if (new_style.bold != old_style.bold || new_style.faint != old_style.faint)
    {
        next();
        if (new_style.bold)
        {
            sgr_bold_on(os);
        }
        else if (new_style.faint)
        {
            sgr_faint_on(os);
        }
        else
        {
            sgr_bold_off(os);  // Normal weight
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
        if(new_style.underline)
        {
            sgr_underline_on(os);
        }
        else
        {
            sgr_underline_off(os);
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
    if (c == terminal_default)
    {
        os << "39";
    }
    else
    {
        os << "38;2;" << int(c.r) << ";" << int(c.g) << ";" << int(c.b);
    }
}

void ancy::sgr_bg(colour c, std::ostream &os)
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

void ancy::sgr_faint_on(std::ostream &os)
{
    os << '2';
}

void ancy::sgr_faint_off(std::ostream &os)
{
    os << "22";
}


void ancy::sgr_underline_on(std::ostream &os)
{
    os << '4';
}

void ancy::sgr_underline_off(std::ostream &os)
{
    os << "24";
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