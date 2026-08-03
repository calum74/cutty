#pragma once
#include <cstdint>

namespace cutty::ansi
{
struct colour
{
    std::uint8_t r, g, b, x = 0;
    bool operator==(const colour &) const = default;
};

enum colour_space
{
    cs_mono,
    cs_666,
    cs_256
};

static constexpr colour terminal_default{0, 0, 0, 128};
static constexpr colour white{255, 255, 255};
static constexpr colour black{0, 0, 0};
static constexpr colour red{255, 0, 0};
static constexpr colour green{0, 255, 0};
static constexpr colour blue1{0, 64, 255};

using color = colour; // Ye haw!
}
