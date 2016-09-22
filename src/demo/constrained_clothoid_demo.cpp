#include "constrained_clothoid_demo.h"
#include "math/printing.h"

namespace kletch {

ConstrainedClothoidDemo::ConstrainedClothoidDemo(const string& display_name) :
    ControlOverlay(display_name)
{
    set_camera(&m_camera);

    add_point(&m_origin);
    add_point(&m_tangent_tip);
    add_vector(&m_origin, &m_tangent_tip);
    //m_control_overlay.add_point(&m_local_arc_end);
    add_point(&m_arc_end);

    update_local_arc();
}

void ConstrainedClothoidDemo::on_render()
{
    ensure_arc_vertices_valid();
    mat3f arc_matrix = eye3f()
        .rotate(m_tangent_angle)
        .translate(m_origin)
        .premul(m_camera.matrix());
    glUseProgram(m_arc_program);
    glUniformMatrix3fv(m_arc_matrix_uniform, arc_matrix);
    glBindBuffer(GL_ARRAY_BUFFER, m_arc_vertices);
    glVertexAttribPointer(m_arc_position_param_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_arc_position_param_attrib);
    glDrawArrays(GL_LINE_STRIP, 0, ARC_VERTEX_COUNT);
    glDisableVertexAttribArray(m_arc_position_param_attrib);

    ControlOverlay::on_render();
}

bool ConstrainedClothoidDemo::on_event(Event const& e)
{
    if (m_camera.on_event(e))
    {
        invalidate();
        return true;
    }

    vec2f origin = m_origin;
    vec2f tangent_tip = m_tangent_tip;
    vec2f arc_end = m_arc_end;
    if (ControlOverlay::on_event(e))
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
        invalidate();
        return true;
    }

    return false;
}

void ConstrainedClothoidDemo::on_open()
{
    ControlOverlay::on_open();

    glClearColor(0.9f, 0.9f, 0.9f, 0);
    m_camera.open_grid();
    m_camera.set_size(width(), height());

    // Arc

    gl::create_buffer(&m_arc_vertices);
    invalidate_arc_vertices();

    gl::link_program(&m_arc_program, "constrained_clothoid_demo_arc_vx", "varying3_ft");
    m_arc_matrix_uniform = gl::get_uniform_location(m_arc_program, "matrix");
    m_arc_color0_uniform = gl::get_uniform_location(m_arc_program, "color0");
    m_arc_color1_uniform = gl::get_uniform_location(m_arc_program, "color1");
    m_arc_position_param_attrib = gl::get_attrib_location(m_arc_program, "position_param");

    glUseProgram(m_arc_program);
    glUniform3f(m_arc_color0_uniform, 0, 0, 0);
    glUniform3f(m_arc_color1_uniform, 0.9f, 0.9f, 0.9f);
}

void ConstrainedClothoidDemo::on_close()
{
    gl::delete_buffer(&m_arc_vertices);
    gl::delete_program(&m_arc_program);

    m_camera.close_grid();

    clear_points();
    ControlOverlay::on_close();
}

void ConstrainedClothoidDemo::update_local_arc()
{
    m_local_arc_end = to_tangent_coords(m_arc_end);
    if (m_local_arc_end.x > 0)
    {
        m_local_arc_end.x = 0;
        update_arc();
    }

    float dx = abs(m_local_arc_end.x), dy = abs(m_local_arc_end.y);
    m_arc_radius = 0.5f * (sq(dx) / dy + dy);
    m_initial_curvature = copysign(1.0f / m_arc_radius, m_local_arc_end.y);

    invalidate_arc_vertices();
}

void ConstrainedClothoidDemo::update_arc()
{
    m_arc_end = from_tangent_coords(m_local_arc_end);
}

void ConstrainedClothoidDemo::ensure_arc_vertices_valid()
{
    if (m_arc_vertices_valid)
        return;

    // The arc is approximated by polyline. Subsequent edges are retrieved through rotation by
    // a constant angle. The first edge is rotated by half the angle from the x axis.

    float arc_span = 2.0f * atan2(m_local_arc_end.y, -m_local_arc_end.x);
    float half_step_angle = arc_span / (2.0f * (ARC_VERTEX_COUNT - 1));
    mat2f half_step_rotation = eye2f().rotate(-half_step_angle);
    mat2f step_rotation = half_step_rotation * half_step_rotation;
    vec2f step = -unx2f();

    struct Vertex { vec2f position; float param; };
    std::vector<Vertex> vertices;
    vertices.reserve(ARC_VERTEX_COUNT);

    vertices.push_back({ 0, 0 });
    step = half_step_rotation.transform(step);
    for (int i = 1; i < ARC_VERTEX_COUNT; ++i)
    {
        vertices.push_back({ vertices.back().position + step, (float)i / (ARC_VERTEX_COUNT - 1) });
        step = step_rotation.transform(step);
    }

    // Rescale to match last vertex with the control point.
    float scale = len(m_local_arc_end) / len(vertices.back().position);
    for (Vertex& vertex : vertices)
        vertex.position *= scale;

    glBindBuffer(GL_ARRAY_BUFFER, m_arc_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices);
    m_arc_vertices_valid = true;
}

vec2f ConstrainedClothoidDemo::to_tangent_coords(vec2f const& v) const
{
    vec2f unx = dir(m_origin, m_tangent_tip).fix(unx2f());
    return (v - m_origin).in(unx, lhp(unx));
}

vec2f ConstrainedClothoidDemo::from_tangent_coords(const vec2f& v) const
{
    vec2f unx = dir(m_origin, m_tangent_tip).fix(unx2f());
    return m_origin + v.of(unx, lhp(unx));
}

} // namespace kletch
