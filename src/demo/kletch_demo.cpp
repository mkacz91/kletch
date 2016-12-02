#include "kletch_demo.h"
#include <lib/display_fresnel.h>
#include <lib/precise_fresnel.h> // TODO(mkc): THis must out!
#include "printing.h"

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
        {
            cout << "=======================================\n";
            for (auto& p : m_samples)
                cout << "{ " << p.x << "f, " << p.y << "f },\n";
            cout << endl;
            return false;
        }
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
        m_world_mouse_pos = m_camera.canvas2world(e.pos);
        if (!m_mouse_down)
            return false;
        put_sample(m_world_mouse_pos);
        return true;
    }
    case KEY_PRESS:
    {
        if (e.key == 'G')
        {
            m_advance = true;
            invalidate();
            return true;
        }
        if (e.key != 'A')
            return false;
        clear();
        m_samples = std::vector<vec2f> {
            { -0.42f, -0.36f },
            { -0.396667f, -0.36f },
            { -0.366667f, -0.36f },
            { -0.29f, -0.36f },
            { -0.223333f, -0.356667f },
            { -0.153333f, -0.34f },
            { -0.11f, -0.323333f },
            { -0.0666667f, -0.306667f },
            { -0.0333333f, -0.286667f },
            { 0.00666666f, -0.26f },
            { 0.0433334f, -0.233333f },
            { 0.0799999f, -0.203333f },
            { 0.113333f, -0.173333f },
            { 0.133333f, -0.15f },
            { 0.163333f, -0.103333f },
            { 0.176667f, -0.0833334f },
            { 0.19f, -0.0633334f },
            { 0.2f, -0.04f },
            { 0.203333f, -0.02f },
            { 0.203333f, 0.00333333f },
            { 0.203333f, 0.02f },
            { 0.203333f, 0.04f },
            { 0.203333f, 0.0566667f },
            { 0.2f, 0.08f },
            { 0.193333f, 0.0933333f },
            { 0.18f, 0.11f },
            { 0.163333f, 0.123333f },
            { 0.123333f, 0.14f },
            { 0.113333f, 0.14f },
            { 0.1f, 0.14f },
            { 0.09f, 0.14f },
            { 0.07f, 0.14f },
            { 0.0533333f, 0.14f },
            { 0.04f, 0.14f },
            { 0.0233334f, 0.13f },
            { 0.0133333f, 0.12f },
            { -0.00333333f, 0.103333f },
            { -0.02f, 0.0866666f },
            { -0.0233333f, 0.0733333f },
            { -0.0366666f, 0.0533333f },
            { -0.04f, 0.0266666f },
            { -0.0433334f, 0.0f },
            { -0.0433334f, -0.0266666f },
            { -0.0433334f, -0.05f },
            { -0.04f, -0.0700001f },
            { -0.0333333f, -0.09f },
            { -0.02f, -0.11f },
            { -0.00333333f, -0.133333f },
            { 0.0166667f, -0.153333f },
            { 0.0533333f, -0.17f },
            { 0.0766667f, -0.173333f },
            { 0.113333f, -0.176667f },
            { 0.146667f, -0.176667f },
            { 0.193333f, -0.176667f },
            { 0.243333f, -0.176667f },
            { 0.29f, -0.17f },
            { 0.346667f, -0.153333f },
            { 0.39f, -0.14f },
            { 0.423333f, -0.126667f },
            { 0.453333f, -0.106667f },
            { 0.466667f, -0.1f },
            { 0.476667f, -0.0966667f },
            { 0.486667f, -0.0933334f },
            { 0.49f, -0.09f },
            { 0.496667f, -0.09f }
        };
        return true;
    }}

    return false;
}

void KletchDemo::on_render()
{
    keep_up();

    glClearColor(1.0f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mat3f matrix = m_camera.matrix();

    // Draw the sample polyline
    glUseProgram(m_sample_program);
    glUniformMatrix3fv(m_sample_matrix_uniform, matrix);
    glUniform3f(m_sample_color_uniform, 0.5f, 0.5f, 0.5f);
    glBindBuffer(GL_ARRAY_BUFFER, m_sample_vertices);
    glVertexAttribPointer(m_sample_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glLineWidth(1);
    glEnableVertexAttribArray(m_sample_position_attrib);
    glDrawArrays(GL_LINE_STRIP, 0, m_sample_vertex_count);

    // Draw tangents (Use the sample program, whatever)
    glUniform3f(m_sample_color_uniform, 0.9f, 0.1f, 0.1f);
    glBindBuffer(GL_ARRAY_BUFFER, m_tangent_vertices);
    glVertexAttribPointer(m_sample_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 4 * m_segment_count);
    glDisableVertexAttribArray(m_sample_position_attrib);

    // Draw the spline
    glUseProgram(m_spline_program);
    glUniformMatrix3fv(m_spline_matrix_uniform, matrix);
    glBindBuffer(GL_ARRAY_BUFFER, m_spline_vertices);
    glVertexAttribPointer(m_spline_position_parity_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glLineWidth(2);
    glEnableVertexAttribArray(m_spline_position_parity_attrib);
    glDrawArrays(GL_LINE_STRIP, 0, m_spline_vertex_count);
    glDisableVertexAttribArray(m_spline_position_parity_attrib);
    glLineWidth(1);
}

void KletchDemo::on_open()
{
    clear();
    m_mouse_down = false;

    // Alter Twbar
    TwAddVarRO(twbar(), "x", TW_TYPE_FLOAT, &m_world_mouse_pos.x, nullptr);
    TwAddVarRO(twbar(), "y", TW_TYPE_FLOAT, &m_world_mouse_pos.y, nullptr);

    // Init sample program
    gl::link_program(&m_sample_program, "mat3_pos2_vx", "uniform3_ft");
    glUseProgram(m_sample_program);
    m_sample_matrix_uniform = gl::get_uniform_location(m_sample_program, "matrix");
    m_sample_color_uniform = gl::get_uniform_location(m_sample_program, "color");
    m_sample_position_attrib = gl::get_attrib_location(m_sample_program, "position");

    // Init spline program
    gl::link_program(&m_spline_program, "kletch_demo_spline_vx", "varying3_ft");
    glUseProgram(m_spline_program);
    m_spline_matrix_uniform = gl::get_uniform_location(m_spline_program, "matrix");
    m_spline_even_color_uniform = gl::get_uniform_location(m_spline_program, "even_color");
    m_spline_odd_color_uniform = gl::get_uniform_location(m_spline_program, "odd_color");
    m_spline_position_parity_attrib = gl::get_attrib_location(m_spline_program, "position_parity");
    glUniform3f(m_spline_even_color_uniform, 0.2f, 0.2f, 0.5f);
    glUniform3f(m_spline_odd_color_uniform, 0.5f, 0.5f, 0.8f);

    gl::create_buffer(&m_sample_vertices);
    gl::create_buffer(&m_spline_vertices);
    gl::create_buffer(&m_tangent_vertices);
}

void KletchDemo::on_close()
{
    gl::delete_buffer(&m_tangent_vertices);
    gl::delete_buffer(&m_spline_vertices);
    gl::delete_buffer(&m_sample_vertices);
    gl::delete_buffer(&m_spline_program);
    gl::delete_program(&m_sample_program);

    TwRemoveVar(twbar(), "x");
    TwRemoveVar(twbar(), "y");
}

void KletchDemo::clear()
{
    m_advance = false;
    m_segment_count = 0;

    m_samples.clear();
    m_pushed_sample_count = 0;
    m_sample_vertex_count = -1;

    m_spline_vertices_local.clear();
    m_spline_vertex_count = -1;

    m_tangent_vertices_local.clear();

    m_kletch.clear();

    invalidate();
}

void KletchDemo::keep_up()
{
    while (m_pushed_sample_count < m_samples.size())
        m_kletch.push(m_samples[m_pushed_sample_count++]);

    Kletch::Segment const* segment;
    if (m_advance && (segment = m_kletch.try_advance()))
    {
        m_advance = false;
        float th0 = segment->th0, k0 = segment->k0, k1 = segment->k1, s = segment->s;
        vec2f p0 = segment->p0, p1 = segment->p1;
        mat3f matrix = eye3f()
            //.map(DisplayFresnel::eval(0, k0, k1, s), p1 - p0)
            .rotate(th0)
            .translate(p0);
        
        float parity = (float)(m_segment_count % 2);
        for (int i = 0; i < SEGMENT_VERTEX_COUNT; ++i)
        {
            float si = i * s / (SEGMENT_VERTEX_COUNT - 1);
            vec2f position = matrix.tform(PreciseFresnel::eval(k0, k1, si));
            m_spline_vertices_local.push_back({ position, parity });
        }

        float th1 = th0 + s * (k0 + 0.5f * s * k1);
        m_tangent_vertices_local.push_back(p0);
        m_tangent_vertices_local.push_back(p0 + 0.2f * dir(th0));
        m_tangent_vertices_local.push_back(p1);
        m_tangent_vertices_local.push_back(p1 - 0.2f * dir(th1));

        ++m_segment_count;
    }
    
    update_buffers();
}

void KletchDemo::put_sample(vec2f position)
{
    m_samples.push_back(position);
    invalidate();
}

void KletchDemo::update_buffers()
{
    if (m_sample_vertex_count != m_samples.size())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_sample_vertices);
        glBufferData(GL_ARRAY_BUFFER, m_samples);
        m_sample_vertex_count = m_samples.size();
    }

    if (m_spline_vertex_count != m_spline_vertices_local.size())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_spline_vertices);
        glBufferData(GL_ARRAY_BUFFER, m_spline_vertices_local);

        glBindBuffer(GL_ARRAY_BUFFER, m_tangent_vertices);
        glBufferData(GL_ARRAY_BUFFER, m_tangent_vertices_local);

        m_spline_vertex_count = m_spline_vertices_local.size();
    }
}

} // namespace kletch
