#include "fitter_demo.h"

#include <lib/display_fresnel.h>
#include <lib/precise_fresnel.h>

namespace kletch {

FitterDemo::FitterDemo() : ControlOverlay("Clothoid Fit")
{
    set_camera(&m_camera);
}

FitterDemo::~FitterDemo()
{
    for (vec2f* sample : m_samples)
        delete sample;
}

bool FitterDemo::on_event(Event const& e)
{
    auto handled = [=]()
    {
        fit();
        invalidate();
        return true;
    };

    if (m_camera.on_event(e) || ControlOverlay::on_event(e))
        return handled();

    switch (e.type) {
    case MOUSE_PRESS:
    {
        if (e.button != MOUSE_LEFT)
            return false;
        m_samples.push_back(new vec2f(m_camera.canvas2world(e.pos)));
        add_point(m_samples.back());
        return handled();
    }
    default:
    {
        break;
    }}

    return false;
}

void FitterDemo::on_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_cloth_program);
    glUniformMatrix3fv(m_cloth_matrix_uniform, m_camera.matrix());
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glVertexAttribPointer(m_cloth_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_cloth_position_attrib);
    glUniform4f(m_cloth_color_uniform, 0.6f, 0.0f, 0.0f, 1);
    glDrawArrays(GL_LINE_STRIP, 0, CLOTHOID_VERTEX_COUNT);
    glDisableVertexAttribArray(m_cloth_position_attrib);

    ControlOverlay::on_render();
}

void FitterDemo::on_open()
{
    ControlOverlay::on_open();

    gl::create_buffer(&m_cloth_vertices);

    gl::link_program(&m_cloth_program, "aimer_demo_cloth_vx", "uniform4_ft");
    m_cloth_matrix_uniform = gl::get_uniform_location(m_cloth_program, "matrix");
    m_cloth_color_uniform = gl::get_uniform_location(m_cloth_program, "color");
    m_cloth_position_attrib = gl::get_attrib_location(m_cloth_program, "position");

    fit();
}

void FitterDemo::on_close()
{
    gl::delete_program(&m_cloth_program);
    gl::delete_buffer(&m_cloth_vertices);

    ControlOverlay::on_close();
}

void FitterDemo::fit()
{
    // Fit clothoid to sample points
    m_fitter.clear();
    for (vec2f const* sample : m_samples)
        m_fitter.push(*sample);
    auto result = m_fitter.fit();
    float k0 = result.k0, k1 = result.k1, s = result.s;

    // Adjust transformation matrix to match ends of precise and display evaluation
    vec2f display_eval = DisplayFresnel::eval(0, k0, k1, s);
    vec2r precise_eval = PreciseFresnel::eval(result.k0, result.k1, result.s);
    mat3f matrix = eye3f().map(display_eval, precise_eval);//.premul(result.matrix);

    // Evaluate resulting clothoid for rendering
    std::vector<vec2f> cloth_vertices; cloth_vertices.reserve(CLOTHOID_VERTEX_COUNT);
    for (int i = 0; i < CLOTHOID_VERTEX_COUNT; ++i)
    {
        float si = (i * float(result.s)) / (CLOTHOID_VERTEX_COUNT - 1);
        cloth_vertices.push_back(matrix.tform(DisplayFresnel::eval(0, k0, k1, si)));
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_cloth_vertices);
    glBufferData(GL_ARRAY_BUFFER, cloth_vertices);
}

} // namespace kletch
