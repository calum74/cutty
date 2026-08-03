#pragma once

#include "common.hpp"
#include "colour.hpp"

#include <vector>

namespace cutty::ansi
{
class bitmap
{
  public:
    bitmap(size s);

    using value_type = colour;
    colour operator[](position p) const;
    colour &operator[](position p);
    size dims() const;

  private:
    std::vector<colour> m_pixels;
    size m_size;
};


enum bitmap_style
{
    c_w2x1, // Two colour characters per pixel
    c_1x1,  // One colour charcter per pixel
    c_1x2,  // Two pixels per character
    c_2x2,  // 4 pixels per character
    c_2x3,  // 6 pixels per character
    c_2x4,  // 8 pixels per character
};

void draw_bitmap(writer &vp, position p, const bitmap &, colour bg, bitmap_style s);

}
