#include <cutty/ansi/bitmap.hpp>
#include <cutty/ansi/character.hpp>
#include <cutty/ansi/writer.hpp>


namespace ancy = cutty::ansi;

ancy::bitmap::bitmap(size s) : m_pixels(s.w * s.h, colour{0, 0, 0}), m_size(s)
{
}

ancy::colour ancy::bitmap::operator[](position p) const
{
    return m_pixels.at(p.x + p.y * m_size.w);
}

ancy::colour &ancy::bitmap::operator[](position p)
{
    return m_pixels.at(p.x + p.y * m_size.w);
}

ancy::size ancy::bitmap::dims() const
{
    return m_size;
}

void ancy::draw_bitmap(writer &vp, position p, const bitmap &bm, colour bg, bitmap_style bs)
{
    const auto w = bm.dims().w;
    const auto h = bm.dims().h;

    if (bs == c_1x1)
    {
        character ch;
        ch.ch = ' ';
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; ++y)
            {
                ch.style.bg = bm[{x, y}];
                vp.put(ch, {p.x + x, p.y + y});
            }
        }
        return;
    }

    if (bs == c_w2x1)
    {
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < w; ++y)
            {
                character ch;
                ch.ch = ' ';
                ch.style.bg = bm[{x, y}];
                vp.put(ch, {p.x + 2 * x, p.y + y});
                vp.put(ch, {p.x + 2 * x + 1, p.y + y});
            }
        }
        return;
    }

    if (bs == c_1x2)
    {
        character ch;
        ch.ch = 0x2580; // Top half
        // Bottom half would be 0x2584
        // Full block would be 0x2588
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; y += 2)
            {
                ch.style.fg = bm[{x, y}];
                ch.style.bg = y + 1 < h ? bm[{x, y + 1}] : bg;
                vp.put(ch, {p.x + x, p.y + y / 2});
            }
        }

        return;
    }

    if (bs == c_2x4)
    {
        character ch;
        ch.ch = 0x28f8;
        ch.style.bold = true;

        vp.put(ch, {p.x, p.y});
        for (int x = 0; x < w; x += 2)
        {
            for (int y = 0; y < h; y += 4)
            {
                ch.ch++;
                vp.put(ch, {p.x + x / 2, p.y + y / 4});
            }
        }
    }
}

namespace
{
int map_1x2(int bitmap)
{
    switch (bitmap)
    {
    default:
    case 0:
        return ' ';
    case 1:
        return 0x2580;
    case 2:
        return 0x2584; // bottom half
    case 3:
        return 0x2588; // Full block
    }
}

int map_2x4(int bitmap)
{
    return 0x2800 + bitmap;
}
} // namespace
