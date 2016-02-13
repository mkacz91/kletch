#include "control_point_overlay.h"

namespace kletch {

const int ControlPointOverlay::CIRCLE_SEGMENT_COUNT = 12;
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
        if (m_points[i].get() == point)
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

    for (const auto& point : m_points)
    {
        vec2f canvas_pos = m_camera->to_canvas(*point);
        vec2f translation = 2 * canvas_pos / m_camera->size - 1;
        vec2f scale = 2 * m_point_radius / m_camera->size;
        glUniform4f(m_point_transform_uniform,
            translation.x, -translation.y,
            scale.x, scale.y
        );
        if (m_selected_points.count(point.get()))
            glUniform4f(m_point_color_uniform, 1, 0, 0, 1);
        else if (m_highlighted_point == point.get())
            glUniform4f(m_point_color_uniform, 1, 1, 0, 1);
        else
            glUniform4f(m_point_color_uniform, 0, 0, 0, 1);
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

vec2f* ControlPointOverlay::pick_point(const vec2i& canvas_pos)
{
    vec2f world_pos = m_camera->to_world(canvas_pos);
    float threshold = sq(m_point_radius);
    for (auto& point : m_points)
    {
        vec2f r = m_camera->to_canvas_vector(world_pos - *point);
        if (r.length_sq() <= threshold)
            return point.get();
    }
    return nullptr;
}

void ControlPointOverlay::handle_event(const DemoEvent& e)
{
    switch (e.type()) {
    case SDL_MOUSEBUTTONDOWN:
    {
        if (e.button().button == SDL_BUTTON_LEFT)
        {
            vec2f* point = pick_point(e.button().x, e.button().y);
            if (point != nullptr)
            {
                m_selected_points.insert(point);
                m_prev_mouse_world_pos = m_camera->to_world(e.button().x, e.button().y);
                e.request_redraw();
                e.mark_handled();
            }
        }
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        if (!m_selected_points.empty() && e.button().button == SDL_BUTTON_LEFT)
        {
            m_selected_points.clear();
            m_highlighted_point = pick_point(e.button().x, e.button().y);
            e.mark_handled();
            e.request_redraw();
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        if (!m_selected_points.empty())
        {
            vec2f mouse_world_pos = m_camera->to_world(e.button().x, e.button().y);
            vec2f translation = mouse_world_pos - m_prev_mouse_world_pos;
            for (vec2f* point : m_selected_points)
                *point += translation;
            m_prev_mouse_world_pos = mouse_world_pos;
            e.mark_handled();
            e.request_redraw();
        }
        else
        {
            vec2f* point = pick_point(e.button().x, e.button().y);
            if (point != m_highlighted_point)
            {
                m_highlighted_point = point;
                e.request_redraw();
            }
        }
        break;
    }};
}

void ControlPointOverlay::highlight_points(const vec2i& canvas_pos)
{

}

} // namespace kletch
