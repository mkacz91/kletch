#include "fitter_demo.h"
#include <lib/display_fresnel.h>

namespace kletch {

FitterDemo::FitterDemo() :
    ConstrainedClothoidDemo("Clothoid Fit")
{

}

FitterDemo::~FitterDemo()
{
    for (vec2f* target : m_targets)
        delete target;
}

void FitterDemo::on_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_aim_results.empty())
    {
        glUseProgram(m_cloth_program);
        glUniformMatrix3fv(m_cloth_matrix_uniform, m_camera.matrix());
        glUniform4f(m_cloth_color_uniform, 0, 0, 0, 1);
        glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
        glEnableVertexAttribArray(m_cloth_position_attrib);
        glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        for (int i = 0; i < m_aim_results.size(); ++i)
            glDrawArrays(GL_LINE_STRIP, i * CLOTHOID_VERTEX_COUNT, CLOTHOID_VERTEX_COUNT);

        glDisableVertexAttribArray(m_cloth_position_attrib);
    }

    ConstrainedClothoidDemo::on_render();
}

bool FitterDemo::on_event(Event const& e)
{
    if (ConstrainedClothoidDemo::on_event(e))
    {
        aim();
        return true;
    }

    switch (e.type) {
    case MOUSE_PRESS:
        if (e.button != MOUSE_LEFT)
            return false;
        m_targets.push_back(new vec2f(m_camera.canvas2world(e.pos)));
        m_control_overlay.add_point(m_targets.back());
        aim();
        return true;
    }

    return false;
}

void FitterDemo::on_open()
{
    ConstrainedClothoidDemo::on_open();

    gl::create_buffer(&m_cloth_vertices);

    gl::link_program(&m_cloth_program, "aimer_demo_cloth_vx", "uniform4_ft");
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");
}

void FitterDemo::on_close()
{
    gl::delete_program(&m_cloth_program);
    gl::delete_buffer(&m_cloth_vertices);

    ConstrainedClothoidDemo::on_close();
}

void FitterDemo::aim()
{
    if (m_targets.empty())
        return;

    vec2r origin = this->origin();
    real arc_radius = this->arc_radius();
    real theta0 = tangent_angle();
    real k0 = rl(1) / arc_radius;
    m_aim_results.clear();
    real k1_sum = 0;
    for (vec2f* target : m_targets)
    {
        m_aim_results.push_back(m_aimer.aim(origin, theta0, k0, *target));
        k1_sum += m_aim_results.back().a;
    }
    m_aim_results.push_back({ k1_sum / m_targets.size(), m_aim_results.back().s });

    std::vector<vec2f> cloth_vertices;
    for (auto const& result : m_aim_results)
    {
        real k1 = result.a, s = result.s;
        for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
        {
            real si = i * s / (CLOTHOID_VERTEX_COUNT - 1);
            cloth_vertices.push_back(origin + arc_radius * DisplayFresnel::eval(theta0, 1, k1, si));
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glBufferData(GL_ARRAY_BUFFER, cloth_vertices);

}

} // namespace kletch
