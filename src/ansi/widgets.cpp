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

void ancy::widget::mouse_click(position)
{
}

void ancy::widget::mouse_move(position)
{
}

void ancy::widget::mouse_release(position)
{
}

void ancy::widget::draw(writer&)
{
}

ancy::widget::widget() : m_parent{}
{}

ancy::widget::widget(widget &parent) : m_parent(&parent)
{
    m_parent->add_child(*this);
}

ancy::widget::~widget()
{
    if(m_parent)
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

ancy::text_box::text_box(widget &parent, position p, size s, const style& sl, std::string_view text) : widget(parent)
{
}

void ancy::text_box::draw(writer &)
{
}
