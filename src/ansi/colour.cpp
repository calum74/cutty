#include <cutty/ansi/colour.hpp>

#include <iostream>

namespace ancy = cutty::ansi;

ancy::colour ancy::colour::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    return { colour_space::rgb, r, g, b };
}

void ancy::colour::sgr_fg(std::ostream &os) const
{
    if (m_cs == colour_space::normal)
    {
        os << int(30 + m_r);
    }
    else
    {
        os << "38;2;" << int(m_r) << ";" << int(m_g) << ";" << int(m_b);
    }
}

void ancy::colour::sgr_bg(std::ostream &os) const
{
    if (m_cs == colour_space::normal)
    {
        os << int(40 + m_r);
    }
    else
    {
        os << "48;2;" << int(m_r) << ";" << int(m_g) << ";" << int(m_b);
    }
}

ancy::colour ancy::colour::normal(sgr_colour c)
{
    return { colour_space::normal, static_cast<std::uint8_t>(c), 0, 0 };
}

ancy::colour::colour() : colour(normal(sgr_colour::terminal_default)) {}

ancy::colour::colour(colour_space cs, std::uint8_t r, std::uint8_t g, std::uint8_t b) : m_cs(cs), m_r(r), m_g(g), m_b(b)
{
}

bool ancy::colour::operator==(const colour &) const = default;

const ancy::colour ancy::colour::terminal_default;
const ancy::colour ancy::colour::red  = ancy::colour::normal (ancy::colour::sgr_colour::red);
const ancy::colour ancy::colour::green  = ancy::colour::normal (ancy::colour::sgr_colour::green);
const ancy::colour ancy::colour::blue  = ancy::colour::normal (ancy::colour::sgr_colour::blue);
const ancy::colour ancy::colour::white  = ancy::colour::normal (ancy::colour::sgr_colour::white);
const ancy::colour ancy::colour::black  = ancy::colour::normal (ancy::colour::sgr_colour::black);
const ancy::colour ancy::colour::cyan  = ancy::colour::normal (ancy::colour::sgr_colour::cyan);
const ancy::colour ancy::colour::magenta  = ancy::colour::normal (ancy::colour::sgr_colour::magenta);
const ancy::colour ancy::colour::yellow  = ancy::colour::normal (ancy::colour::sgr_colour::yellow);