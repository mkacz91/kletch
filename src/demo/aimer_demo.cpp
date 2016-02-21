#include "aimer_demo.h"
#include <vector>
#include <lib/fresnel.h>

namespace kletch {

AimerDemo::AimerDemo() : ConstrainedClothoidDemo("Clothoid Aim")
{
    m_control_overlay.add_point(&m_aim_eval);
}

void AimerDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid

    glUseProgram(m_grid_program);
    glUniformMatrix3fv(m_grid_matrix_uniform, m_camera.matrix() * m_aimer.m_grid_box.matrix_to());
    glBindBuffer(GL_ARRAY_BUFFER, m_grid_vertices);
    glEnableVertexAttribArray(m_grid_position_attrib);
    glVertexAttribPointer(m_grid_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, 4 * (ClothoidAimer::GRID_SIZE + 1));

    glDisableVertexAttribArray(m_grid_position_attrib);

    // ----

    glUseProgram(m_cloth_program);
    glUniformMatrix3fv(m_cloth_matrix_uniform, m_camera.matrix());
    glEnableVertexAttribArray(m_cloth_position_attrib);

    // Draw samples

    glUniform4f(m_cloth_color_uniform, 0.5f, 0.5f, 0.8f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, m_sample_vertices);
    glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, m_aimer.m_samples.size() * 4);

    // Draw clothoid
    if (m_cloth_ready)
    {
        glUniform4f(m_cloth_color_uniform, 1, 0, 0, 1);
        glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
        glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, CLOTHOID_VERTEX_COUNT);
    }

    glDisableVertexAttribArray(m_cloth_position_attrib);

    ConstrainedClothoidDemo::render();
}

void AimerDemo::handle_event(const DemoEvent& e)
{
    ConstrainedClothoidDemo::handle_event(e);
    if (e.handled())
    {
        real theta0 = tangent_angle();
        real kappa0 = rl(1) / rl(arc_radius());
        m_aim_result = m_aimer.aim(origin(), theta0, kappa0, target());
        real a = m_aim_result.a;
        real s = m_aim_result.s;

        //cout << theta0 << " " << kappa0 << " " << a << " " << s << endl;
        m_aim_eval = origin() + Fresnel::eval(theta0, kappa0, a, s);

        vector<vec2f> cloth_vertices;
        for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
        {
            real si = i * s / (CLOTHOID_VERTEX_COUNT - 1);
            cloth_vertices.push_back(origin() + Fresnel::eval(theta0, kappa0, a, si));
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
        glBufferData(GL_ARRAY_BUFFER, cloth_vertices);
        m_cloth_ready = true;
        return;
    }
}

void AimerDemo::open()
{
    ConstrainedClothoidDemo::open();

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
    m_sample_vertices = gl::create_buffer(GL_ARRAY_BUFFER, sample_vertices);

    // Clothoid

    glGenBuffers(1, &m_cloth_vertices);
    m_cloth_ready = false;

    m_cloth_program = gl::link_program(
        "shaders/aimer_demo_cloth_vx.glsl",
        "shaders/uniform4_ft.glsl"
    );
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");

    glUseProgram(m_cloth_program);
    glUniform4f(m_cloth_color_uniform, 0, 0, 0, 1);

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
    m_grid_vertices = gl::create_buffer(GL_ARRAY_BUFFER, grid_vertices);

    m_grid_program = gl::link_program("shaders/mat3_pos2_vx.glsl", "shaders/uniform3_ft.glsl");
    m_grid_matrix_uniform = gl::get_uniform_location(m_grid_program, "matrix");
    m_grid_color_uniform = gl::get_uniform_location(m_grid_program, "color");
    m_grid_position_attrib = gl::get_attrib_location(m_grid_program, "position");

    glUseProgram(m_grid_program);
    glUniform3f(m_grid_color_uniform, 0.8f, 0.8f, 0.8f);
}

void AimerDemo::close() noexcept
{
    m_cloth_ready = false;
    glDeleteProgram(m_cloth_program); m_cloth_program = 0;
    glDeleteBuffers(1, &m_cloth_vertices); m_cloth_vertices = 0;
    glDeleteBuffers(1, &m_sample_vertices); m_sample_vertices = 0;

    ConstrainedClothoidDemo::close();
}

} // namespace kletch
