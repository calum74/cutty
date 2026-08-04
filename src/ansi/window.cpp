#include <cutty/ansi/window.hpp>
#include <cutty/ansi/events.hpp>


#include <algorithm>

namespace ancy = cutty::ansi;

ancy::window::window(size s, std::ostream &os) : m_underlying(os), m_dimensions(s), m_data(s.w * s.h)
{
    m_dirty_list.reserve(m_data.size());
    for (int i = 0; i < s.h; i++)
    {
        m_underlying.endl();
    }
}

ancy::window::~window()
{
    flush();
}

void ancy::window::flush()
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

void ancy::window::put(const character &ch, position p)
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

ancy::size ancy::window::dimensions() const
{
    return m_dimensions;
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
    text(m_style, sv);
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
    m_style = s;
}

void ancy::window::go_to(position p)
{
    m_position = p;
    m_underlying.go_to(p);
}

void ancy::window::reset()
{
    m_style = {};
}

ancy::writer &ancy::window::get_writer()
{
    return *this;
}

void ancy::window::add_child(widget&w)
{
    m_children.push_back(&w);
}

void ancy::window::remove_child(widget&w)
{
    for(auto it=m_children.begin(); it!=m_children.end(); ++it)
    {
        if(*it == &w)
        {
            m_children.erase(it);
            return;
        }
    }
    // Silently ignore?
}

void ancy::window::run()
{
    m_quit = false;
    flush();
    ansi::run([this](const event &e)
    {
        if(ansi::key_press p{e})
        {
            key_press(p.key());
        }
        if(ansi::mouse_click c{e})
        {

        }
        flush();
        return m_quit ? event_return::exit_loop : event_return::continue_loop;
    });
}

void ancy::window::key_press(char32_t ch)
{
    for(auto *child : m_children)
    {
        child->key_press(ch);
    }
}


void ancy::window::quit()
{
    m_quit = true;
}

std::function<void()> ancy::window::quit_action()
{
    return [this] { quit(); };
}
