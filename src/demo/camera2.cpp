#include "camera2.h"

namespace kletch {

void Camera2::set_uniform(int location)
{
    glUniform4f(
        location,
        translation.x, translation.y,
        2 * scale.x / size.x , 2 * scale.y / size.y
    );
}

vec2f Camera2::to_world(const vec2f& canvas_pos) const
{
    return vec2f(
        (canvas_pos.x - size.x / 2) / scale.x - translation.x,
        (size.y / 2 - canvas_pos.y) / scale.y - translation.y
    );
}

vec2f Camera2::to_canvas(const vec2f& world_pos) const
{
    return vec2f(
        (world_pos.x + translation.x) * scale.x + size.x / 2,
        size.y / 2 - (world_pos.y + translation.y) * scale.y
    );
}

void Camera2::handle_event(const DemoEvent& e)
{
    switch (e.type()) {
    case SDL_MOUSEBUTTONDOWN:
    {
        switch (e.button().button) {
        case SDL_BUTTON_LEFT:
        {
            m_grab_position = vec2i(e.button().x, e.button().y);
            m_translation_at_grab = translation;
            m_dragging = true;
            e.mark_handled();
            break;
        }
        case SDL_BUTTON_WHEELUP:
        {
            scale *= 1.1f;
            e.mark_handled();
            e.request_redraw();
            break;
        }
        case SDL_BUTTON_WHEELDOWN:
        {
            scale /= 1.1f;
            e.mark_handled();
            e.request_redraw();
            break;
        }}
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        if (e.button().button == SDL_BUTTON_LEFT)
        {
            m_dragging = false;
            e.mark_handled();
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        vec2f ggg = vec2f(e.motion().x, e.motion().y);
        cout << to_world(ggg) << " " << scale << endl;
        cout << ggg - to_canvas(to_world(ggg)) << endl;
        if (m_dragging)
        {
            vec2i u(e.motion().x - m_grab_position.x, m_grab_position.y - e.motion().y);
            translation.x = m_translation_at_grab.x + u.x / scale.x;
            translation.y = m_translation_at_grab.y + u.y / scale.y;
            e.request_redraw();
            e.mark_handled();
        }
        break;
    }
    case SDL_VIDEORESIZE:
    {
        size.x = e.resize().w;
        size.y = e.resize().h;
        break;
    }}
}

void Camera2::open_grid()
{
    std::vector<float> grid_vertices = {
        -1, -1,
         0,  1,
         1, -1
    };
    m_grid_vertices = gl::create_buffer(GL_ARRAY_BUFFER, grid_vertices);
    m_grid_program = gl::link_program("shaders/grid_vx.glsl", "shaders/uniform_ft.glsl");
    m_grid_transform_uniform = gl::get_uniform_location(m_grid_program, "transform");
    m_grid_color_uniform = gl::get_uniform_location(m_grid_program, "color");
    m_grid_position_attrib = gl::get_attrib_location(m_grid_program, "position");
}

void Camera2::close_grid()
{
    assert(m_grid_program != 0);
    glDeleteBuffers(1, &m_grid_vertices); m_grid_vertices = 0;
    glDeleteProgram(m_grid_program); m_grid_program = 0;
}

void Camera2::render_grid()
{
    if (m_grid_program == 0)
    {
        throw std::runtime_error(
            "Camera2::draw_grid() must occur between open_grid() and close_grid()"
        );
    }

    if (size.x < 1 || size.y < 1)
        return;

    glUseProgram(m_grid_program);
    glBindBuffer(GL_ARRAY_BUFFER, m_grid_vertices);
    set_uniform(m_grid_transform_uniform);
    glUniform4f(m_grid_color_uniform, 1, 0, 0, 1);
    glVertexAttribPointer(m_grid_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_grid_position_attrib);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(m_grid_position_attrib);
}

} // namespace kletch
