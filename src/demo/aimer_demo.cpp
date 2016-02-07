#include "aimer_demo.h"
#include <vector>

namespace kletch {

const int AimerDemo::CIRCLE_SEGMENT_COUNT = 24;
const int AimerDemo::POINT_VERTEX_COUNT = CIRCLE_SEGMENT_COUNT + 5;
const float AimerDemo::POINT_RADIUS = 20;
const float AimerDemo::TAU = 6.28318530718f;

void AimerDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_point_program);
    glUniform4f(
        m_point_transform_uniform,
        point.x + 0.5f / width(), point.y + 0.5f / height(),
        POINT_RADIUS / width(), POINT_RADIUS / height()
    );
    glBindBuffer(GL_ARRAY_BUFFER, m_point_vertices);
    glVertexAttribPointer(m_point_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_point_position_attrib);

    glDrawArrays(GL_LINE_STRIP, 0, POINT_VERTEX_COUNT);
}

void AimerDemo::handle_event(const DemoEvent& e)
{

}

void AimerDemo::open()
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
    m_point_position_attrib = glGetAttribLocation(m_point_program, "position");
}

void AimerDemo::close() noexcept
{
    glDeleteProgram(m_point_program); m_point_program = 0;
    glDeleteBuffers(1, &m_point_vertices); m_point_vertices = 0;
}

} // namespace kletch
