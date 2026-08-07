#pragma once

namespace cutty::ansi
{
struct position
{
    int x = 0, y = 0;
    friend bool operator==(position, position) = default;
};

struct size
{
    int w = 0, h = 0;
};

position operator+(position p1, position p2);
position operator-(position p1, position p2);

struct rectangle
{
    ansi::position position;
    ansi::size size;
};

struct colour;
class writer;
struct character;
struct style;

} // namespace cutty::ansi
