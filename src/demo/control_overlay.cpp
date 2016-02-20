#include "control_overlay.h"

namespace kletch {

const int ControlOverlay::CIRCLE_SEGMENT_COUNT = 12;
const float ControlOverlay::TAU = 6.28318530718f;
const int_range ControlOverlay::POINT_VERTEX_RANGE = { 0, CIRCLE_SEGMENT_COUNT + 5 };
const int_range ControlOverlay::VECTOR_EDGE_VERTEX_RANGE
    = { ControlOverlay::POINT_VERTEX_RANGE.end(), 2 };
const int_range ControlOverlay::VECTOR_CAP_VERTEX_RANGE
    = { ControlOverlay::VECTOR_EDGE_VERTEX_RANGE.end(), 3 };

int ControlOverlay::add_point(vec2f* point)
{
    m_points.push_back(point);
    return m_points.size() - 1;
}

int ControlOverlay::add_vector(vec2f* start, vec2f* end)
{
    m_vectors.emplace_back(start, end);
    return m_vectors.size() - 1;
}

int ControlOverlay::point_index(const vec2f* point) const
{
    for (int i = 0; i < m_points.size(); ++i)
    {
        if (m_points[i] == point)
            return i;
    }

    throw std::invalid_argument("Given point is not member of this ControlOverlay");
}

void ControlOverlay::render()
{
    if (m_point_program == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);

    // Render points
    glUseProgram(m_point_program);
    glUniformMatrix3fv(m_point_origin_matrix_uniform, m_camera->matrix());
    glUniform2f(m_point_offset_scale_uniform, 2 * m_point_radius / m_camera->size());
    glVertexAttribPointer(m_point_offset_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_point_offset_attrib);

    for (vec2f* point : m_points)
    {
        glUniform2f(m_point_origin_uniform, *point);
        if (m_selected_points.count(point))
            glUniform4f(m_point_color_uniform, 1, 0, 0, 1);
        else if (m_highlighted_point == point)
            glUniform4f(m_point_color_uniform, 1, 1, 0, 1);
        else
            glUniform4f(m_point_color_uniform, 0, 0, 0, 1);
        glDrawArrays(GL_LINE_STRIP, POINT_VERTEX_RANGE);
    }

    glDisableVertexAttribArray(m_point_offset_attrib);

    // Render vector edges
    glUseProgram(m_vector_edge_program);
    glVertexAttribPointer(m_vector_edge_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_vector_edge_position_attrib);

    for (auto& v : m_vectors)
    {
        vec2f p0 = m_camera->matrix().transform(*get<0>(v));
        vec2f p1 = m_camera->matrix().transform(*get<1>(v));
        glUniform4f(m_vector_edge_transform_uniform, p0.x, p0.y, p1.x, p1.y);
        glUniform4f(m_vector_edge_color_uniform, 0, 0, 0, 1);
        glDrawArrays(GL_LINES, VECTOR_EDGE_VERTEX_RANGE);
    }

    glDisableVertexAttribArray(m_vector_edge_position_attrib);

    // Render vector caps
    glUseProgram(m_vector_cap_program);
    glVertexAttribPointer(m_vector_cap_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_vector_cap_position_attrib);

    for (auto& v : m_vectors)
    {
        // TODO
    }

    glDisableVertexAttribArray(m_vector_cap_position_attrib);
}

void ControlOverlay::open()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    // Initialize vertex buffer

    std::vector<vec2f> vertices;

    // Point circle
    for (int i = 0; i <= CIRCLE_SEGMENT_COUNT; ++i)
    {
        float angle = TAU * i / CIRCLE_SEGMENT_COUNT;
        vertices.emplace_back(cos(angle), sin(angle));
    }

    // Point cross
    vertices.emplace_back(-1,  0);
    vertices.emplace_back( 0,  0);
    vertices.emplace_back( 0,  1);
    vertices.emplace_back( 0, -1);

    // Vector edge
    vertices.emplace_back(0, 0);
    vertices.emplace_back(1, 1);

    // Vector cap
    vertices.emplace_back( 0.0f,  0.0f);
    vertices.emplace_back(-1.0f,  0.3f);
    vertices.emplace_back(-1.0f, -0.3f);

    m_vertices = gl::create_buffer(GL_ARRAY_BUFFER, vertices);

    // Initialize shader programs

    // Point program
    m_point_program = gl::link_program("shaders/aimer_point_vx.glsl", "shaders/uniform_ft.glsl");
    m_point_origin_matrix_uniform = gl::get_uniform_location(m_point_program, "origin_matrix");
    m_point_origin_uniform = gl::get_uniform_location(m_point_program, "origin");
    m_point_offset_scale_uniform = gl::get_uniform_location(m_point_program, "offset_scale");
    m_point_color_uniform = gl::get_uniform_location(m_point_program, "color");
    m_point_offset_attrib = gl::get_attrib_location(m_point_program, "offset");

    // Vector edge program
    m_vector_edge_program = gl::link_program(
        "shaders/control_overlay_vector_edge_vx.glsl",
        "shaders/uniform_ft.glsl"
    );
    m_vector_edge_transform_uniform = gl::get_uniform_location(m_vector_edge_program, "transform");
    m_vector_edge_color_uniform = gl::get_uniform_location(m_vector_edge_program, "color");
    m_vector_edge_position_attrib = gl::get_attrib_location(m_vector_edge_program, "position");

    // Vectro cap program
    m_vector_cap_program = gl::link_program(
        "shaders/control_overlay_vector_cap_vx.glsl",
        "shaders/uniform_ft.glsl"
    );
    m_vector_cap_transform_uniform = gl::get_uniform_location(m_vector_cap_program, "transform");
    m_vector_cap_color_uniform = gl::get_uniform_location(m_vector_cap_program, "color");
    m_vector_cap_position_attrib = gl::get_attrib_location(m_vector_cap_program, "position");
}

void ControlOverlay::close()
{
    glDeleteProgram(m_vector_cap_program); m_vector_cap_program = 0;
    glDeleteProgram(m_vector_edge_program); m_vector_edge_program = 0;
    glDeleteProgram(m_point_program); m_point_program = 0;
    glDeleteBuffers(1, &m_vertices); m_vertices = 0;
}

vec2f* ControlOverlay::pick_point(const vec2i& canvas_pos)
{
    vec2f world_pos = m_camera->canvas_to_world(canvas_pos);
    float threshold = sq(2 * m_point_radius / (m_camera->scale() * m_camera->size().y));
    for (auto& point : m_points)
    {
        if (dist_sq(world_pos, *point) <= threshold)
            return point;
    }
    return nullptr;
}

void ControlOverlay::handle_event(const DemoEvent& e)
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
                m_prev_mouse_world_pos = m_camera->canvas_to_world(
                    e.button().x,
                    e.button().y
                );
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
            vec2f mouse_world_pos = m_camera->canvas_to_world(e.button().x, e.button().y);
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

void ControlOverlay::highlight_points(const vec2i& canvas_pos)
{

}

} // namespace kletch
