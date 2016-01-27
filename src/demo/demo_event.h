#ifndef KLETCH_DEMO_DEMO_EVENT_H
#define KLETCH_DEMO_DEMO_EVENT_H

#include "prefix.h"

namespace kletch {

class DemoEvent
{
public:
    SDL_Event& sdl_event() { return m_sdl_event; }
    Uint8 type() const { return m_sdl_event.type; }
    const SDL_KeyboardEvent& key() const { return m_sdl_event.key; }
    const SDL_MouseMotionEvent& motion() const { return m_sdl_event.motion; }
    const SDL_MouseButtonEvent& button() const { return m_sdl_event.button; }
    const SDL_ResizeEvent& resize() const { return m_sdl_event.resize; }

    void mark_handled() const { *m_handled = true; }
    bool handled() const { return *m_handled; }

    void request_redraw() const { *m_redraw_requested = true; }
    bool redraw_requested() const { return *m_redraw_requested; }

    void request_quit() const { *m_quit_requested = true; }
    bool quit_requested() const { return *m_quit_requested; }

    void reset();

private:
    SDL_Event m_sdl_event;
    unique_ptr<bool> m_handled = make_unique(new bool);
    unique_ptr<bool> m_redraw_requested = make_unique(new bool);
    unique_ptr<bool> m_quit_requested = make_unique(new bool);
};

inline void DemoEvent::reset()
{
    *m_handled = false;
    *m_redraw_requested = false;
    *m_quit_requested = false;
}

} // namespace kletch

#endif // KLETCH_DEMO_DEMO_EVENT_H
