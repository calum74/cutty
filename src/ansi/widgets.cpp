#include <cutty/ansi/widgets.hpp>

namespace ancy = cutty::ansi;

void ancy::draw_box(writer &vp, const style &s, line_style, int x, int y, int w, int h)
{
    w--;
    h--;
    if (w <= 0 || h <= 0)
    {
        return;
    }
    character top_left = {s, '+'};
    character top_right = {s, '+'};
    character bottom_left = {s, '+'};
    character bottom_right = {s, '+'};
    character horizontal = {s, '-'};
    character vertical = {s, '|'};

    vp.put(top_left, {x, y});
    vp.put(bottom_right, {x + w, y + h});
    vp.put(bottom_left, {x, y + h});
    vp.put(top_right, {x + w, y});

    for (int i = 1; i < w; ++i)
    {
        vp.put(horizontal, {x + i, y});
        vp.put(horizontal, {x + i, y + h});
    }
    for (int j = 1; j < h; ++j)
    {
        vp.put(vertical, {x, y + j});
        vp.put(vertical, {x + w, y + j});
    }
}

void ancy::draw_progress(writer &vp, const style &s, int x, int y, int w, int value, int max)
{
    character ch;
    ch.style = s;
    int N = 8 * w * value / max;
    for (int i = 0; i < w; i++, N -= 8)
    {
        if (N >= 8)
        {
            ch.ch = 0x2588; // Full block
        }
        else if (N <= 0)
        {
            ch.ch = ' ';
        }
        else
        {
            ch.ch = 0x2590 - N;
        }
        vp.put(ch, {x + i, y});
    }
}

void ancy::fill_rect(writer &w, const character &c, position p, size s)
{
    for (int y = p.y; y < p.y + s.h; ++y)
    {
        for (int x = p.x; x < p.x + s.w; ++x)
        {
            w.put(c, {x, y});
        }
    }
}

void ancy::widget::key_press(char32_t)
{
}

void ancy::widget::mouse_click(position, mouse_flags)
{
}

void ancy::widget::mouse_move(position, mouse_flags)
{
}

void ancy::widget::mouse_release(position, mouse_flags)
{
}

void ancy::widget::mouse_scroll(position, mouse_flags)
{
}

void ancy::widget::draw(writer &)
{
}

ancy::widget::widget() : m_parent{}
{
}

ancy::widget::widget(widget *parent) : m_parent{parent}
{
    m_parent->add_child(*this);
}

ancy::widget::widget(widget &parent, position p, size s) : m_parent(&parent), m_position(p), m_size(s)
{
    m_parent->add_child(*this);
}

ancy::widget::~widget()
{
    if (m_parent)
    {
        m_parent->remove_child(*this);
    }
}

void ancy::widget::add_child(widget &w)
{
}

void ancy::widget::remove_child(widget &w)
{
}

ancy::writer &ancy::widget::get_writer()
{
    if (m_parent)
    {
        return m_parent->get_writer();
    }
    else
    {
        throw std::runtime_error("No writer!");
    }
}

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

ancy::key_command::key_command(widget &parent, char32_t key, std::function<void()> fn)
    : widget(&parent), m_key(key), m_function(fn)
{
}

void ancy::key_command::key_press(char32_t key)
{
    if (key == m_key)
    {
        m_function();
    }
}

ancy::move_command::move_command(widget &parent, std::function<void(position, mouse_flags)> fn)
    : widget(&parent), m_function(fn)
{
}

void ancy::move_command::mouse_move(position p, mouse_flags f)
{
    m_function(p, f);
}

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
    character c{.style = m_focus ? m_selected : m_normal};
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
    if (k == m_key)
    {
        m_action();
    }
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
