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
    cout << "Demo " << squote(display_name()) << " opened" << endl;
}

void Demo::close(bool)
{
    cout << "Closing demo " << squote(display_name()) << "..." << endl;
    assert(m_canvas != nullptr);
    assert(m_twbar != nullptr);
    close();
    m_twbar = nullptr;
    m_canvas = nullptr;
    cout << "Done closing demo " << squote(display_name()) << endl;
}

} // namespace kletch
