#include <cutty/ansi/raw.hpp>
#include <cutty/ansi/writer.hpp>
#include <cutty/ansi/unicode.hpp>

namespace ancy = cutty::ansi;

ancy::raw_writer::raw_writer(std::ostream &os) : os(os), m_position(0, 0)
{
}

void ancy::raw_writer::text(std::string_view sv)
{
    // TODO: Check SV for special characters
    os << sv;
    m_position.x += sv.size();
}

void ancy::raw_writer::endl()
{
    os << '\n';
    m_position.x = 0;
    m_position.y++;
}

void ancy::raw_writer::reset()
{
    current_style = {};
    ansi::reset(os);
}

void ancy::raw_writer::go_to(position new_position)
{
    if (new_position.x < m_position.x)
    {
        ansi::left(m_position.x - new_position.x, os);
    }
    else if (new_position.x > m_position.x)
    {
        ansi::right(new_position.x - m_position.x, os);
    }
    if (new_position.y < m_position.y)
    {
        ansi::up(m_position.y - new_position.y, os);
    }
    else if (new_position.y > m_position.y)
    {
        ansi::down(new_position.y - m_position.y, os);
    }
    m_position = new_position;
}

void ancy::raw_writer::apply(const ansi::style &new_style)
{
    change_style(current_style, new_style, os);
    current_style = new_style;
}

void ancy::raw_writer::put(const character &ch)
{
    m_position.x++;
    apply(ch.style);
    write_utf8(ch.ch, os);
}

void ancy::raw_writer::flush()
{
    os.flush();
}

ancy::size ancy::raw_writer::dimensions() const
{
    return get_terminal_size();
}


void ancy::raw_writer::put(const character &ch, position p)
{
    go_to(p);
    put(ch);
}

void ancy::raw_writer::text(const style &s, std::string_view str)
{
    apply(s);
    text(str);
}

ancy::position ancy::raw_writer::current_position() const
{
    return m_position;
}

void ancy::raw_writer::hide_cursor()
{
    ancy::cursor_hide(os);
}

void ancy::raw_writer::show_cursor(position p)
{
    go_to(p);
    ancy::cursor_show(os);
}
