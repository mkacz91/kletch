#include "demo.h"

namespace kletch {

void Demo::open(SDL_Surface* canvas)
{
    assert(canvas != nullptr);
    cout << "Opening demo " << squote(display_name()) << "..." << endl;
    m_canvas = canvas;
    open();
    cout << "Demo " << squote(display_name()) << " opened" << endl;
}

void Demo::close(bool)
{
    cout << "Closing demo " << squote(display_name()) << "..." << endl;
    close();
    assert(m_canvas != nullptr);
    m_canvas = nullptr;
    cout << "Done closing demo " << squote(display_name()) << endl;
}

} // namespace kletch
