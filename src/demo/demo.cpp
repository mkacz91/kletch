#include "demo.h"

namespace kletch {

void Demo::open(SDL_Window* window, TwBar* twbar)
{
    assert(window != nullptr);
    assert(twbar != nullptr);
    cout << "Opening demo " << squote(display_name()) << "..." << endl;
    m_window = window;
    m_twbar = twbar;
    on_open();
    resize();
    invalidate();
    cout << "Demo " << squote(display_name()) << " opened" << endl;
}

void Demo::close()
{
    cout << "Closing demo " << squote(display_name()) << "..." << endl;
    assert(m_window != nullptr);
    assert(m_twbar != nullptr);
    on_close();
    m_twbar = nullptr;
    m_window = nullptr;
    m_size = vec2i::ZERO;
    cout << "Done closing demo " << squote(display_name()) << endl;
}

void Demo::resize()
{
    SDL_GetWindowSize(m_window, &m_size.x, &m_size.y);
    on_resize();
    invalidate();
}

void Demo::render()
{
    on_render();
    m_needs_redraw = false;
}

} // namespace kletch
