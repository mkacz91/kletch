#ifndef KLETCH_DEMO_FITTER_DEMO_H
#define KLETCH_DEMO_FITTER_DEMO_H

#include "prefix.h"
#include <lib/clothoid_fitter.h>

#include "control_overlay.h"

namespace kletch {

class FitterDemo : public ControlOverlay
{
public:
    FitterDemo();
    ~FitterDemo();

    virtual bool on_event(Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;

private:
    static const int CLOTHOID_VERTEX_COUNT = 40;

    Camera2 m_camera;
    ClothoidFitter m_fitter;
    std::vector<vec2f*> m_samples;
    CurveWindow m_curve;

    GLuint m_cloth_vertices = 0;
    GLuint m_cloth_program = 0;
    GLint m_cloth_matrix_uniform;
    GLint m_cloth_color_uniform;
    GLint m_cloth_position_attrib;

    void fit();
};

} // namespace kletch

#endif // KLETCH_DEMO_FITTER_DEMO_H
