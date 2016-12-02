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
    Camera2 m_camera;
    std::vector<vec2f> m_samples;

    GLuint m_stroke_program = 0;
    GLint m_stroke_matrix_uniform;
    GLint m_stroke_color_uniform;
    GLuint m_stroke_position_attrib;
    GLuint m_stroke_vertices = 0;

    bool m_mouse_down;
    int m_buffered_sample_count;

    void clear();
    void put_sample(vec2f position);
    void update_buffers();
};

} // namespace kletch

#endif // KLETCH_DEMO_KLETCH_DEMO_H
