#include <cutty/ansi/unicode.hpp>
#include <iostream>

namespace ancy = cutty::ansi;

void ancy::write_utf8(char32_t codepoint, std::ostream &os)
{
    if (codepoint >= 0 && codepoint < ' ')
    {
        os << "?";
        return;
    }

    // Written by ChatGPT

    if (codepoint <= 0x7F)
    {
        os << static_cast<char>(codepoint);
    }
    else if (codepoint <= 0x7FF)
    {
        os << static_cast<char>(0xC0 | (codepoint >> 6));
        os << static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0xFFFF)
    {
        os << static_cast<char>(0xE0 | (codepoint >> 12));
        os << static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        os << static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0x10FFFF)
    {
        os << static_cast<char>(0xF0 | (codepoint >> 18));
        os << static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        os << static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        os << static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else
    {
        throw std::invalid_argument("Invalid Unicode code point");
    }
}
