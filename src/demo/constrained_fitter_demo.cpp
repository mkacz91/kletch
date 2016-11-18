#include "constrained_fitter_demo.h"
#include <lib/display_fresnel.h>
#include <lib/precise_fresnel.h>

namespace kletch {

ConstrainedFitterDemo::ConstrainedFitterDemo() :
    ConstrainedClothoidDemo("Constrained Clothoid Fit")
{

}

ConstrainedFitterDemo::~ConstrainedFitterDemo()
{
    for (vec2f* target : m_targets)
        delete target;
}

void ConstrainedFitterDemo::on_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_aim_results.empty())
    {
        glUseProgram(m_cloth_program);
        glUniformMatrix3fv(m_cloth_matrix_uniform, m_camera.matrix());
        glEnableVertexAttribArray(m_cloth_position_attrib);

        glBindBuffer(GL_ARRAY_BUFFER, m_poly_vertices);
        glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUniform4f(m_cloth_color_uniform, 0.7f, 0.7f, 0.7f, 1);
        glDrawArrays(GL_LINE_STRIP, 0, m_targets.size() + 1);

        glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
        glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUniform4f(m_cloth_color_uniform, 0.4f, 0.4f, 0.4f, 1);
        for (int i = 0; i < m_aim_results.size(); ++i)
        {
            if (i == m_aim_results.size() - 1)
                glUniform4f(m_cloth_color_uniform, 1, 0, 0, 1);
            glDrawArrays(GL_LINE_STRIP, i * CLOTHOID_VERTEX_COUNT, CLOTHOID_VERTEX_COUNT);
        }

        glDisableVertexAttribArray(m_cloth_position_attrib);
    }

    ConstrainedClothoidDemo::on_render();
}

bool ConstrainedFitterDemo::on_event(Event const& e)
{
    if (ConstrainedClothoidDemo::on_event(e))
    {
        aim();
        return true;
    }

    switch (e.type) {
    case MOUSE_PRESS:
    {
        if (e.button != MOUSE_LEFT)
            return false;
        m_targets.push_back(new vec2f(m_camera.canvas2world(e.pos)));
        add_point(m_targets.back());
        aim();
        return true;
    }
    default:
    {
        break;
    }}

    return false;
}

void ConstrainedFitterDemo::on_open()
{
    ConstrainedClothoidDemo::on_open();

    gl::create_buffer(&m_cloth_vertices);
    gl::create_buffer(&m_poly_vertices);

    gl::link_program(&m_cloth_program, "aimer_demo_cloth_vx", "uniform4_ft");
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");
}

void ConstrainedFitterDemo::on_close()
{
    gl::delete_program(&m_cloth_program);
    gl::delete_buffer(&m_cloth_vertices);
    gl::delete_buffer(&m_poly_vertices);

    ConstrainedClothoidDemo::on_close();
}

void ConstrainedFitterDemo::on_point_move(vec2f* point, vec2f prev_pos)
{
    ConstrainedClothoidDemo::on_point_move(point, prev_pos);

    int index = -1;
    for (int i = 0; i < m_targets.size(); ++i)
    {
        if (point == m_targets[i])
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;

    vec2f dp = span(prev_pos, *point);
    for (int i = 0; i < m_targets.size(); ++i)
    {
        if (i == index)
            continue;
        *m_targets[i] += dp / (2 << abs(i - index));
    }
}

void ConstrainedFitterDemo::aim()
{
    if (m_targets.empty())
        return;

    vec2r origin = this->origin();
    real arc_radius = this->arc_radius();
    real angle = tangent_angle();
    real k0 = rl(1) / arc_radius;
    m_aim_results.clear();
    real k1_sum = 0;
    real w_sum = 0;
    std::vector<vec2f> poly_vertices { {0, 0} };
    for (vec2f* target : m_targets)
    {
        auto aim_result = m_aimer.aim(k0, (*target - origin).rotate(-angle));
        m_aim_results.push_back(aim_result);
        real w = PreciseFresnel::eval_m2(k0, aim_result.k1, aim_result.s).len_sq();
        k1_sum += w * m_aim_results.back().k1;
        w_sum += w;
        poly_vertices.push_back(*target);
    }
    m_aim_results.push_back({ k1_sum / w_sum, m_aim_results.back().s });

    std::vector<vec2f> cloth_vertices;
    for (auto const& result : m_aim_results)
    {
        real k1 = result.k1, s = result.s;
        for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
        {
            real si = i * s / (CLOTHOID_VERTEX_COUNT - 1);
            cloth_vertices.push_back(origin + arc_radius * PreciseFresnel::eval(1, k1, si).rotate(angle));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_poly_vertices);
    glBufferData(GL_ARRAY_BUFFER, poly_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glBufferData(GL_ARRAY_BUFFER, cloth_vertices);
}

} // namespace kletch
