#include "demo.h"

namespace kletch {

void Demo::init(SDL_Surface* canvas)
{
    assert(canvas != nullptr);
    cout << "Initializing demo " << squote(display_name()) << "..." << endl;
    m_canvas = canvas;
    init();
    cout << "Done initializing demo " << squote(display_name()) << endl;
}

void Demo::close(bool)
{
    cout << "Closing demo " << squote(display_name()) << "..." << endl;
    close();
    assert(m_canvas != nullptr);
    m_canvas = nullptr;
    cout << "Done closing demo " << squote(display_name()) << "..." << endl;
}

} // namespace kletch
