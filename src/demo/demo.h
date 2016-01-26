#ifndef KLETCH_DEMO_DEMO_H
#define KLETCH_DEMO_DEMO_H

#include "prefix.h"

#include "demo_event.h"

namespace kletch {

class Demo
{
public:
    void init(SDL_Surface* canvas);
    void clean_up(bool);

    SDL_Surface* canvas() { return m_canvas; }
    int width() const { return m_canvas->w; }
    int height() const { return m_canvas->h; }

    virtual void init() = 0;
    virtual void clean_up() = 0;
    virtual void redraw() = 0;
    virtual bool handle_event(const DemoEvent& e) { } // Do nothing

private:
    SDL_Surface* m_canvas;
};

} // namespace kletch

#endif // KLETCH_DEMO_DEMO_H
