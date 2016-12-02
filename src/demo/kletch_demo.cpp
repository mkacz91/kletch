#include "kletch_demo.h"

namespace kletch {

KletchDemo::KletchDemo() : Demo("Sketchpad")
{

}

bool KletchDemo::on_event(Event const& e)
{
    if (m_camera.on_event(e))
    {
        invalidate();
        return true;
    }

    switch (e.type) {
    case MOUSE_PRESS:
    {
        if (e.button != MOUSE_LEFT)
            return false;
        m_mouse_down = true;
        clear();
        put_sample(m_camera.canvas2world(e.pos));
        return true;
    }
    case MOUSE_RELEASE:
    {
        if (e.button != MOUSE_LEFT)
            return false;
        m_mouse_down = false;
        return true;
    }
    case MOUSE_MOVE:
    {
        if (!m_mouse_down)
            return false;
        put_sample(m_camera.canvas2world(e.pos));
        return true;
    }}

    return false;
}

void KletchDemo::on_render()
{
    glClearColor(1.0f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    update_buffers();

    glUseProgram(m_stroke_program);
    glUniformMatrix3fv(m_stroke_matrix_uniform, m_camera.matrix());
    glBindBuffer(GL_ARRAY_BUFFER, m_stroke_vertices);
    glVertexAttribPointer(m_stroke_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_stroke_position_attrib);
    glDrawArrays(GL_LINE_STRIP, 0, m_samples.size());
    glDisableVertexAttribArray(m_stroke_position_attrib);
}

void KletchDemo::on_open()
{
    m_mouse_down = false;
    m_buffered_sample_count = 0;

    gl::link_program(&m_stroke_program, "mat3_pos2_vx", "uniform3_ft");
    glUseProgram(m_stroke_program);
    m_stroke_matrix_uniform = gl::get_uniform_location(m_stroke_program, "matrix");
    m_stroke_color_uniform = gl::get_uniform_location(m_stroke_program, "color");
    m_stroke_position_attrib = gl::get_attrib_location(m_stroke_program, "position");
    glUniform3f(m_stroke_color_uniform, 0.5f, 0.5f, 0.5f);

    gl::create_buffer(&m_stroke_vertices);
}

void KletchDemo::on_close()
{
    gl::delete_buffer(&m_stroke_vertices);
    gl::delete_program(&m_stroke_program);
}

void KletchDemo::clear()
{
    m_samples.clear();
    invalidate();
}

void KletchDemo::put_sample(vec2f position)
{
    m_samples.push_back(position);
    invalidate();
}

void KletchDemo::update_buffers()
{
    if (m_buffered_sample_count == m_samples.size())
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_stroke_vertices);
    glBufferData(GL_ARRAY_BUFFER, m_samples);

    m_buffered_sample_count = m_samples.size();
}

} // namespace kletch
