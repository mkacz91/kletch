#include "aimer_demo.h"
#include <vector>
#include <lib/fresnel.h>

namespace kletch {

AimerDemo::AimerDemo() :
    ConstrainedClothoidDemo("Clothoid Aim")
{
    //m_control_overlay.add_point(&m_aim_eval);
}

void AimerDemo::on_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw aimer preprocessed data

    glUseProgram(m_aimer_program);
    mat3f aimer_matrix = mat3f::eye()
        .scale(arc_radius(), arc_radius())
        .rotate(tangent_angle())
        .translate(origin())
        .premul(m_camera.matrix());
    glEnableVertexAttribArray(m_aimer_position_attrib);

    // Draw grid

    glUniformMatrix3fv(m_aimer_matrix_uniform, aimer_matrix * m_aimer.m_grid_box.matrix_to());
    glUniform3f(m_aimer_color_uniform, 0.8f, 0.8f, 0.8f);
    glBindBuffer(GL_ARRAY_BUFFER, m_grid_vertices);
    glVertexAttribPointer(m_aimer_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 4 * (ClothoidAimer::GRID_SIZE + 1));

    // Draw samples

    glUniformMatrix3fv(m_aimer_matrix_uniform, aimer_matrix);
    glUniform3f(m_cloth_color_uniform, 0.5f, 0.5f, 0.8f);
    glBindBuffer(GL_ARRAY_BUFFER, m_sample_vertices);
    glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, m_aimer.m_samples.size() * 4);

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

bool AimerDemo::on_event(const SDL_Event& e)
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
    TwAddVarRO(twbar(), "a", TW_TYPE_FLOAT, &m_a, nullptr);
    TwAddVarRO(twbar(), "s", TW_TYPE_FLOAT, &m_s, nullptr);
    TwAddVarCB(
        twbar(), "Iters", TW_TYPE_INT32,
        set_refine_steps_cb, get_refine_steps_cb,
        this, "min=0 max=10"
    );

    // Clothoid
    gl::create_buffer(&m_cloth_vertices);
    m_cloth_ready = false;

    gl::link_program(&m_cloth_program,
        "shaders/aimer_demo_cloth_vx.glsl", "shaders/uniform4_ft.glsl");
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");

    glUseProgram(m_cloth_program);
    glUniform4f(m_cloth_color_uniform, 0, 0, 0, 1);

    // Aimer grid and samples

    gl::link_program(&m_aimer_program, "shaders/mat3_pos2_vx.glsl", "shaders/uniform3_ft.glsl");
    m_aimer_matrix_uniform = gl::get_uniform_location(m_aimer_program, "matrix");
    m_aimer_color_uniform = gl::get_uniform_location(m_aimer_program, "color");
    m_aimer_position_attrib = gl::get_attrib_location(m_aimer_program, "position");

    // Grid

    std::vector<vec2f> grid_vertices;
    grid_vertices.reserve(4 * (ClothoidAimer::GRID_SIZE + 1));
    for (int i = 0; i <= ClothoidAimer::GRID_SIZE; ++i)
    {
        float t = lerp(-1.0f, 1.0f, float(i) / float(ClothoidAimer::GRID_SIZE));
        grid_vertices.emplace_back( t, -1);
        grid_vertices.emplace_back( t,  1);
        grid_vertices.emplace_back(-1,  t);
        grid_vertices.emplace_back( 1,  t);
    }
    gl::create_buffer(&m_grid_vertices, grid_vertices);

    // Samples

    const float SAMPLE_SIZE = 0.02f;
    std::vector<vec2f> sample_vertices;
    for (const ClothoidAimer::Sample& sample : m_aimer.m_samples)
    {
        float x = sample.p.x, y = sample.p.y;
        sample_vertices.emplace_back(x - SAMPLE_SIZE, y);
        sample_vertices.emplace_back(x + SAMPLE_SIZE, y);
        sample_vertices.emplace_back(x, y - SAMPLE_SIZE);
        sample_vertices.emplace_back(x, y + SAMPLE_SIZE);
    }
    gl::create_buffer(&m_sample_vertices, sample_vertices);

    aim();
}

void AimerDemo::on_close()
{
    m_cloth_ready = false;

    gl::delete_program(&m_cloth_program);
    gl::delete_program(&m_aimer_program);
    gl::delete_buffer(&m_cloth_vertices);
    gl::delete_buffer(&m_sample_vertices);
    gl::delete_buffer(&m_grid_vertices);

    TwRemoveVar(twbar(), "Iters");
    TwRemoveVar(twbar(), "s");
    TwRemoveVar(twbar(), "a");

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
    real theta0 = tangent_angle();
    real kappa0 = rl(1) / rl(arc_radius());
    m_aim_result = m_aimer.aim(origin(), theta0, kappa0, target());
    real a = m_aim_result.a;
    real s = m_aim_result.s;
    m_a = float(a); m_s = float(s);

    m_aim_eval = origin() + arc_radius() * Fresnel::eval(theta0, 1, a, s); // TODO

    std::vector<vec2f> cloth_vertices;
    for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
    {
        real si = i * s / (CLOTHOID_VERTEX_COUNT - 1);
        cloth_vertices.push_back(
            origin() + arc_radius() * Fresnel::eval(theta0, 1, a, si)
        );
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glBufferData(GL_ARRAY_BUFFER, cloth_vertices);
    m_cloth_ready = true;
}

} // namespace kletch
