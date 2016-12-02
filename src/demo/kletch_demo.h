#ifndef KLETCH_DEMO_KLETCH_DEMO_H
#define KLETCH_DEMO_KLETCH_DEMO_H

#include "prefix.h"
#include <vector>
#include <lib/kletch.h>

#include "camera2.h"
#include "demo.h"


namespace kletch {

class KletchDemo : public Demo
{
public:
    KletchDemo();

    virtual bool on_event(Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;

private:
    struct SplineVertex
    {
        vec2f position;
        float parity;
    };

    static constexpr int SEGMENT_VERTEX_COUNT = 20;

    Camera2 m_camera;
    Kletch m_kletch;
    std::vector<vec2f> m_samples;

    GLuint m_sample_program = 0;
    GLint m_sample_matrix_uniform;
    GLint m_sample_color_uniform;
    GLuint m_sample_position_attrib;
    GLuint m_sample_vertices = 0;

    GLuint m_spline_program = 0;
    GLint m_spline_matrix_uniform;
    GLint m_spline_even_color_uniform;
    GLint m_spline_odd_color_uniform;
    GLint m_spline_position_parity_attrib;
    GLuint m_spline_vertices = 0;
    GLuint m_tangent_vertices = 0;
    std::vector<vec2f> m_tangent_vertices_local;
    std::vector<SplineVertex> m_spline_vertices_local; // TODO(mkc): Memory mapping, or sth?

    bool m_mouse_down;
    vec2f m_world_mouse_pos;
    int m_segment_count;
    int m_pushed_sample_count;
    int m_sample_vertex_count;
    int m_spline_vertex_count;

    void clear();
    void keep_up();
    void put_sample(vec2f position);
    void update_buffers();

    bool m_advance = false;
};

} // namespace kletch

#endif // KLETCH_DEMO_KLETCH_DEMO_H
