#include <cutty/ansi/window.hpp>

namespace ancy = cutty::ansi;

int main()
{
    // Create a window of a given size
    ancy::window writer({40,10}, std::cout);

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

    // Unlike the raw writer, a window is double-buffered and you must call flush() to output
    writer.flush();
}
