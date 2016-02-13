#ifndef KLETCJ_DEMO_CAMERA2_H
#define KLETCJ_DEMO_CAMERA2_H

#include "prefix.h"

#include "demo_event.h"

namespace kletch {

class Camera2
{
public:
    vec2f size = 1;
    vec2f translation = 0;
    vec2f scale = 100.0f;

    void set_uniform(int location);

    vec2f to_world(const vec2f& canvas_pos) const;
    vec2f to_world(float cx, float cy) const { return to_world(vec2f(cx, cy)); }

    vec2f to_canvas(const vec2f& world_pos) const;
    vec2f to_canvas(float wx, float wy) const { return to_canvas(vec2f(wx, wy)); }

    vec2f to_canvas_vector(const vec2f& world_vector) const;
    vec2f to_canvas_vector(float wvx, float wvy) { return to_canvas_vector(vec2f(wvx, wvy)); }

    void handle_event(const DemoEvent& e);

    void open_grid();
    void close_grid();
    void render_grid();

private:
    bool m_dragging = false;
    vec2i m_grab_position = 0;
    vec2f m_translation_at_grab = 0;

    GLuint m_grid_vertices = 0;
    GLuint m_grid_program = 0;
    GLint m_grid_transform_uniform = -1;
    GLint m_grid_color_uniform = -1;
    GLint m_grid_position_attrib = -1;
};

} // namespace kletch

#endif // KLETCJ_DEMO_CAMERA2_H
