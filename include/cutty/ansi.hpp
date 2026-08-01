#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

namespace cutty::ansi
{
struct position
{
    int x=0, y=0;
};

struct size
{
    int w=0, h=0;
};

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

struct terminal_type
{
    bool utf8;       // Allow unicode characters. false = ASCII only
    bool isatty;     // Allow cursor movement, otherwise it's a log
    bool ansi;       // Allow colours and emphasis
    bool alt_screen; // Go full screen
    colour_space cs; // Colour options supported
    size dimensions;
};

static constexpr colour terminal_default{0, 0, 0, 128};
static constexpr colour white{255, 255, 255};
static constexpr colour black{0, 0, 0};
static constexpr colour red{255, 0, 0};
static constexpr colour green{0, 255, 0};
static constexpr colour blue1{0, 64, 255};

using color = colour; // Ye haw!

struct style
{
    colour fg = terminal_default;
    colour bg = terminal_default;
    bool bold = false;
    bool faint = false;
    bool underline = false;
    bool strikethrough = false;

    bool operator==(const style &) const = default;
};

struct character
{
    ansi::style style;
    char32_t ch = ' ';
};


struct rectangle
{
    ansi::position position;
    ansi::size size;
};

size get_terminal_size();

// Basic control sequences
void start_of_line(std::ostream &);
void up(std::ostream &);
void down(std::ostream &);
void left(std::ostream &);
void right(std::ostream &);
void up(int n, std::ostream &);
void down(int n, std::ostream &);
void left(int n, std::ostream &);
void right(int n, std::ostream &);
void move_to_absolute(position, std::ostream &);
void move_to_column(int x, std::ostream &);
void home(std::ostream&);

void wrap_off(std::ostream &);
void wrap_on(std::ostream &);
void cursor_hide(std::ostream &);
void cursor_show(std::ostream &);
void alternate_screen_on(std::ostream &);
void alternate_screen_off(std::ostream &);

// SGR - complete codes
void reset(std::ostream &);
void bold_on(std::ostream &);
void bold_off(std::ostream&);
void fg_colour(colour c, colour_space cs, std::ostream &os);
void bg_colour(colour c, colour_space cs, std::ostream &os);

// SGR - assemblage
void sgr_start(std::ostream &);
void sgr_next(std::ostream &);
void sgr_end(std::ostream &);
void sgr_reset(std::ostream &);
void sgr_bold_on(std::ostream &os);
void sgr_bold_off(std::ostream &os);
void sgr_fg(colour, colour_space, std::ostream &);
void sgr_bg(colour, colour_space, std::ostream &);
void sgr_finish(std::ostream &os);

void change_style(const style &old_style, const style &new_style, colour_space cs, std::ostream &os);

void write_utf8(char32_t ch, std::ostream &os);

// Braille
//    char32_t codepoint = 0x2800 + bitmap;

// ANSI writer API

class writer
{
  public:

    virtual ~writer() = default;

    virtual void reset() = 0;

    // Writes the string to the output using the current style
    virtual void text(std::string_view str) = 0;

    // Writes the string to the current position using the given style
    virtual void text(const ansi::style &s, std::string_view str) = 0;

    // Changes the current style
    virtual void apply(const ansi::style &s) = 0;

    // Moves to a specific position relative to the initial position
    virtual void go_to(position p) = 0;

    // Puts a character at the current position
    virtual void put(const character &ch) = 0;

    // Puts a character at the given position
    virtual void put(const character &ch, position p) = 0;

    // Move current position to the next line
    virtual void endl() = 0;

    // Output all buffered changes
    virtual void flush() = 0;

    // Gets the size of the terminal, or the size of the viewport
    virtual size dimensions() const = 0;
};

// Ignores all ANSI colouring and just outputs plain text
class ascii_writer : public writer
{
  public:
    ascii_writer(std::ostream &os);

  private:
    std::ostream &m_os;
};

class unbuffered_writer : public writer
{
  public:
    unbuffered_writer(std::ostream &os);

    void reset() override;

    void text(std::string_view str) override;

    // Writes the string to the current position using the given style
    void text(const ansi::style &s, std::string_view str) override;

    // Changes the current style
    void apply(const ansi::style &s) override;

    // Moves to a specific position relative to the initial position
    void go_to(position p) override;

    // Puts a character at the current position
    void put(const character &ch) override;

    // Puts a character at the given position
    void put(const character &ch, position p) override;

    // Move current position to the next line
    void endl() override;

    // Output all buffered changes
    void flush() override;

    // Gets the size of the terminal, or the size of the viewport
    size dimensions() const override;

  private:
    std::ostream &os;
    colour_space cs = cs_256;

    // The current state of the terminal
    ansi::style current_style;

    // The current x,y of the terminal relative to the initial position
    position m_position;
};

class buffered_writer : public writer
{
  public:
    buffered_writer(writer &underlying, size s);

    void put(const character &ch, position p) override;

    void flush() override;

    void reset() override;

    void text(std::string_view) override;

    void text(const style&, std::string_view) override;

    void apply(const style&) override;

    void go_to(position) override;

    void endl() override;

    void put(const character&) override;

    size dimensions() const override;

  private:
    writer &m_underlying;
    position m_position;
    style m_style;
    size m_dimensions;
    struct viewport_character : character
    {
        bool dirty = false;
    };
    std::vector<viewport_character> m_data;
    std::vector<int> m_dirty_list;
};

class window : public writer
{
public:
    window(std::ostream &os = std::cout);
private:
    unbuffered_writer m_unbuffered;
    buffered_writer m_buffered;
};

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

class bitmap
{
  public:
    bitmap(size s);

    using value_type = colour;
    colour operator[](position p) const;
    colour &operator[](position p);
    size dims() const;

  private:
    std::vector<colour> m_pixels;
    size m_size;
};

namespace widgets
{
class widget
{
  public:
    struct visitor
    {
        virtual void visit(widget &) = 0;
    };
    position m_pos;
    size m_size;
    virtual void draw(writer &vp);
    virtual void key_event();
    virtual void mouse_event();
    virtual void visit(visitor &);

    virtual size min_size();
    virtual size size_hint(size max);
};
class text : public widget
{
};
class progress : public widget
{
};

class layout
{
};

template <typename... Children> class composite : public widget
{
    std::tuple<Children...> m_children;
};

class dynamic_composite : public widget
{
    std::vector<std::unique_ptr<widget>> m_children;
};
} // namespace widgets

enum bitmap_style
{
    c_w2x1, // Two colour characters per pixel
    c_1x1,  // One colour charcter per pixel
    c_1x2,  // Two pixels per character
    c_2x2,  // 4 pixels per character
    c_2x3,  // 6 pixels per character
    c_2x4,  // 8 pixels per character
};

void draw_bitmap(writer &vp, position p, const bitmap &, colour bg, bitmap_style s);

// abstraction for drawing a progress bar on:
// std::ostream, viewport, widget, string


character pixel(colour c);
character pixel(colour c1, colour c2);
character pixel(colour c[8]);

// Application layer

enum events
{
    key_events=1,
    mouse_move_events=2,
    mouse_click_events=4,
    all = key_events | mouse_move_events | mouse_click_events
};

enum event_return
{
    continue_loop,
    exit_loop
};

class event_listener
{
    public:
        virtual ~event_listener() = 0;
        virtual event_return key(char c) = 0;
        virtual event_return mouse_drag(position, position) = 0;
        virtual event_return mouse_move(position) = 0;
};

void console_event_loop(std::istream &is, event_listener&);

} // namespace cutty::ansi
