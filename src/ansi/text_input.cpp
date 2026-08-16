#include <cutty/ansi/widgets.hpp>

namespace ancy = cutty::ansi;

ancy::text_input::text_input(widget &parent, position p, size s, const style &text_style,
                             const style &button_normal_style, const style &button_focus_style,
                             const style &disabled_style, std::string initial_text, std::string prompt_text,
                             std::function<void()> change_action, std::function<void()> enter_action)
    : widget(parent, p, s), m_text_style(text_style), m_button_normal_style(button_normal_style),
      m_button_focus_style(button_focus_style), m_disabled_style(disabled_style), m_prompt(std::move(prompt_text)),
      on_changed(std::move(change_action)), on_enter(std::move(enter_action))
{
    set_text(std::move(initial_text));
}

void ancy::text_input::set_text(std::string text)
{
    m_text = text;
    draw(get_writer());
}

void ancy::text_input::draw(writer &w)
{
    const auto &str = m_text.empty() ? m_prompt : m_text;
    const auto &style = m_text.empty() ? m_disabled_style : m_text_style;

    character ch{.style = style};

    bool overflow = m_text.size() > (m_size.w);
    int offset = overflow ? m_text.size() - (m_size.w) : 0;

    for (int i = 0; i < m_size.w; ++i)
    {
        int j = i + offset;

        if (overflow && i == 0)
        {
            ch.ch = '<';
        }
        else if (j >= 0 && j < str.size())
        {
            ch.ch = str[j];
        }
        else
        {
            ch.ch = ' ';
        }
        w.put(ch, {m_position.x + i, m_position.y});
    }
    if(m_has_focus)
    {
        w.show_cursor({m_position.x + int(m_text.size()) - offset, m_position.y});
    }
}

void ancy::text_input::mouse_click(position, mouse_flags)
{
}

void ancy::text_input::key_press(char32_t key)
{
    if (key == 127)
    {
        if (!m_text.empty())
        {
            m_text.pop_back();
            on_changed();
        }
    }
    else if (key == 13)
    {
        // Enter key: perform action
        on_enter();
    }
    else if (key == UP || key == '\t')
    {
        m_parent->next_focus();
    }
    else if (key < 127)
    {
        m_text += key;
        on_changed();
    }
    else
    {
        // Ignore?
        return;
    }
    draw(get_writer());
}

const std::string &ancy::text_input::get_text() const
{
    return m_text;
}

void ancy::text_input::grant_focus(bool b)
{
    m_has_focus = b;
    redraw();
}

bool ancy::text_input::can_take_focus()
{
    return true;
}
