#include <cutty/ansi.hpp>

#include <algorithm>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace cy = cutty;

cy::ansi::raw_writer::raw_writer(std::ostream &os) : os(os), m_position(0, 0)
{
}

void cy::ansi::raw_writer::text(std::string_view sv)
{
    // TODO: Check SV for special characters
    os << sv;
    m_position.x += sv.size();
}

void cy::ansi::raw_writer::endl()
{
    os << '\n';
    m_position.x = 0;
    m_position.y++;
}

void cy::ansi::raw_writer::reset()
{
    current_style = {};
    ansi::reset(os);
}

void cy::ansi::raw_writer::go_to(position new_position)
{
    if (new_position.x < m_position.x)
    {
        ansi::left(m_position.x - new_position.x, os);
    }
    else if (new_position.x > m_position.x)
    {
        ansi::right(new_position.x - m_position.x, os);
    }
    if (new_position.y < m_position.y)
    {
        ansi::up(m_position.y - new_position.y, os);
    }
    else if (new_position.y > m_position.y)
    {
        ansi::down(new_position.y - m_position.y, os);
    }
    m_position = new_position;
}

void cy::ansi::raw_writer::apply(const ansi::style &new_style)
{
    change_style(current_style, new_style, cs, os);
    current_style = new_style;
}

void cy::ansi::change_style(const style &old_style, const style &new_style, colour_space cs, std::ostream &os)
{
    if (new_style == old_style)
    {
        return;
    }

    bool output = false;

    auto next = [&] {
        if (output)
        {
            sgr_next(os);
        }
        else
        {
            sgr_start(os);
            output = true;
        }
    };

    if (new_style.bold != old_style.bold)
    {
        next();
        if (new_style.bold)
        {
            sgr_bold_on(os);
        }
        else
        {
            sgr_bold_off(os);
        }
    }

    if (new_style.fg != old_style.fg)
    {
        next();
        sgr_fg(new_style.fg, cs, os);
    }

    if (new_style.bg != old_style.bg)
    {
        next();
        sgr_bg(new_style.bg, cs, os);
    }

    sgr_finish(os);
}

void cutty::ansi::start_of_line(std::ostream &os)
{
    os << '\r';
}

void cutty::ansi::up(std::ostream &os)
{
    os << "\x1b[A";
}

void cutty::ansi::down(std::ostream &os)
{
    os << "\x1b[B";
}

void cutty::ansi::left(std::ostream &os)
{
    os << "\x1b[D";
}

void cutty::ansi::right(std::ostream &os)
{
    os << "\x1b[C";
}

void cutty::ansi::up(int n, std::ostream &os)
{
    os << "\x1b[" << n << "A";
}

void cutty::ansi::down(int n, std::ostream &os)
{
    os << "\x1b[" << n << "B";
}

void cutty::ansi::left(int n, std::ostream &os)
{
    os << "\x1b[" << n << "D";
}

void cutty::ansi::right(int n, std::ostream &os)
{
    os << "\x1b[" << n << "C";
}

// x = column (1-based), y = row (1-based)
void cutty::ansi::move_to_absolute(position p, std::ostream &os)
{
    // ?? Is this the right way round ??
    os << "\x1b[" << p.y << ";" << p.x << "H";
}

// x = column (1-based)
void cutty::ansi::move_to_column(int x, std::ostream &os)
{
    os << "\x1b[" << x << "G";
}

void cutty::ansi::home(std::ostream &os)
{
    os << "\x1b[H";
}

void cutty::ansi::bold_on(std::ostream &os)
{
    os << "\x1b[1m";
}

void cutty::ansi::bold_off(std::ostream &os)
{
    os << "\x1b[22m";
}

void cutty::ansi::reset(std::ostream &os)
{
    os << "\x1b[0m";
}

void cy::ansi::fg_colour(colour c, colour_space cs, std::ostream &os)
{
    sgr_start(os);
    sgr_fg(c, cs, os);
    sgr_finish(os);
}

void cy::ansi::bg_colour(colour c, colour_space cs, std::ostream &os)
{
    sgr_start(os);
    sgr_bg(c, cs, os);
    sgr_finish(os);
}

void cy::ansi::sgr_reset(std::ostream &os)
{
    os << '0';
}

void cy::ansi::sgr_finish(std::ostream &os)
{
    os << 'm';
}

void cy::ansi::sgr_fg(colour c, colour_space cs, std::ostream &os)
{
    if (c == terminal_default)
    {
        os << "39";
    }
    else
    {
        os << "38;2;" << int(c.r) << ";" << int(c.g) << ";" << int(c.b);
    }
}

void cy::ansi::sgr_bg(colour c, colour_space cs, std::ostream &os)
{
    if (c == terminal_default)
    {
        os << "49";
    }
    else
    {
        os << "48;2;" << int(c.r) << ";" << int(c.g) << ";" << int(c.b);
    }
}

void cy::ansi::sgr_bold_on(std::ostream &os)
{
    os << '1';
}

void cy::ansi::sgr_bold_off(std::ostream &os)
{
    os << "22";
}

void cy::ansi::sgr_start(std::ostream &os)
{
    os << "\x1b[";
}

void cy::ansi::sgr_next(std::ostream &os)
{
    os << ';';
}

void cy::ansi::wrap_off(std::ostream &os)
{
    os << "\x1b[?7l";
}

void cy::ansi::wrap_on(std::ostream &os)
{
    os << "\x1b[?7h";
}

void cy::ansi::cursor_hide(std::ostream &os)
{
    os << "\x1b[?25l";
}

void cy::ansi::cursor_show(std::ostream &os)
{
    os << "\x1b[?25h";
}

void cy::ansi::alternate_screen_on(std::ostream &os)
{
    os << "\x1b[?1049h";
}

void cy::ansi::alternate_screen_off(std::ostream &os)
{
    os << "\x1b[?1049l";
}

cy::ansi::window::window(size s, std::ostream &os) : m_underlying(os), m_dimensions(s), m_data(s.w * s.h)
{
    m_dirty_list.reserve(m_data.size());
    for (int i = 0; i < s.h; i++)
    {
        m_underlying.endl();
    }
}

cy::ansi::window::~window()
{
    flush();
}

void cy::ansi::window::flush()
{
    std::sort(m_dirty_list.begin(), m_dirty_list.end());
    for (auto index : m_dirty_list)
    {
        auto &c = m_data[index];
        m_underlying.go_to({index % m_dimensions.w, index / m_dimensions.w});
        m_underlying.put(c);
        c.dirty = false;
    }
    m_underlying.reset();
    m_underlying.go_to({0, m_dimensions.h});
    m_underlying.flush();
    m_dirty_list.clear();
}

namespace
{
// TODO: Streaming version
std::string utf8_encode(char32_t codepoint)
{
    if (codepoint >= 0 && codepoint < ' ')
    {
        return "?";
    }

    std::string s;

    if (codepoint <= 0x7F)
    {
        s.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FF)
    {
        s.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
        s.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else if (codepoint <= 0xFFFF)
    {
        s.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
        s.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        s.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else if (codepoint <= 0x10FFFF)
    {
        s.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
        s.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        s.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        s.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else
    {
        throw std::invalid_argument("Invalid Unicode code point");
    }

    return s;
}
} // namespace

void cy::ansi::raw_writer::put(const character &ch)
{
    m_position.x++;
    apply(ch.style);
    os << utf8_encode(ch.ch);
}

void cy::ansi::draw_box(writer &vp, const style &s, line_style, int x, int y, int w, int h)
{
    w--;
    h--;
    if (w <= 0 || h <= 0)
    {
        return;
    }
    character top_left = {s, '+'};
    character top_right = {s, '+'};
    character bottom_left = {s, '+'};
    character bottom_right = {s, '+'};
    character horizontal = {s, '-'};
    character vertical = {s, '|'};

    vp.put(top_left, {x, y});
    vp.put(bottom_right, {x + w, y + h});
    vp.put(bottom_left, {x, y + h});
    vp.put(top_right, {x + w, y});

    for (int i = 1; i < w; ++i)
    {
        vp.put(horizontal, {x + i, y});
        vp.put(horizontal, {x + i, y + h});
    }
    for (int j = 1; j < h; ++j)
    {
        vp.put(vertical, {x, y + j});
        vp.put(vertical, {x + w, y + j});
    }
}

void cy::ansi::window::put(const character &ch, position p)
{
    if (p.x < 0 || p.x >= m_dimensions.w || p.y < 0 || p.y >= m_dimensions.h)
    {
        return;
    }

    auto index = p.x + p.y * m_dimensions.w;
    auto &my_char = m_data.at(index);
    if (ch.ch != my_char.ch || ch.style != my_char.style)
    {
        my_char.style = ch.style;
        my_char.ch = ch.ch;
        if (!my_char.dirty)
        {
            m_dirty_list.push_back(index);
            my_char.dirty = true;
        }
    }
}

void cy::ansi::draw_progress(writer &vp, const style &s, int x, int y, int w, int value, int max)
{
    character ch;
    ch.style = s;
    int N = 8 * w * value / max;
    for (int i = 0; i < w; i++, N -= 8)
    {
        if (N >= 8)
        {
            ch.ch = 0x2588; // Full block
        }
        else if (N <= 0)
        {
            ch.ch = ' ';
        }
        else
        {
            ch.ch = 0x2590 - N;
        }
        vp.put(ch, {x + i, y});
    }
}

cy::ansi::bitmap::bitmap(size s) : m_pixels(s.w * s.h, colour{0, 0, 0}), m_size(s)
{
}

cy::ansi::colour cy::ansi::bitmap::operator[](position p) const
{
    return m_pixels.at(p.x + p.y * m_size.w);
}

cy::ansi::colour &cy::ansi::bitmap::operator[](position p)
{
    return m_pixels.at(p.x + p.y * m_size.w);
}

cy::ansi::size cy::ansi::bitmap::dims() const
{
    return m_size;
}

void cy::ansi::draw_bitmap(writer &vp, position p, const bitmap &bm, colour bg, bitmap_style bs)
{
    const auto w = bm.dims().w;
    const auto h = bm.dims().h;

    if (bs == c_1x1)
    {
        character ch;
        ch.ch = ' ';
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; ++y)
            {
                ch.style.bg = bm[{x, y}];
                vp.put(ch, {p.x + x, p.y + y});
            }
        }
        return;
    }

    if (bs == c_w2x1)
    {
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < w; ++y)
            {
                character ch;
                ch.ch = ' ';
                ch.style.bg = bm[{x, y}];
                vp.put(ch, {p.x + 2 * x, p.y + y});
                vp.put(ch, {p.x + 2 * x + 1, p.y + y});
            }
        }
        return;
    }

    if (bs == c_1x2)
    {
        character ch;
        ch.ch = 0x2580; // Top half
        // Bottom half would be 0x2584
        // Full block would be 0x2588
        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; y += 2)
            {
                ch.style.fg = bm[{x, y}];
                ch.style.bg = y + 1 < h ? bm[{x, y + 1}] : bg;
                vp.put(ch, {p.x + x, p.y + y / 2});
            }
        }

        return;
    }

    if (bs == c_2x4)
    {
        character ch;
        ch.ch = 0x28f8;
        ch.style.bold = true;

        vp.put(ch, {p.x, p.y});
        for (int x = 0; x < w; x += 2)
        {
            for (int y = 0; y < h; y += 4)
            {
                ch.ch++;
                vp.put(ch, {p.x + x / 2, p.y + y / 4});
            }
        }
    }
}

namespace
{
int map_1x2(int bitmap)
{
    switch (bitmap)
    {
    default:
    case 0:
        return ' ';
    case 1:
        return 0x2580;
    case 2:
        return 0x2584; // bottom half
    case 3:
        return 0x2588; // Full block
    }
}

int map_2x4(int bitmap)
{
    return 0x2800 + bitmap;
}
} // namespace

cy::ansi::size cy::ansi::get_terminal_size()
{
    winsize ws{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        throw std::runtime_error("Failed to get terminal size");

    return {static_cast<int>(ws.ws_col), static_cast<int>(ws.ws_row)};
}

cy::ansi::size cy::ansi::window::dimensions() const
{
    return m_dimensions;
}

void cy::ansi::window::put(const character &c)
{
    put(c, m_position);
    ++m_position.x;
}

void cy::ansi::window::endl()
{
    m_position.y++;
    m_position.x = 0;
}

void cy::ansi::window::text(std::string_view sv)
{
    text(m_style, sv);
}

void cy::ansi::window::text(const style &s, std::string_view sv)
{
    character ch{.style = s};
    for (auto c : sv)
    {
        ch.ch = c;
        put(ch);
    }
}

void cy::ansi::window::apply(const style &s)
{
    m_style = s;
}

void cy::ansi::window::go_to(position p)
{
    m_position = p;
    m_underlying.go_to(p);
}

void cy::ansi::window::reset()
{
    m_style = {};
}

void cy::ansi::raw_writer::put(const character &ch, position p)
{
    go_to(p);
    put(ch);
}

void cy::ansi::raw_writer::text(const style &s, std::string_view str)
{
    apply(s);
    text(str);
}

void cy::ansi::raw_writer::flush()
{
    os.flush();
}

cy::ansi::size cy::ansi::raw_writer::dimensions() const
{
    return get_terminal_size();
}

void cy::ansi::fill_rect(writer &w, const character &c, position p, size s)
{
    for (int y = p.y; y < p.y + s.h; ++y)
    {
        for (int x = p.x; x < p.x + s.w; ++x)
        {
            w.put(c, {x, y});
        }
    }
}

namespace
{
class RawEventsSetup
{
  public:
    RawEventsSetup(std::ostream &os) : m_os(os)
    {
        tcgetattr(STDIN_FILENO, &m_old);

        termios raw = m_old;
        cfmakeraw(&raw);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        m_os << "\x1b[?1003h"
             << "\x1b[?1006h" << std::flush;
    }

    ~RawEventsSetup()
    {
        m_os << "\x1b[?1003l"
             << "\x1b[?1006l" << std::flush;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_old);
    }

    std::ostream &m_os;
    termios m_old;
};
} // namespace

namespace
{
cy::ansi::position read_position()
{
    std::cout << "\x1b[6n" << std::flush;
    // Response:
    // ESC [ row ; column R

    char c;
    int n = 0;
    int row = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (std::isdigit(c))
        {
            n = n * 10 + c - '0';
        }
        else if (c == ';')
        {
            row = n;
            n = 0;
        }
        else if (c == 'R')
        {
            return {row, n};
        }
        else if (c == 27 || c == '[')
        {
            // ok
        }
        else
        {
            // Failure
            return {0, 0};
        }
    }
    return {0, 0};
}
} // namespace

void cy::ansi::run(const std::function<event_return(event)> &fn)
{
    RawEventsSetup setup(std::cout);

    unsigned char c;

    int state = 0;
    /*
        States:
            0:
            1: 27
            2: 27 [
            3: 27 [ <
    */

    // first things first - Send a device status report
    // ESC [ 6 n
    // Terminal replies with
    // ESC [ row ; column R
    // This lets us know where the cursor is so we can calculate mouse moves relative to
    // our window.

    auto pos = read_position();
    std::cout << "Cursor X=" << pos.x << " Y=" << pos.y << "\r\n" << std::flush;

    int n;
    int num_params = 0;
    int params[3];

    event e;

    auto send_event = [&] { return fn(e) == event_return::exit_loop; };

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        switch (state)
        {
        case 0: // (Start of line)
            if (c == 27)
            {
                state = 1;
            }
            else
            {
                e.type = event_type::key_press;
                e.key = c;
                if (send_event())
                    return;
                if (c == 'q')
                    return;
                state = 0;
            }
            break;
        case 1: // 27
            if (c == '[')
            {
                state = 2;
            }
            else
            {
                state = 0;
            }
            break;
        case 2: // 27 [
            if (c == '<')
            {
                state = 3;
                n = 0;
                num_params = 0;
            }
            else
            {
                e.type = event_type::key_press;
                switch (c)
                {
                case 'A':
                    e.key = key::UP;
                    break;
                case 'B':
                    e.key = key::DOWN;
                    break;
                case 'C':
                    e.key = key::RIGHT;
                    break;
                case 'D':
                    e.key = key::LEFT;
                    break;
                default:
                    break;
                }
                state = 0;
                if (send_event())
                    return;
            }
            break;
        case 3:
            // 27 [ <
            // We have a sequence of semi-separated numbers followed by M
            if (c == ';')
            {
                if (num_params < 3)
                {
                    params[num_params] = n;
                    n = 0;
                    ++num_params;
                }
                else
                {
                    state = 0;
                }
            }
            else if (isdigit(c))
            {
                n = n * 10 + c - '0';
            }
            else
            {
                // Dump
                // for (int i = 0; i < num_params; ++i)
                // {
                //     std::cout << params[i] << ';';
                // }
                // std::cout << n << char(c) << "\r\n" << std::flush;

                if (c == 'M')
                {
                    // Mouse move or click
                    // param[0] indicates which buttons are pressed
                    // param[1] is the X
                    // param[2] is the Y

                    // 0 means just a click (no movement)
                    // +32 = mouse movement
                    // +3 = not buttons pressed (cursor movement)

                    // +4 = shift key
                    // +8 = option key
                    // +16 = CTRL key
                    e.key = params[0];
                    e.type = params[0] & 32 ? event_type::mouse_move : event_type::mouse_click;
                    e.x = params[1];
                    e.y = n;
                    if (send_event())
                        return;
                }
                else if (c == 'm')
                {
                    // Mouse release
                    e.type = event_type::mouse_release;
                    e.key = params[0];
                    e.x = params[1];
                    e.y = n;
                    if (send_event())
                        return;
                }
                else
                {
                    // Unknown: drop
                }
                state = 0;
            }
            break;

        default:
            return;
        }
    }
}

cy::ansi::key_press::key_press(const event &e) : m_key(e.type == event_type::key_press ? e.key : 0)
{
}

cy::ansi::key_press::operator bool() const
{
    return m_key;
}

int cy::ansi::key_press::key() const
{
    return m_key;
}


cy::ansi::mouse_click::mouse_click(const event &e)
{
    if(e.type == event_type::mouse_click)
    {
        flags = e.key;
        m_pos.x = e.x;
        m_pos.y = e.y;
    }
    else
    {
        m_pos.x = m_pos.y = -1;
    }
}

cy::ansi::mouse_click::operator bool() const
{
    return m_pos.x>=0 && m_pos.y >=0;
}

cy::ansi::position cy::ansi::mouse_click::pos() const
{
    return m_pos;
}