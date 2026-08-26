#include <cutty/ansi/writer.hpp>

namespace ancy = cutty::ansi;

int main()
{
    // A raw writer creates a drawing area on the terminal, and keeps track of the current position and style
    ancy::raw_writer writer(std::cout);

    // Add some whitespace to draw into
    writer.endl();
    writer.endl();
    writer.endl();
    writer.endl();

    // Define some styles
    ancy::style s1 { .fg = ancy::colour::white, .bg = ancy::colour::red, .weight = ancy::weight::heavy };
    ancy::style s2 { .fg = ancy::colour::yellow, .bg = ancy::colour::blue, .weight = ancy::weight::light };

    // Move cursor relative to top-left of the drawing area
    writer.go_to({5, 2});

    // Output some text in style s1
    writer.text(s1, "Salut!");

    // A character to write, including its style
    ancy::character c1 { .style = s2, .ch = '*' };

    // Draw the character at the given locations
    writer.put(c1, {1,1});
    writer.put(c1, {10,1});
    writer.put(c1, {20,2});

    // Position the cursor somewhere sensible for when we exit
    writer.go_to({0,4});
}
