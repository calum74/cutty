#pragma once

#include <cutty/ansi/common.hpp>

#include <functional>

namespace cutty::ansi
{
enum class event_type
{
    key_press,
    mouse_move,
    mouse_click,
    mouse_release
};

struct event
{
    event_type type;
    int key, x, y;

    bool is_key_press() const;
    bool is_mouse() const;
    bool is_mouse_move() const;
    bool is_mouse_click() const;
    bool is_mouse_release() const;

    bool shift_key() const;
    bool ctrl_key() const;
    bool option_key() const;
};

class key_press
{
public:
    key_press(const event &e);
    explicit operator bool() const;
    int key() const;
private:
    int m_key;
};

class mouse_click
{
public:
    mouse_click(const event &e);
    explicit operator bool() const;

    bool shift();
    bool ctrl() const;
    bool option() const;

    position pos() const;

private:
    position m_pos;
    int flags;
};

class mouse_release
{
};

class mouse_move
{
public:
    // Unfortunate duplication of mouse_click TODO

    mouse_move(const event &e);
    explicit operator bool() const;

    bool shift();
    bool ctrl() const;
    bool option() const;

    position pos() const;

private:
    position m_pos;
    int flags;
};

enum key
{
    TAB = 9,
    RET = 13,
    ESC = 27,
    UP = 201,
    DOWN = 202,
    RIGHT = 203,
    LEFT = 204

};


enum event_return
{
    continue_loop,
    exit_loop
};

void run(const std::function<event_return(event)>&fn);

}
