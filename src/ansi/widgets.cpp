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

void ancy::widget::mouse_move(position p, mouse_flags)
{
    if (mouse_hit(p) && can_take_focus())
    {
        m_parent->set_focus(*this);
    }
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

void ancy::widget::grant_focus(bool)
{
}

void ancy::widget::next_focus()
{
}

void ancy::widget::prev_focus()
{
}

void ancy::widget::set_focus(widget &)
{
}

bool ancy::widget::can_take_focus()
{
    return false;
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


const ancy::theme &ancy::default_theme()
{
    static theme t{
        .window{.bg = colour::blue},
        .title{.bg = colour::blue},
        .text{.fg = colour::white, .bg = colour::blue},
        .data_text{.fg = colour::yellow, .bg = colour::black},
        .text_input{.fg = colour::white, .bg = colour::black},
        .button_normal{.fg = colour::red, .bg = colour::white},
        .button_focus{.fg = colour::red, .bg = colour::yellow, .weight = weight::heavy},
        .status{.fg = colour::blue, .bg = colour::white},
        .x_normal{.fg = colour::white, .bg = colour::red},
        .x_highlight{.fg = colour::white, .bg = colour::red, .weight = weight::heavy},
    };
    return t;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void ancy::widget::redraw()
{
    draw(get_writer());
}