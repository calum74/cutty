#include <cutty/ansi/events.hpp>
#include <cutty/ansi/raw.hpp>
#include <cutty/ansi/window.hpp>

#include <algorithm>
#include <vector>

namespace ancy = cutty::ansi;

class ancy::window::impl
{
public:
    impl();
    impl(size s, std::ostream &os);

    raw_writer m_underlying;
    position m_position;
    style m_style;
    size m_dimensions;
    struct viewport_character : character
    {
        bool dirty = false;
    };
    std::vector<viewport_character> m_data;
    std::vector<int> m_dirty_list;
    std::vector<widget *> m_children;
    bool m_quit;
    bool m_alt_screen;

    bool m_show_cursor;
    position m_cursor_position;
    widget *m_focus = 0;
};

ancy::window::window(size s, std::ostream &os) : m_impl(std::make_unique<impl>(s, os))
{
}

ancy::window::impl::impl(size s, std::ostream &os)
    : m_underlying(os), m_dimensions(s), m_data(s.w * s.h), m_alt_screen(false), m_show_cursor(false)
{
    m_dirty_list.reserve(m_data.size());
    for (int i = 0; i < s.h; i++)
    {
        m_underlying.endl();
    }
}

ancy::window::window() : m_impl(std::make_unique<impl>())
{
}

ancy::window::impl::impl()
    : m_underlying(std::cout), m_dimensions(get_terminal_size()), m_data(m_dimensions.w * m_dimensions.h),
      m_alt_screen(true)
{
    // TODO: Tidy up ostream and the rest
    alternate_screen_on(std::cout);
    wrap_off(std::cout);

    // m_underlying.go_to({0,0});
    // m_underlying.text("X");
    // m_underlying.go_to({1,1});
    // m_underlying.text("Y");
    // flush();
}

ancy::window::~window()
{
    if (!m_impl->m_show_cursor)
    {
        m_impl->m_underlying.show_cursor();
    }

    if (m_impl->m_alt_screen)
    {
        wrap_on(std::cout);
        alternate_screen_off(std::cout);
    }
    else
    {
        m_impl->m_underlying.go_to({0, m_impl->m_dimensions.h});
    }
    m_impl->m_underlying.flush();
}

void ancy::window::flush()
{
    std::sort(m_impl->m_dirty_list.begin(), m_impl->m_dirty_list.end());
    for (auto index : m_impl->m_dirty_list)
    {
        auto &c = m_impl->m_data[index];
        m_impl->m_underlying.go_to({index % m_impl->m_dimensions.w, index / m_impl->m_dimensions.w});
        m_impl->m_underlying.put(c);
        c.dirty = false;
    }
    m_impl->m_underlying.reset();
    if (!m_impl->m_alt_screen)
    {
        // TODO: Need a location for the cursor...
        // TODO: Need a cursor style...
        m_impl->m_underlying.go_to({0, m_impl->m_dimensions.h});
    }
    else
    {
    }

    if (m_impl->m_show_cursor)
    {
        m_impl->m_underlying.go_to(m_impl->m_cursor_position);
    }

    m_impl->m_underlying.flush();
    m_impl->m_dirty_list.clear();
}

void ancy::window::put(const character &ch, position p)
{
    if (p.x < 0 || p.x >= m_impl->m_dimensions.w || p.y < 0 || p.y >= m_impl->m_dimensions.h)
    {
        return;
    }

    auto index = p.x + p.y * m_impl->m_dimensions.w;
    auto &my_char = m_impl->m_data.at(index);
    if (ch.ch != my_char.ch || ch.style != my_char.style)
    {
        my_char.style = ch.style;
        my_char.ch = ch.ch;
        if (!my_char.dirty)
        {
            m_impl->m_dirty_list.push_back(index);
            my_char.dirty = true;
        }
    }
}

ancy::size ancy::window::dimensions() const
{
    return m_impl->m_dimensions;
}

void ancy::window::put(const character &c)
{
    put(c, m_position);
    ++m_position.x;
}

void ancy::window::endl()
{
    m_position.y++;
    m_position.x = 0;
}

void ancy::window::text(std::string_view sv)
{
    text(m_impl->m_style, sv);
}

void ancy::window::text(const style &s, std::string_view sv)
{
    character ch{.style = s};
    for (auto c : sv)
    {
        ch.ch = c;
        put(ch);
    }
}

void ancy::window::apply(const style &s)
{
   m_impl->m_style = s;
}

void ancy::window::go_to(position p)
{
    m_position = p;
    m_impl->m_underlying.go_to(p);
}

void ancy::window::reset()
{
    m_impl->m_style = {};
}

ancy::writer &ancy::window::get_writer()
{
    return *this;
}

void ancy::window::add_child(widget &w)
{
    m_impl->m_children.push_back(&w);

    // !! This does not work as add_child is called from the constructor
    if (w.can_take_focus() && !m_impl->m_focus)
    {
        m_impl->m_focus = &w;
        m_impl->m_focus->grant_focus(true);
    }
}

void ancy::window::remove_child(widget &w)
{
    if (m_impl->m_focus == &w)
    {
        m_impl->m_focus = nullptr;
    }

    for (auto it = m_impl->m_children.begin(); it != m_impl->m_children.end(); ++it)
    {
        if (*it == &w)
        {
           m_impl->m_children.erase(it);
            return;
        }
    }
    // Silently ignore?
}

void ancy::window::run()
{
    // TODO: Grant focus
    if(!m_impl->m_focus)
    {
        for(auto i : m_impl->m_children)
        {
            if(i->can_take_focus())
            {
                set_focus(*i);
                break;
            }
        }
    }


    m_impl->m_quit = false;
    flush();
    auto mouse_offset = current_position();
    ansi::run([this, mouse_offset](const event &e) {
        if (ansi::key_press p{e})
        {
            key_press(p.key());
        }
        else if (ansi::mouse_click c{e})
        {
            mouse_click(c.pos() + mouse_offset, c.flags());
        }
        else if (ansi::mouse_release c{e})
        {
            mouse_release(c.pos() + mouse_offset, c.flags());
        }
        else if (ansi::mouse_move m{e})
        {
            mouse_move(m.pos() + mouse_offset, m.flags());
        }
        else if (ansi::mouse_scroll s{e})
        {
            mouse_scroll(s.pos() + mouse_offset, s.flags());
        }
        flush();
        return m_impl->m_quit ? event_return::exit_loop : event_return::continue_loop;
    });
}

void ancy::window::key_press(char32_t ch)
{
    if (m_impl->m_focus)
    {
        if(ch == UP)
        {
            prev_focus();
        }
        else if(ch == '\t' || ch == DOWN)
        {
            next_focus();
        }
        else
        {
            m_impl->m_focus->key_press(ch);
        }
    }
    else
    {
        for (auto *child : m_impl->m_children)
        {
            child->key_press(ch);
        }
    }
}

void ancy::window::mouse_click(position p, mouse_flags f)
{
    for (auto *child : m_impl->m_children)
    {
        child->mouse_click(p, f);
    }
}

void ancy::window::mouse_release(position p, mouse_flags f)
{
    for (auto *child : m_impl->m_children)
    {
        child->mouse_release(p, f);
    }
}

void ancy::window::mouse_move(position p, mouse_flags f)
{
    for (auto *child : m_impl->m_children)
    {
        child->mouse_move(p, f);
    }
}

void ancy::window::mouse_scroll(position p, mouse_flags f)
{
    for (auto *child : m_impl->m_children)
    {
        child->mouse_scroll(p, f);
    }
}

void ancy::window::quit()
{
    m_impl->m_quit = true;
}

std::function<void()> ancy::window::quit_action()
{
    return [this] { quit(); };
}

ancy::position ancy::window::current_position() const
{
    return m_impl->m_underlying.current_position();
}

void ancy::window::show_cursor()
{
    if (!m_impl->m_show_cursor)
    {
        m_impl->m_underlying.show_cursor();
        m_impl->m_show_cursor = true;
    }
}

void ancy::window::show_cursor(position p)
{
    if (!m_impl->m_show_cursor)
    {
        m_impl->m_underlying.show_cursor(p);
        m_impl->m_show_cursor = true;
    }
    m_impl->m_cursor_position = p;
}

void ancy::window::hide_cursor()
{
    if (m_impl->m_show_cursor)
    {
        m_impl->m_underlying.hide_cursor();
        m_impl->m_show_cursor = false;
    }
}

void ancy::window::set_focus(widget &child)
{
    if (m_impl->m_focus == &child)
    {
        return;
    }
    else if (m_impl->m_focus)
    {
        m_impl->m_focus->grant_focus(false);
    }
    m_impl->m_focus = &child;
    m_impl->m_focus->grant_focus(true);
}

void ancy::window::next_focus()
{
    for(auto c : m_impl->m_children)
    {
        if(m_impl->m_focus ==c)
        {
            c->grant_focus(false);
            m_impl->m_focus = nullptr;
        }
        else if(!m_impl->m_focus && c->can_take_focus())
        {
            set_focus(*c);
            return;
        }
    }
    for(auto c : m_impl->m_children)
    {
        if(c->can_take_focus())
        {
            set_focus(*c);
            return;
        }
    }
}

void ancy::window::prev_focus()
{
    widget * last_widget = nullptr;
    for(auto *w : m_impl->m_children)
    {
        if(w->can_take_focus())
        {
            last_widget = w;
        }
    }

    if(!last_widget)
    {
        return;
    }

    if(!m_impl->m_focus)
    {
        set_focus(*last_widget);
        return;
    }

    for(auto *w : m_impl->m_children)
    {
        if(w == m_impl->m_focus)
        {
            set_focus(*last_widget);
            return;
        }

        if(w->can_take_focus())
        {
            last_widget = w;
        }
    }
}
