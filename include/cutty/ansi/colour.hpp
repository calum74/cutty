#pragma once
#include <cstdint>


namespace cutty::ansi
{
enum colour_space
{
    cs_terminal_default,
    cs_mono,
    cs_666,
    cs_256
};

struct colour
{
    std::uint8_t r, g, b;
    colour_space cs = cs_256;
    bool operator==(const colour &) const = default;
};

static constexpr colour terminal_default{0, 0, 0, cs_terminal_default};
static constexpr colour white{255, 255, 255};
static constexpr colour black{0, 0, 0};
static constexpr colour red{255, 0, 0};
static constexpr colour green{0, 255, 0};
static constexpr colour blue1{0, 64, 255};
static constexpr colour yellow{255,255, 0};

using color = colour; // Ye haw!
}
