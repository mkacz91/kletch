#include "camera2.h"

namespace kletch {

bool Camera2::on_event(SDL_Event const& e)
{
    switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
    {
        if (e.button.button == SDL_BUTTON_RIGHT)
        {
            m_grab_position = vec2i(e.button.x, e.button.y);
            m_translation_at_grab = m_translation;
            m_dragging = true;
            return true;
        }
    }
    case SDL_MOUSEWHEEL:
    {
        scale(pow(1.1, e.wheel.y));
        return true;
    }
    case SDL_MOUSEBUTTONUP:
    {
        if (e.button.button == SDL_BUTTON_RIGHT)
        {
            m_dragging = false;
            return true;
        }
        return false;
    }
    case SDL_MOUSEMOTION:
    {
        if (!m_dragging)
            return false;
        vec2i position = vec2i(e.motion.x, e.motion.y);
        set_translation(m_translation_at_grab + canvas2worldv(position - m_grab_position));
        return true;
    }
    case SDL_KEYDOWN:
    {
        switch (e.key.keysym.sym) {
        case SDLK_q:
        {
            rotate(0.1f);
            return true;
        }
        case SDLK_e:
        {
            rotate(-0.1f);
            return true;
        }}
        return false;
    }}
    return false;
}

void Camera2::open_grid()
{
    const int tick_count = GRID_PARTITION * sqrt(GRID_PARTITION);
    std::vector<vec2f> grid_vertices;
    grid_vertices.reserve(4 * (2 * tick_count + 1));
    for (int i = -tick_count; i <= tick_count; ++i)
    {
        grid_vertices.emplace_back(-tick_count, i);
        grid_vertices.emplace_back( tick_count, i);
        grid_vertices.emplace_back(i, -tick_count);
        grid_vertices.emplace_back(i,  tick_count);
    }
    gl::create_buffer(&m_grid_vertices, grid_vertices);
    m_grid_vertex_count = grid_vertices.size();

    gl::link_program(&m_grid_program, "shaders/grid_vx.glsl", "shaders/uniform4_ft.glsl");
    m_grid_matrix_uniform = gl::get_uniform_location(m_grid_program, "matrix");
    m_grid_color_uniform = gl::get_uniform_location(m_grid_program, "color");
    m_grid_position_attrib = gl::get_attrib_location(m_grid_program, "position");
}

void Camera2::close_grid()
{
    gl::delete_buffer(&m_grid_vertices);
    gl::delete_program(&m_grid_program);
}

void Camera2::render_grid()
{
    if (m_grid_program == 0)
    {
        throw std::runtime_error(
            "Camera2::draw_grid() must occur between open_grid() and close_grid()"
        );
    }

    if (m_size.x < 1 || m_size.y < 1)
        return;

    float ref_tick = (m_size.x + m_size.y) / (GRID_PARTITION * m_scale * m_size.y);
    float log_ref_tick = floor(log(GRID_PARTITION, ref_tick));
    float lo_tick = pow(GRID_PARTITION, log_ref_tick);
    float hi_tick = lo_tick * GRID_PARTITION;
    float lo_alpha = (hi_tick - ref_tick) / hi_tick;
    vec2f grid_translation(
        round(-m_translation.x / hi_tick) * hi_tick,
        round(-m_translation.y / hi_tick) * hi_tick);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glUseProgram(m_grid_program);
    glBindBuffer(GL_ARRAY_BUFFER, m_grid_vertices);
    glVertexAttribPointer(m_grid_position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_grid_position_attrib);

    mat3f grid_matrix = eye3f()
        .scale(lo_tick)
        .translate(grid_translation)
        .premul(matrix());
    glUniformMatrix3fv(m_grid_matrix_uniform, grid_matrix);
    glUniform4f(m_grid_color_uniform, 0, 0, 0, lo_alpha);
    glDrawArrays(GL_LINES, 0, m_grid_vertex_count);

    grid_matrix = eye3f()
        .scale(hi_tick)
        .translate(grid_translation)
        .premul(matrix());
    glUniformMatrix3fv(m_grid_matrix_uniform, grid_matrix);
    glUniform4f(m_grid_color_uniform, 0, 0, 0, 1);
    glDrawArrays(GL_LINES, 0, m_grid_vertex_count);

    glDisableVertexAttribArray(m_grid_position_attrib);
    glDisable(GL_BLEND);
}

void Camera2::ensure_projection_matrix_valid() const
{
    if (m_projection_matrix_valid)
        return;
    m_projection_matrix = eye3f().scale(float(m_size.y) / float(m_size.x), 1.0f);
    m_projection_matrix_valid = true;
}

void Camera2::ensure_inverse_projection_matrix_valid() const
{
    if (m_inverse_projection_matrix_valid)
        return;
    m_inverse_projection_matrix = eye3f().scale(float(m_size.x) / float(m_size.y), 1.0f);
    m_inverse_projection_matrix_valid = true;
}

void Camera2::ensure_view_matrix_valid() const
{
    if (m_view_matrix_valid)
        return;
    m_view_matrix = eye3f()
        .translate(m_translation)
        .rotate(m_rotation)
        .scale(m_scale, m_scale);
    m_view_matrix_valid = true;
}

void Camera2::ensure_inverse_view_matrix_valid() const
{
    if (m_inverse_view_matrix_valid)
        return;
    m_inverse_view_matrix = eye3f()
        .scale(1 / m_scale, 1 / m_scale)
        .rotate(-m_rotation)
        .translate(-m_translation);
    m_inverse_view_matrix_valid = true;
}

void Camera2::ensure_matrix_valid() const
{
    if (m_matrix_valid)
        return;
    m_matrix = projection_matrix() * view_matrix();
    m_matrix_valid = true;
}

void Camera2::ensure_inverse_matrix_valid() const
{
    if (m_inverse_matrix_valid)
        return;
    m_inverse_matrix = inverse_view_matrix() * inverse_projection_matrix();
    m_inverse_matrix_valid = true;
}

} // namespace kletch
