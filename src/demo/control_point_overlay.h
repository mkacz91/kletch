#ifndef KLETCH_DEMO_CONTROL_POINT_OVERLAY_H
#define KLETCH_DEMO_CONTROL_POINT_OVERLAY_H

#include "prefix.h"
#include <unordered_set>

#include "camera2.h"
#include "demo_event.h"

namespace kletch {

class ControlPointOverlay
{
public:
    const Camera2* camera() const { return m_camera; }
    void set_camera(const Camera2* camera) { m_camera = camera; }

    float point_radius() const { return m_point_radius; }
    void set_point_radius(float point_radius) { m_point_radius = point_radius; }

    int points_count() const { return m_points.size(); }
    vec2f* point(int i) { return m_points.at(i).get(); }
    vec2f* new_point();
    int point_index(const vec2f* point) const;
    void remove_point(int i) { m_points.erase(m_points.begin() + i); }
    void remove_point(const vec2f* point) { remove_point(point_index(point)); }
    void clear_points() { m_points.clear(); }

    void render();
    void open();
    void close();

    vec2f* pick_point(const vec2i& canvas_pos);
    vec2f* pick_point(int cx, int cy) { return pick_point(vec2i(cx, cy)); }

    void handle_event(const DemoEvent& e);

private:
    static const int CIRCLE_SEGMENT_COUNT;
    static const int POINT_VERTEX_COUNT;
    static const float TAU;

    std::vector<unique_ptr<vec2f>> m_points;
    std::unordered_set<vec2f*> m_selected_points;
    vec2f* m_highlighted_point = nullptr;

    const Camera2* m_camera = nullptr;
    float m_point_radius = 10;
    vec2f m_prev_mouse_world_pos;

    GLuint m_point_vertices = 0;
    GLuint m_point_program = 0;
    GLint m_point_transform_uniform = -1;
    GLint m_point_color_uniform = -1;
    GLint m_point_position_attrib = -1;

    void highlight_points(const vec2i& canvas_pos);
    void highlight_points(int cx, int cy) { return highlight_points(vec2i(cx, cy)); }
};

} // namespace kletch

#endif // KLETCH_DEMO_CONTROL_POINT_OVERLAY_H
