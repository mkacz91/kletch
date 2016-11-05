#include "aimer_demo.h"
#include <vector>
#include <lib/display_fresnel.h>
#include <lib/kd_tree.h>
#include <math/fuzzy.h>

namespace kletch {

AimerDemo::AimerDemo() :
    ConstrainedClothoidDemo("Clothoid Aim")
{
    //m_control_overlay.add_point(&m_aim_eval);
    add_point(&m_target);
}

void AimerDemo::on_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw aimer preprocessed data

    glUseProgram(m_aimer_program);
    mat3f aimer_matrix = mat3f::eye()
        .scale(arc_radius(), signed_arc_radius())
        .rotate(tangent_angle())
        .translate(origin())
        .premul(m_camera.matrix());
    glEnableVertexAttribArray(m_aimer_position_attrib);

    // Draw kd-tree structure

    glUniformMatrix3fv(m_aimer_matrix_uniform, aimer_matrix);
    glUniform3f(m_aimer_color_uniform, 0.8f, 0.8f, 0.8f);
    glBindBuffer(GL_ARRAY_BUFFER, m_kd_tree_vertices);
    glVertexAttribPointer(m_aimer_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, m_kd_tree_vertex_count);

    // Draw samples

    glUniformMatrix3fv(m_aimer_matrix_uniform, aimer_matrix);
    glUniform3f(m_cloth_color_uniform, 0.5f, 0.5f, 0.8f);
    glBindBuffer(GL_ARRAY_BUFFER, m_sample_vertices);
    glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, m_sample_vertex_count);

    glDisableVertexAttribArray(m_aimer_position_attrib);

    // Draw clothoid

    glUseProgram(m_cloth_program);
    glUniformMatrix3fv(m_cloth_matrix_uniform, m_camera.matrix());
    glEnableVertexAttribArray(m_cloth_position_attrib);

    if (m_cloth_ready)
    {
        glUniform4f(m_cloth_color_uniform, 1, 0, 0, 1);
        glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
        glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, CLOTHOID_VERTEX_COUNT);
    }

    glDisableVertexAttribArray(m_cloth_position_attrib);

    ConstrainedClothoidDemo::on_render();
}

bool AimerDemo::on_event(Event const& e)
{
    if (ConstrainedClothoidDemo::on_event(e))
    {
        aim();
        return true;
    }
    return false;
}

void AimerDemo::on_open()
{
    ConstrainedClothoidDemo::on_open();

    // Alter TwBar

    TwAddVarRO(twbar(), "k1", TW_TYPE_FLOAT, &m_k1, nullptr);
    TwAddVarRO(twbar(), "s", TW_TYPE_FLOAT, &m_s, nullptr);
    TwAddVarCB(
        twbar(), "Iters", TW_TYPE_INT32,
        set_refine_steps_cb, get_refine_steps_cb,
        this, "min=0 max=10"
    );

    // Clothoid

    gl::create_buffer(&m_cloth_vertices);
    m_cloth_ready = false;

    gl::link_program(&m_cloth_program, "aimer_demo_cloth_vx", "uniform4_ft");
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");

    glUseProgram(m_cloth_program);
    glUniform4f(m_cloth_color_uniform, 0, 0, 0, 1);

    // Aimer kd-tree and samples

    std::vector<vec2f> samples = m_aimer.get_samples();

    gl::link_program(&m_aimer_program, "mat3_pos2_vx", "uniform3_ft");
    m_aimer_matrix_uniform = gl::get_uniform_location(m_aimer_program, "matrix");
    m_aimer_color_uniform = gl::get_uniform_location(m_aimer_program, "color");
    m_aimer_position_attrib = gl::get_attrib_location(m_aimer_program, "position");

    std::vector<float> grid_vertices = KdTree::get_lines(samples, 0.02f);
    gl::create_buffer(&m_kd_tree_vertices, grid_vertices);
    m_kd_tree_vertex_count = grid_vertices.size() / 2;

    const float SAMPLE_SIZE = 0.02f;
    std::vector<vec2f> sample_vertices;
    for (vec2r const& sample : samples)
    {
        float x = sample.x, y = sample.y;
        sample_vertices.emplace_back(x - SAMPLE_SIZE, y);
        sample_vertices.emplace_back(x + SAMPLE_SIZE, y);
        sample_vertices.emplace_back(x, y - SAMPLE_SIZE);
        sample_vertices.emplace_back(x, y + SAMPLE_SIZE);
    }
    gl::create_buffer(&m_sample_vertices, sample_vertices);
    m_sample_vertex_count = sample_vertices.size();

    aim();
}

void AimerDemo::on_close()
{
    m_cloth_ready = false;

    gl::delete_program(&m_cloth_program);
    gl::delete_program(&m_aimer_program);
    gl::delete_buffer(&m_cloth_vertices);
    gl::delete_buffer(&m_sample_vertices);
    gl::delete_buffer(&m_kd_tree_vertices);

    TwRemoveVar(twbar(), "Iters");
    TwRemoveVar(twbar(), "s");
    TwRemoveVar(twbar(), "k1");

    ConstrainedClothoidDemo::on_close();
}

void AimerDemo::set_refine_steps_cb(const void* value, void* client_data)
{
    AimerDemo* aimer_demo = (AimerDemo*)client_data;
    aimer_demo->m_aimer.set_refine_steps(*(const int*)value);
    aimer_demo->aim();
}

void AimerDemo::get_refine_steps_cb(void* value, void* client_data)
{
    AimerDemo* aimer_demo = (AimerDemo*)client_data;
    *(int*)value = aimer_demo->m_aimer.refine_steps();
}

void AimerDemo::aim()
{
    real angle = tangent_angle();
    real k0 = initial_curvature();
    vec2r target = (m_target - origin()).rot(-angle);
    auto aim_result = m_aimer.aim(k0, target);
    if (!equal(aim_result.p, target, AIM_TOL))
        aim_result = m_aimer.aim(k0, target, 0);

    m_aim_result = aim_result;
    real k1 = m_aim_result.k1;
    real s = m_aim_result.s;
    m_k1 = float(k1); m_s = float(s);

    m_aim_eval = origin() + DisplayFresnel::eval(angle, k0, k1, s); // TODO

    std::vector<vec2f> cloth_vertices;
    for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
    {
        real si = i * s / (CLOTHOID_VERTEX_COUNT - 1);
        cloth_vertices.push_back(
            origin() + DisplayFresnel::eval(angle, k0, k1, si)
        );
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glBufferData(GL_ARRAY_BUFFER, cloth_vertices);
    m_cloth_ready = true;
}

} // namespace kletch
