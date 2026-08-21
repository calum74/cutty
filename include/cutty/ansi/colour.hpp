#pragma once
#include <cstdint>

namespace cutty::ansi
{
enum colour_space
{
    cs_disabled,  // Colour disabled completely
    cs_216,    // The 216 colour cube
    cs_rgb,     
    cs_basic  // 10 builtin colours
};

enum class sgr_colour : std::uint8_t
{
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    terminal_default
};

class colour
{
public:
    constexpr colour() : cs(cs_disabled) {}
    constexpr colour(colour_space cs) : cs(cs), r{}, g{}, b{} {}
    constexpr colour(sgr_colour c) : cs(cs_basic), r(static_cast<uint8_t>(c)), g(), b{} {}
    constexpr colour(std::uint8_t r, std::uint8_t g, std::uint8_t b) : r(r), g(g), b(b), cs(cs_rgb)
    {
    }

    bool operator==(const colour &) const = default;

    std::uint8_t r, g, b;
    colour_space cs;
};

static constexpr colour terminal_default(sgr_colour::terminal_default);
static constexpr colour white{sgr_colour::white};
static constexpr colour black{sgr_colour::black};
static constexpr colour red{sgr_colour::red};
static constexpr colour green{sgr_colour::green};
static constexpr colour blue1{0, 64, 255};
static constexpr colour yellow{sgr_colour::yellow};

using color = colour;
}
