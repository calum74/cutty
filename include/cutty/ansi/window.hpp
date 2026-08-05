#pragma once

#include "events.hpp"
#include "widget.hpp"
#include "writer.hpp"

#include <vector>

namespace cutty::ansi
{

class window : public writer, public widget
{
  public:
    window();  // Alt-screen mode

    explicit window(size s, std::ostream &os = std::cout);

    ~window();

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

    writer &get_writer() override;

    void run();

    void add_child(widget&w) override;

    void remove_child(widget&w) override;

    void key_press(char32_t ch) override;
    void mouse_click(position p, mouse_flags) override;
    void mouse_release(position p, mouse_flags) override;
    void mouse_move(position p, mouse_flags) override;
    void mouse_scroll(position p, mouse_flags) override;

    void quit();

    std::function<void()> quit_action();

  private:
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
    std::vector<widget*> m_children;
    bool m_quit;
    bool m_alt_screen;
};
}