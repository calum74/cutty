#pragma once

#include "widget.hpp"

#include <functional>
#include <string>

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
    button(widget &parent, position p, size s, char32_t hotkey, std::string text, const style &normal, const style &highlight, std::function<void()> action);

    void set_text(std::string str);
    void key_press(char32_t) override;
    void mouse_move(position p, mouse_flags m) override;
    void mouse_click(position p, mouse_flags m) override;
    virtual void draw(writer&) override;

  private:
    void set_focus(bool);

    char32_t m_key;
    style m_normal, m_selected;
    std::string m_text;
    std::function<void()> m_action;
    bool m_focus = false;
};

class text_box : public widget
{
  public:
    text_box(widget &parent, position p, size s, const style &st, const theme &t, std::string_view str);
    void set_text(std::string_view value);
    void draw(writer &w) override;

    void mouse_click(position p, mouse_flags) override;
    void mouse_move(position p, mouse_flags) override;

  private:
    style m_style, m_button_normal, m_button_focus;
    std::string m_text;
    int m_hidden;
    bool m_on_left_anchor = false, m_on_right_anchor = false;
    bool has_left_anchor() const;
    bool has_right_anchor() const;
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

class move_command : public widget
{
  public:
    move_command(widget &parent, std::function<void(position, mouse_flags)>);
    void mouse_move(position, mouse_flags) override;

  private:
    std::function<void(position, mouse_flags)> m_function;
};

class progress : public widget
{
};

class layout
{
};

} // namespace cutty::ansi