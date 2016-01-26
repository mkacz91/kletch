#ifndef KLETCH_DEMO_DEMO_EVENT_H
#define KLETCH_DEMO_DEMO_EVENT_H

#include "prefix.h"

namespace kletch {

class DemoEvent : public SDLEvent
{
public:
    void mark_handled() const { m_handled = true; }
    bool handled() const { return m_hanled; }

    void order_redraw() const { m_redraw_ordered = true; }
    bool redraw_ordered() const { return m_redraw_ordered; }

    void order_quit() const { m_quit_ordered = true; }
    bool quit_ordered() const { return m_quit_ordered; }

    void reset();

private:
    bool m_handled;
    bool m_redraw_ordered;
    bool m_quit_ordered;
}

inline void DemoEvent::reset()
{
    m_handled = false;
    m_redraw_ordered = false;
    m_quit_ordered = false;
}

} // namespace kletch

#endif // KLETCH_DEMO_DEMO_EVENT_H
