#include "demo.h"

namespace kletch {

void Demo::open(SDL_Surface* canvas, TwBar* twbar)
{
    assert(canvas != nullptr);
    assert(twbar != nullptr);
    cout << "Opening demo " << squote(display_name()) << "..." << endl;
    m_canvas = canvas;
    m_twbar = twbar;
    open();
    gl_open();
    cout << "Demo " << squote(display_name()) << " opened" << endl;
}

void Demo::close(bool)
{
    cout << "Closing demo " << squote(display_name()) << "..." << endl;
    assert(m_canvas != nullptr);
    assert(m_twbar != nullptr);
    gl_close();
    close();
    m_twbar = nullptr;
    m_canvas = nullptr;
    cout << "Done closing demo " << squote(display_name()) << endl;
}

void Demo::gl_lost()
{
    cout << "Recreating OpenGL resources ..." << endl;
    gl_open();
}

} // namespace kletch
