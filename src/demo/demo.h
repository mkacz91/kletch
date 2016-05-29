#ifndef KLETCH_DEMO_DEMO_H
#define KLETCH_DEMO_DEMO_H

#include "prefix.h"

namespace kletch {

class Demo
{
public:
    virtual ~Demo() { assert(!active()); }

    const string& display_name() const { return m_display_name; }

    void open(SDL_Window* window, TwBar* twbar);
    void close();
    void resize();

    SDL_Window* window() { return m_window; }
    TwBar* twbar() { return m_twbar; }
    bool active() const { return m_window != nullptr; }

    vec2i const& size() { return m_size; }
    int width() { return m_size.x; }
    int height() { return m_size.y; }

    void render();
    void invalidate() { m_needs_redraw = true; }
    bool needs_redraw() const { return m_needs_redraw; }

    virtual bool on_event(SDL_Event const& e) { unused(e); return false; }

protected:
    Demo(string const& display_name) : m_display_name(display_name) { }

    virtual void on_open() = 0;
    virtual void on_close() = 0;
    virtual void on_render() = 0;
    virtual void on_resize() = 0;

private:
    string const m_display_name;
    SDL_Window* m_window = nullptr;
    TwBar* m_twbar = nullptr;
    vec2i m_size = vec2i::ZERO;
    bool m_needs_redraw = false;
};

} // namespace kletch

#endif // KLETCH_DEMO_DEMO_H
