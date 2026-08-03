#pragma once
#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

#include "ansi/common.hpp"
#include "ansi/colour.hpp"
#include "ansi/style.hpp"
#include "ansi/character.hpp"
#include "ansi/raw.hpp"
#include "ansi/writer.hpp"
#include "ansi/bitmap.hpp"
#include "ansi/window.hpp"
#include "ansi/widgets.hpp"

namespace cutty::ansi
{


struct terminal_type
{
    bool utf8;       // Allow unicode characters. false = ASCII only
    bool isatty;     // Allow cursor movement, otherwise it's a log
    bool ansi;       // Allow colours and emphasis
    bool alt_screen; // Go full screen
    colour_space cs; // Colour options supported
    size dimensions;
};

// Progress bars:
// █ full block
// ▉
// ▊
// ▋
// ▌
// ▍
// ▎
// ▏

/*
    █  Full block         U+2588
▇  Lower 7/8          U+2587
▆  Lower 6/8          U+2586
▅  Lower 5/8          U+2585
▄  Lower 4/8          U+2584
▃  Lower 3/8          U+2583
▂  Lower 2/8          U+2582
▁  Lower 1/8          U+2581
*/
std::string_view horizontal_progress_char(int progress);
std::string_view vertical_progress(int progress);

// Ultimate progress bar: A line chart





// Widgets


/*
 * What about w.add_button(p, s, "Ok", [] {...}); // 
 * add_button(w, ...
 */



} // namespace cutty::ansi
