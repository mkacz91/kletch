#ifndef KLETCJ_DEMO_CAMERA2_H
#define KLETCJ_DEMO_CAMERA2_H

#include "prefix.h"

#include "demo_event.h"

namespace kletch {

class Camera2
{
public:
    const mat3f& projection_matrix() const;
    const mat3f& inverse_projection_matrix() const;
    const mat3f& view_matrix() const;
    const mat3f& inverse_view_matrix() const;
    const mat3f& matrix() const;
    const mat3f& inverse_matrix() const;

    vec2i const& size() const { return m_size; }
    void set_size(const vec2i& size);
    void set_size(int w, int h) { set_size(vec2i(w, h)); }

    float scale() const { return m_scale; }
    void set_scale(float scale);
    void scale(float factor);

    float rotation() const { return m_rotation; }
    void set_rotation(float rotation);
    void rotate(float rotation);

    const vec2f& translation() const { return m_translation; }
    void set_translation(const vec2f& translation);
    void set_translation(float tx, float ty) { set_translation(vec2f(tx, ty)); }
    void translate(const vec2f& translation);
    void translate(float tx, float ty) { translate(vec2f(tx, ty)); }

    vec2f canvas_to_ndc(int x, int y) const;
    vec2f canvas_to_ndc(const vec2i& p) const { return canvas_to_ndc(p.x, p.y); }

    vec2f canvas_to_ndc_vec(int x, int y) const;
    vec2f canvas_to_ndc_vec(const vec2i& v) const { return canvas_to_ndc_vec(v.x, v.y); }

    vec2f canvas_to_world(int x, int y) const;
    vec2f canvas_to_world(const vec2i& p) const { return canvas_to_world(p.x, p.y); }

    vec2f canvas_to_world_vec(int x, int y) const;
    vec2f canvas_to_world_vec(const vec2i& v) const { return canvas_to_world_vec(v.x, v.y); }

    void handle_event(const DemoEvent& e);
    void open_grid();
    void close_grid();
    void render_grid();

private:
    mutable bool m_projection_matrix_valid = false;
    mutable bool m_inverse_projection_matrix_valid = false;
    mutable mat3f m_projection_matrix;
    mutable mat3f m_inverse_projection_matrix;
    mutable bool m_view_matrix_valid = false;
    mutable bool m_inverse_view_matrix_valid = false;
    mutable mat3f m_view_matrix;
    mutable mat3f m_inverse_view_matrix;
    mutable bool m_matrix_valid = false;
    mutable bool m_inverse_matrix_valid = false;
    mutable mat3f m_matrix;
    mutable mat3f m_inverse_matrix;

    vec2i m_size = vec2i(2, 2);
    vec2f m_translation;
    float m_scale = 1;
    float m_rotation = 0;

    bool m_dragging = false;
    vec2i m_grab_position = 0;
    vec2f m_translation_at_grab = 0;

    GLuint m_grid_vertices = 0;
    GLuint m_grid_program = 0;
    GLint m_grid_matrix_uniform = -1;
    GLint m_grid_color_uniform = -1;
    GLint m_grid_position_attrib = -1;

    void invalidate_projection_matrix();
    void invalidate_view_matrix();
    void ensure_projection_matrix_valid() const;
    void ensure_inverse_projection_matrix_valid() const;
    void ensure_view_matrix_valid() const;
    void ensure_inverse_view_matrix_valid() const;
    void ensure_matrix_valid() const;
    void ensure_inverse_matrix_valid() const;
};

inline const mat3f& Camera2::projection_matrix() const
{
    ensure_projection_matrix_valid();
    return m_projection_matrix;
}

inline const mat3f& Camera2::inverse_projection_matrix() const
{
    ensure_inverse_projection_matrix_valid();
    return m_inverse_projection_matrix;
}

inline const mat3f& Camera2::view_matrix() const
{
    ensure_view_matrix_valid();
    return m_view_matrix;
}

inline const mat3f& Camera2::inverse_view_matrix() const
{
    ensure_inverse_view_matrix_valid();
    return m_inverse_view_matrix;
}

inline const mat3f& Camera2::matrix() const
{
    ensure_matrix_valid();
    return m_matrix;
}

inline const mat3f& Camera2::inverse_matrix() const
{
    ensure_inverse_matrix_valid();
    return m_inverse_matrix;
}

inline void Camera2::set_size(const vec2i& size)
{
    m_size = size;
    invalidate_projection_matrix();
}

inline void Camera2::set_scale(float scale)
{
    m_scale = scale;
    invalidate_view_matrix();
}

inline void Camera2::scale(float scale)
{
    m_scale *= scale;
    invalidate_view_matrix();
}

inline void Camera2::set_rotation(float rotation)
{
    m_rotation = rotation;
    invalidate_view_matrix();
}

inline void Camera2::rotate(float rotation)
{
    m_rotation += rotation;
    invalidate_view_matrix();
}

inline void Camera2::set_translation(const vec2f& translation)
{
    m_translation = translation;
    invalidate_view_matrix();
}

inline void Camera2::translate(const vec2f& translation)
{
    m_translation += translation;
    invalidate_view_matrix();
}

inline vec2f Camera2::canvas_to_ndc(int x, int y) const
{
    return vec2f(
        2.0f * x / m_size.x - 1.0f,
        -2.0f * y / m_size.y - 1.0f
    );
}

inline vec2f Camera2::canvas_to_ndc_vec(int x, int y) const
{
    return vec2f(2.0f * x / m_size.x, -2.0f * y / m_size.y);
}

inline void Camera2::invalidate_projection_matrix()
{
    m_projection_matrix_valid = false;
    m_inverse_projection_matrix_valid = false;
    m_matrix_valid = false;
    m_inverse_matrix_valid = false;
}

inline void Camera2::invalidate_view_matrix()
{
    m_view_matrix_valid = false;
    m_inverse_view_matrix_valid = false;
    m_matrix_valid = false;
    m_inverse_matrix_valid = false;
}

} // namespace kletch

#endif // KLETCJ_DEMO_CAMERA2_H
