#pragma once
#include <cstdint>
#include <iosfwd>

namespace cutty::ansi
{
/// A text foreground or background colour
class colour
{
public:

    /// The different SGR colour spaces
    enum class colour_space : std::uint8_t
    {
        normal,  // 10 builtin colours
        bright,  // 10 builtin colours (bright version)
        grey,    // 16 levels of greyscale
        cube,    // The 216 colour cube
        rgb      // 256 levels of RGB
    };

    /// One of the built in SGR colours
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


    /// Terminal-default colour
    colour ();

    static colour normal(sgr_colour c);

    static colour bright(sgr_colour c);

    static colour grey (std::uint8_t level);

    static colour six_cube(std::uint8_t r, std::uint8_t g, std::uint8_t b);

    static colour rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);

    colour to_six_cube() const;
    colour to_greyscale() const;

    bool operator==(const colour &) const;

    void sgr_fg (std::ostream &os) const;
    void sgr_bg (std::ostream &os) const;

    static const colour terminal_default;
    static const colour white;
    static const colour black;
    static const colour red;
    static const colour green;
    static const colour blue;
    static const colour yellow;
    static const colour magenta;
    static const colour cyan;

private:
    colour(colour_space cs, std::uint8_t r, std::uint8_t g, std::uint8_t b);
    colour_space m_cs;
    std::uint8_t m_r, m_g, m_b;
};

using color = colour;
}
