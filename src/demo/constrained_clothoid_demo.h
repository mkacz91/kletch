#ifndef KLETCH_DEMO_CONSTRAINED_CLOTHOID_DEMO_H
#define KLETCH_DEMO_CONSTRAINED_CLOTHOID_DEMO_H

#include "prefix.h"

#include "camera2.h"
#include "control_overlay.h"
#include "demo.h"

namespace kletch {

class ConstrainedClothoidDemo : public Demo
{
public:
    ConstrainedClothoidDemo(const string& display_name);

    const vec2f origin() const { return m_origin; }
    float arc_radius() const { return m_arc_radius; }
    float tangent_angle() const { return m_tangent_angle; }

    virtual bool on_event(Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;

private:
    static const int ARC_VERTEX_COUNT = 20;

protected: // TODO: This is temporary
    Camera2 m_camera;
    ControlOverlay m_control_overlay;

private:
    vec2f m_origin = 0;
    vec2f m_tangent_tip = vec2f(1, 0);
    vec2f m_arc_end = vec2f(-1, 1);
    vec2f m_local_arc_end;

    float m_arc_radius;
    float m_local_arc_angle0;
    float m_local_arc_angle1;
    vec2f m_local_arc_center;
    float m_tangent_angle = 0;

    GLuint m_arc_vertices = 0;
    GLuint m_arc_program = 0;
    GLint m_arc_matrix_uniform;
    GLint m_arc_center_r_uniform;
    GLint m_arc_angle_uniform;
    GLint m_arc_color0_uniform;
    GLint m_arc_color1_uniform;
    GLint m_arc_param_attrib;

    void update_local_arc();
    void update_arc();

    vec2f to_tangent_coords(vec2f const& v) const;
    vec2f from_tangent_coords(vec2f const& v) const;
};

} // namespace kletch

#endif // KLETCH_DEMO_CONSTRAINED_CLOTHOID_DEMO_H
