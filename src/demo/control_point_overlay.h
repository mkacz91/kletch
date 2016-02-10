#ifndef KLETCH_DEMO_CONTROL_POINT_OVERLAY_H
#define KLETCH_DEMO_CONTROL_POINT_OVERLAY_H

#include "prefix.h"

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
    vec2f* point(int i) { return m_points[i].position.get(); }
    vec2f* new_point();
    int point_index(const vec2f* point) const;
    void remove_point(int i) { m_points.erase(m_points.begin() + i); }
    void remove_point(const vec2f* point) { remove_point(point_index(point)); }
    void clear_points() { m_points.clear(); }

    void render();
    void open();
    void close();

    void handle_event(const DemoEvent& e);

private:
    static const int CIRCLE_SEGMENT_COUNT;
    static const int POINT_VERTEX_COUNT;
    static const float TAU;

    enum ControlPointState
    {
        CONTROL_POINT_IDLE,
        CONTROL_POINT_HIGHLIGHTED,
        CONTROL_POINT_SELECTED
    };

    struct ControlPoint
    {
        unique_ptr<vec2f> position;
        ControlPointState state = CONTROL_POINT_IDLE;

        ControlPoint(vec2f* position) : position(make_unique(position)) { }
    };

    std::vector<ControlPoint> m_points;

    const Camera2* m_camera = nullptr;
    float m_point_radius = 7;

    GLuint m_point_vertices = 0;
    GLuint m_point_program = 0;
    GLint m_point_transform_uniform = -1;
    GLint m_point_color_uniform = -1;
    GLint m_point_position_attrib = -1;
};

} // namespace kletch

#endif // KLETCH_DEMO_CONTROL_POINT_OVERLAY_H
