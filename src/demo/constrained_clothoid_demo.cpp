#include "constrained_clothoid_demo.h"

namespace kletch {

ConstrainedClothoidDemo::ConstrainedClothoidDemo(const string& display_name) :
    Demo(display_name)
{
    m_control_overlay.set_camera(&m_camera); 
}

void ConstrainedClothoidDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_control_overlay.render();
}

void ConstrainedClothoidDemo::handle_event(const DemoEvent& e)
{
    if (e.type() == SDL_VIDEORESIZE)
    {
        close(); // TODO: This is not ok
        open();
    }

    m_camera.handle_event(e);
    if (e.handled())
        return;
    m_control_overlay.handle_event(e);
    if (e.handled())
        return;
}

void ConstrainedClothoidDemo::open()
{
    glClearColor(0.9f, 0.9f, 0.9f, 0);
    m_camera.open_grid();
    m_control_overlay.open();

    m_camera.size.x = width();
    m_camera.size.y = height();

    m_control_overlay.add_point(&m_origin);
    m_control_overlay.add_point(&m_target);
    m_control_overlay.add_point(&m_tangent_tip);
    m_control_overlay.add_vector(&m_origin, &m_tangent_tip);
    m_control_overlay.add_point(&m_circle_point);
}

void ConstrainedClothoidDemo::close() noexcept
{
    m_control_overlay.clear_points();

    m_control_overlay.close();
    m_camera.close_grid();
}

} // namespace kletch
