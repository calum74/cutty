#pragma once

#include "widget.hpp"

#include <string>
#include <functional>

namespace cutty::ansi
{

// Not quite sure where these go yet...

enum class line_style
{
    none,    // Just blank spaces or colour
    ascii,   // + - |
    solid,   //
    double_, // ╠════╦════╣ ║    ║    ║ ╠════╬════╣ ║    ║    ║  ╚════╩════╝
    light,   // ┌ ┐ └ ┘ ─ │ ├ ┤ ┬ ┴ ┼
    heavy,   // ┏ ┓ ┗ ┛ ━ ┃ ┣ ┫ ┳ ┻ ╋
    rounded, // ╭ ╮ ╰ ╯ ─ │ ├ ┤ ┬ ┴ ┼
    half     //
};

void draw_box(writer &vp, const style &s, line_style, int x, int y, int w, int h);
void draw_progress(writer &vp, const style &s, int x, int y, int w, int value, int max);

enum class valign
{
    left,
    center,
    right
};
enum class halign
{
    top,
    middle,
    bottom
};

void draw_text(writer &vp, const style &s, std::string_view text, const rectangle &, valign, halign);

// abstraction for drawing a progress bar on:
// std::ostream, viewport, widget, string

void fill_rect(writer &w, const character &c, position p, size s);

class button : public widget
{
  public:
    button(position p, size s, std::string text);

    virtual void on_focus(writer &w);
    // virtual void on_mouse_move(writer &w, mouse_move m);

    virtual void on_click() = 0;
};

class text_box : public widget
{
  public:
    text_box(widget &parent, position p, size s, const style &sl, std::string_view str);
    void set_text(std::string_view value);
    void draw(writer &w) override;

  private:
    style m_style;
    std::string m_text;
};

class key_command : public widget
{
public:
  key_command(widget &parent, char32_t key, std::function<void()> cmd);
  void key_press(char32_t key) override;
private:
  char32_t m_key;
  std::function<void()> m_function;
};

class progress : public widget
{
};

class layout
{
};

} // namespace cutty::ansi