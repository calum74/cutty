#include <cutty/ansi/widgets.hpp>

namespace ancy = cutty::ansi;

ancy::button::button(widget &parent, position p, size s, char32_t key, std::string text, const style &normal,
                     const style &selected, std::function<void()> action)
    : widget(parent, p, s), m_key(key), m_normal(normal), m_selected(selected), m_action(action)
{
    set_text(text);
}

void ancy::button::set_text(std::string text)
{
    m_text = std::move(text);
    draw(get_writer());
}

void ancy::button::draw(writer &w)
{
    int left_padding = (m_size.w - m_text.size()) / 2;
    character c{.style = (m_focus || m_key_focus )? m_selected : m_normal};
    for (int j = 0; j < m_size.h; ++j)
    {
        for (int i = 0; i < m_size.w; ++i)
        {
            int k = i - left_padding;
            c.ch = k >= 0 && k < m_text.size() ? m_text[k] : ' ';
            c.style.underline = std::tolower(c.ch) == m_key;
            w.put(c, {m_position.x + i, m_position.y + j});
        }
    }
}

void ancy::button::key_press(char32_t k)
{
    if (k == m_key || (k == '\n' && m_key_focus))
    {
        m_action();
    }
}

bool ancy::button::can_take_focus()
{
    return true;
}
void ancy::button::grant_focus(bool f)
{
    m_key_focus = f;
    draw(get_writer());
}

void ancy::button::mouse_move(position p, mouse_flags m)
{
    set_focus(mouse_hit(p));
}

bool ancy::widget::mouse_hit(position p) const
{
    return p.x >= m_position.x && p.x < m_position.x + m_size.w && p.y >= m_position.y && p.y < m_position.y + m_size.h;
}

void ancy::button::mouse_click(position p, mouse_flags m)
{
    if (mouse_hit(p))
    {
        m_action();
    }
}

void ancy::button::set_focus(bool f)
{
    if (f != m_focus)
    {
        m_focus = f;
        draw(get_writer());
    }
}
