#include <cutty/ansi/widgets.hpp>

namespace ancy = cutty::ansi;

ancy::text_box::text_box(widget &parent, position p, size s, const style &st, const theme &t, std::string_view text)
    : widget(parent, p, s), m_style(st), m_button_normal(t.button_normal), m_button_focus(t.button_focus)
{
    set_text(text);
}

bool ancy::text_box::has_left_anchor() const
{
    return m_hidden > 0;
}

bool ancy::text_box::has_right_anchor() const
{
    return m_text.size() > m_size.w + m_hidden;
}

void ancy::text_box::draw(writer &w)
{
    character ch{.style = m_style};
    for (int i = 0; i < m_size.w; ++i)
    {
        int j = i + m_hidden;
        if (i == 0 && has_left_anchor())
        {
            ch.style = m_on_left_anchor ? m_button_focus : m_button_normal;
            ch.ch = '<';
        }
        else if (i == m_size.w - 1 && has_right_anchor())
        {
            ch.style = m_on_right_anchor ? m_button_focus : m_button_normal;
            ch.ch = '>';
        }
        else if (j >= 0 && j < m_text.size())
        {
            ch.style = m_style;
            ch.ch = m_text[j];
        }
        else
        {
            ch.style = m_style;
            ch.ch = ' ';
        }
        w.put(ch, {m_position.x + i, m_position.y});
    }
}

void ancy::text_box::mouse_click(position p, mouse_flags)
{
    auto right = m_position;
    right.x += m_size.w - 1;

    if (p == right && has_right_anchor())
    {
        // set_text("abc");
        // return;
        m_hidden += m_size.w - 2;
        draw(get_writer());
    }
    else if (p == m_position && has_left_anchor())
    {
        m_hidden -= m_size.w - 2;
        if (m_hidden < 0)
            m_hidden = 0; // ???
        draw(get_writer());
    }
}

void ancy::text_box::mouse_move(position p, mouse_flags)
{
    auto right = m_position;
    right.x += m_size.w - 1;

    if ((p == m_position) != m_on_left_anchor)
    {
        m_on_left_anchor = p == m_position;
        if (has_left_anchor())
        {
            draw(get_writer()); // !!! Need a "redraw" method
        }
    }
    if ((p == right) != m_on_right_anchor)
    {
        m_on_right_anchor = p == right;
        if (has_right_anchor())
        {
            draw(get_writer()); // !!! Need a "redraw" method
        }
    }
}

void ancy::text_box::set_text(std::string_view s)
{
    m_text = s;
    m_hidden = 0;
    draw(get_writer());
}

