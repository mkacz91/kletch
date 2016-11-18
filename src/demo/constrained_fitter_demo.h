#ifndef KLETCH_DEMO_CONSTRAINED_FITTER_DEMO_H
#define KLETCH_DEMO_CONSTRAINED_FITTER_DEMO_H

#include "prefix.h"

#include <lib/clothoid_aimer.h>
#include <lib/constrained_clothoid_fitter.h>

#include "constrained_clothoid_demo.h"

namespace kletch {

class ConstrainedFitterDemo : public ConstrainedClothoidDemo
{
public:
    ConstrainedFitterDemo();
    ~ConstrainedFitterDemo();

    virtual bool on_event(Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;
    virtual void on_point_move(vec2f* point, vec2f prev_pos) override;

private:
    static const int CLOTHOID_VERTEX_COUNT = 40;

    ClothoidAimer m_aimer;
    ConstrainedClothoidFitter m_fitter;
    std::vector<vec2f*> m_targets;
    std::vector<ClothoidAimer::Result> m_cloths; // TODO(mkc): Use more appropriate type

    GLuint m_cloth_vertices = 0;
    GLuint m_poly_vertices = 0;
    GLuint m_cloth_program = 0;
    GLint m_cloth_matrix_uniform ;
    GLint m_cloth_color_uniform;
    GLint m_cloth_position_attrib;

    void fit();
};

} // namespace kletch

#endif // KLETCH_DEMO_CONSTRAINED_FITTER_DEMO_H
