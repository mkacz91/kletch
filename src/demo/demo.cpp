#include "demo.h"

namespace kletch {

void Demo::init(SDL_Surface* canvas)
{
    assert(canvas != nullptr);
    m_canvas = canvas;
    init();
}

void Demo::clean_up(bool)
{
    clean_up();
    assert(m_canvas != nullptr);
    m_canvas = nullptr;
}

} // namespace kletch
