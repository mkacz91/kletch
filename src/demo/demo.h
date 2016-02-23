#ifndef KLETCH_DEMO_DEMO_H
#define KLETCH_DEMO_DEMO_H

#include "prefix.h"

#include "demo_event.h"

namespace kletch {

class Demo
{
public:
    const string& display_name() const noexcept { return m_display_name; }

    void open(SDL_Surface* canvas, TwBar* twbar);
    void close(bool);

    SDL_Surface* canvas() noexcept { return m_canvas; }
    int width() const noexcept { return m_canvas->w; }
    int height() const noexcept { return m_canvas->h; }
    bool active() const noexcept { return m_canvas != nullptr; }

    TwBar* twbar() noexcept { return m_twbar; }

    virtual void render() = 0;
    virtual void handle_event(const DemoEvent& e) { } // Do nothing

    virtual ~Demo() noexcept
    {
        assert(!active());
    }

protected:
    Demo(const string& display_name) : m_display_name(display_name) { }

    virtual void open() = 0;
    virtual void close() noexcept = 0;

private:
    const string m_display_name;
    SDL_Surface* m_canvas = nullptr;
    TwBar* m_twbar = nullptr;
};

} // namespace kletch

#endif // KLETCH_DEMO_DEMO_H
