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

const ancy::theme &ancy::default_theme()
{
    static theme t{
        .window{.bg = blue1},
        .title{.bg = blue1},
        .text{.fg = white, .bg = blue1},
        .data_text{.fg = yellow, .bg = black},
        .text_input{.fg = white, .bg = black},
        .button_normal{.fg = red, .bg = white},
        .button_focus{.fg = red, .bg = yellow, .bold = true},
        .status{.fg = blue1, .bg = white},
        .x_normal{.fg = ancy::white, .bg = ancy::red},
        .x_highlight{.fg = ancy::white, .bg = ancy::red, .bold = true},
    };
    return t;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

ancy::text_input::text_input(widget &parent, position p, size s, const style &text_style,
                             const style &button_normal_style, const style &button_focus_style,
                             const style &disabled_style, std::string initial_text, std::string prompt_text,
                             std::function<void()> change_action,
                             std::function<void()> enter_action)
    : widget(parent, p, s), m_text_style(text_style), m_button_normal_style(button_normal_style),
      m_button_focus_style(button_focus_style), m_disabled_style(disabled_style), m_prompt(std::move(prompt_text)),
      m_change_action(std::move(change_action)),
      m_enter_action(std::move(enter_action))
{
    set_text(std::move(initial_text));
}

void ancy::text_input::set_text(std::string text)
{
    m_text = text;
    draw(get_writer());
}

void ancy::text_input::draw(writer&w)
{
    const auto & str = m_text.empty() ? m_prompt: m_text;
    const auto & style = m_text.empty() ? m_disabled_style : m_text_style;

    character ch { .style = style };

    bool overflow = m_text.size() > (m_size.w);
    int offset = overflow ? m_text.size() - (m_size.w) : 0;

    for(int i=0; i<m_size.w; ++i)
    {
        int j=i + offset;

        if(overflow && i==0)
        {
            ch.ch = '<';
        }
        else if(j>=0 && j < str.size())
        {
            ch.ch = str[j];
        }
        else
        {
            ch.ch = ' ';
        }
        w.put(ch, {m_position.x + i, m_position.y});
    }
    // if(m_has_focus)
    {
        w.show_cursor({m_position.x + int(m_text.size()) - offset, m_position.y});
    }
}

void ancy::text_input::mouse_move(position, mouse_flags)
{
}

void ancy::text_input::mouse_click(position, mouse_flags)
{
}

void ancy::text_input::key_press(char32_t key)
{
    if(key == 127)
    {
        if(!m_text.empty())
        {
            m_text.pop_back();
            m_change_action();
        }
    }
    else if(key==13)
    {
        // Enter key: perform action
        m_enter_action();
    }
    else if(key <127)
    {
        m_text += key;
        m_change_action();
    }
    else
    {
        // Ignore?
        return;
    }
    draw(get_writer());
}

const std::string &ancy::text_input::get_text() const
{
    return m_text;
}
