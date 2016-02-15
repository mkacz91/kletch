#include "constrained_clothoid_demo.h"

namespace kletch {

ConstrainedClothoidDemo::ConstrainedClothoidDemo(const string& display_name) :
    Demo(display_name)
{
    m_control_overlay.set_camera(&m_camera);
    update_local_arc();
}

void ConstrainedClothoidDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw arc
    glUseProgram(m_arc_program);
    m_camera.set_uniform(m_arc_transform_uniform);
    vec2f arc_center = from_tangent_coords(m_local_arc_center);
    glUniform3f(m_arc_center_r_uniform, arc_center.x, arc_center.y, m_arc_radius);
    glUniform2f(m_arc_angle_uniform,
        m_local_arc_angle0 + m_tangent_angle,
        m_local_arc_angle1 + m_tangent_angle
    );
    glBindBuffer(GL_ARRAY_BUFFER, m_arc_vertices);
    glVertexAttribPointer(m_arc_param_attrib, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_arc_param_attrib);

    glDrawArrays(GL_LINE_STRIP, 0, ARC_VERTEX_COUNT);

    glDisableVertexAttribArray(m_arc_param_attrib);

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

    vec2f origin = m_origin;
    vec2f tangent_tip = m_tangent_tip;
    vec2f arc_end = m_arc_end;
    m_control_overlay.handle_event(e);
    if (e.handled())
    {
        if (origin != m_origin)
            m_tangent_tip = tangent_tip + m_origin - origin;
        if (tangent_tip != m_tangent_tip)
        {
            m_tangent_angle = (m_tangent_tip - m_origin).angle();
            update_arc();
        }
        else if (arc_end != m_arc_end)
            update_local_arc();
        return;
    }
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
    //m_control_overlay.add_point(&m_local_arc_end);
    m_control_overlay.add_point(&m_arc_end);

    // Init arc

    std::vector<float> arc_vertices;
    arc_vertices.reserve(ARC_VERTEX_COUNT);
    for (int i = 0; i < ARC_VERTEX_COUNT; ++i)
        arc_vertices.push_back((float)i / (ARC_VERTEX_COUNT - 1));
    m_arc_vertices = gl::create_buffer(GL_ARRAY_BUFFER, arc_vertices);

    m_arc_program = gl::link_program(
        "shaders/constrained_clothoid_demo_arc_vx.glsl",
        "shaders/varying3_ft.glsl"
    );
    m_arc_transform_uniform = gl::get_uniform_location(m_arc_program, "transform");
    m_arc_center_r_uniform = gl::get_uniform_location(m_arc_program, "center_r");
    m_arc_angle_uniform = gl::get_uniform_location(m_arc_program, "angle");
    m_arc_color0_uniform = gl::get_uniform_location(m_arc_program, "color0");
    m_arc_color1_uniform = gl::get_uniform_location(m_arc_program, "color1");
    m_arc_param_attrib = gl::get_attrib_location(m_arc_program, "param");

    glUseProgram(m_arc_program);
    glUniform3f(m_arc_color0_uniform, 0, 0, 0);
    glUniform3f(m_arc_color1_uniform, 0.9f, 0.9f, 0.9f);
}

void ConstrainedClothoidDemo::close() noexcept
{
    glDeleteBuffers(1, &m_arc_vertices); m_arc_vertices = 0;
    glDeleteProgram(m_arc_program); m_arc_program = 0;

    m_control_overlay.clear_points();

    m_control_overlay.close();
    m_camera.close_grid();
}

void ConstrainedClothoidDemo::update_local_arc()
{
    m_local_arc_end = to_tangent_coords(m_arc_end);
    if (m_local_arc_end.x > 0)
    {
        m_local_arc_end.x = 0;
        update_arc();
        return;
    }

    float dx = abs(m_local_arc_end.x), dy = abs(m_local_arc_end.y);
    m_arc_radius = 0.5f * (sq(dx) / dy + dy);
    float arc_angle_span = atan2(dx, m_arc_radius - dy);
    m_local_arc_angle0 = -1.57079632679f;
    m_local_arc_angle1 = m_local_arc_angle0 - arc_angle_span;
    m_local_arc_center = vec2f(0, m_arc_radius);
    if (m_local_arc_end.y < 0)
    {
        m_local_arc_center.y = -m_local_arc_center.y;
        m_local_arc_angle0 = -m_local_arc_angle0;
        m_local_arc_angle1 = -m_local_arc_angle1;
    }
}

void ConstrainedClothoidDemo::update_arc()
{
    m_arc_end = from_tangent_coords(m_local_arc_end);
}

vec2f ConstrainedClothoidDemo::to_tangent_coords(vec2f v)
{
    vec2f unit_x = (m_tangent_tip - m_origin).normalized_safe();
    vec2f unit_y = vec2f(-unit_x.y, unit_x.x);
    v -= m_origin;
    return vec2f(dot(v, unit_x), dot(v, unit_y));
}

vec2f ConstrainedClothoidDemo::from_tangent_coords(const vec2f& v)
{
    vec2f unit_x = (m_tangent_tip - m_origin).normalized_safe();
    vec2f unit_y = vec2f(-unit_x.y, unit_x.x);
    return m_origin + v.x * unit_x + v.y * unit_y;
}

} // namespace kletch
