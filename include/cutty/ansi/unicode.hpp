#pragma once

#include "common.hpp"

#include <iosfwd>

namespace cutty::ansi
{

// Braille
//    char32_t codepoint = 0x2800 + bitmap;

    void write_utf8(char32_t ch, std::ostream &os);

    character pixel(colour c);
    character pixel(colour c1, colour c2);
    character pixel(colour c[8]);



}
