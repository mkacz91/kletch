#ifndef KLETCH_DEMO_AIMER_DEMO_H
#define KLETCH_DEMO_AIMER_DEMO_H

#include "prefix.h"

#include <lib/clothoid_aimer.h>

#include "constrained_clothoid_demo.h"

namespace kletch {

class AimerDemo : public ConstrainedClothoidDemo
{
public:
    AimerDemo();

    virtual void render() override;
    virtual void handle_event(const DemoEvent& e) override;

protected:
    virtual void open() override;
    virtual void close() noexcept override;

private:
    static const int CLOTHOID_VERTEX_COUNT = 40;

    ClothoidAimer m_aimer;
    ClothoidAimer::Result m_aim_result;
    vec2f m_aim_eval = { 0.0f, 0.5f };

    bool m_cloth_ready = false;
    GLuint m_cloth_vertices = 0;
    GLuint m_cloth_program = 0;
    GLint m_cloth_matrix_uniform;
    GLint m_cloth_color_uniform;
    GLint m_cloth_position_attrib;

    GLuint m_aimer_program = 0;
    GLint m_aimer_matrix_uniform = -1;
    GLint m_aimer_color_uniform = -1;
    GLint m_aimer_position_attrib = -1;

    GLuint m_grid_vertices = 0;
    GLuint m_sample_vertices = 0;
};

} // namespace kletch

#endif // KLETCH_DEMO_AIMER_DEMO_H
