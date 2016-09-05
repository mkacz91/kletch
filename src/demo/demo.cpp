#include "demo.h"
#include "printing.h"

namespace kletch {

void Demo::open(GLFWwindow* window, TwBar* twbar, Event const& resize_event)
{
    assert(window != nullptr);
    assert(twbar != nullptr);
    cout << "Opening demo " << squote(display_name()) << "..." << endl;
    m_window = window;
    m_twbar = twbar;
    on_open();
    on_event(resize_event);
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
    m_size = 0;
    cout << "Done closing demo " << squote(display_name()) << endl;
}

void Demo::render()
{
    on_render();
    m_needs_redraw = false;
}

} // namespace kletch
