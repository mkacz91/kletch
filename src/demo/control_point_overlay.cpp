#include "control_point_overlay.h"

namespace kletch {

const int ControlPointOverlay::CIRCLE_SEGMENT_COUNT = 24;
const int ControlPointOverlay::POINT_VERTEX_COUNT = CIRCLE_SEGMENT_COUNT + 5;
const float ControlPointOverlay::TAU = 6.28318530718f;

vec2f* ControlPointOverlay::new_point()
{
    vec2f* point = new vec2f(0);
    m_points.emplace_back(point);
    return point;
}

int ControlPointOverlay::point_index(const vec2f* point) const
{
    for (int i = 0; i < m_points.size(); ++i)
    {
        if (m_points[i].position.get() == point)
            return i;
    }

    throw std::invalid_argument("Given point is not member of this ControlPointOverlay");
}

void ControlPointOverlay::render()
{
    if (m_point_program == 0)
        return;

    glUseProgram(m_point_program);
    glBindBuffer(GL_ARRAY_BUFFER, m_point_vertices);
    glVertexAttribPointer(m_point_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_point_position_attrib);

    for (const ControlPoint& point : m_points)
    {
        vec2f canvas_pos = m_camera->to_canvas(*point.position);
        vec2f translation = 2 * canvas_pos / m_camera->size - 1;
        vec2f scale = 2 * m_point_radius / m_camera->size;
        glUniform4f(m_point_transform_uniform,
            translation.x, -translation.y,
            scale.x, scale.y
        );
        glUniform4f(m_point_color_uniform, 0, 1, 0, 1);
        glDrawArrays(GL_LINE_STRIP, 0, POINT_VERTEX_COUNT);
    }

    glDisableVertexAttribArray(m_point_position_attrib);
}

void ControlPointOverlay::open()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    std::vector<vec2f> point_vertices;
    for (int i = 0; i <= CIRCLE_SEGMENT_COUNT; ++i)
    {
        float angle = TAU * i / CIRCLE_SEGMENT_COUNT;
        point_vertices.emplace_back(cos(angle), sin(angle));
    }
    point_vertices.emplace_back(-1,  0);
    point_vertices.emplace_back( 0,  0);
    point_vertices.emplace_back( 0,  1);
    point_vertices.emplace_back( 0, -1);

    glGenBuffers(1, &m_point_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_point_vertices);
    glBufferData(GL_ARRAY_BUFFER, point_vertices);

    m_point_program = gl::link_program("shaders/aimer_point_vx.glsl", "shaders/uniform_ft.glsl");
    m_point_transform_uniform = glGetUniformLocation(m_point_program, "transform");
    m_point_color_uniform = glGetUniformLocation(m_point_program, "color");
    m_point_position_attrib = glGetAttribLocation(m_point_program, "position");
}

void ControlPointOverlay::close()
{
    glDeleteProgram(m_point_program); m_point_program = 0;
    glDeleteBuffers(1, &m_point_vertices); m_point_vertices = 0;
}

void ControlPointOverlay::handle_event(const DemoEvent& e)
{

}

} // namespace kletch
