#include "aimer_demo.h"
#include <vector>

namespace kletch {

AimerDemo::AimerDemo() : Demo("Clothoid Aim")
{
    m_points.set_camera(&m_camera);
}

void AimerDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_camera.render_grid();
    m_points.render();
}

void AimerDemo::handle_event(const DemoEvent& e)
{
    if (e.type() == SDL_VIDEORESIZE)
    {
        close(); // TODO: This is not ok
        open();
    }

    m_camera.handle_event(e);
    if (e.handled())
        return;
    m_points.handle_event(e);
    if (e.handled())
        return;
}

void AimerDemo::open()
{
    glClearColor(1, 1, 1, 0);
    m_camera.open_grid();
    m_points.open();

    m_camera.size.x = width();
    m_camera.size.y = height();

    *m_points.new_point() = vec2f(0.5f, 0.3f);
}

void AimerDemo::close() noexcept
{
    m_points.clear_points();

    m_points.close();
    m_camera.close_grid();
}

} // namespace kletch
