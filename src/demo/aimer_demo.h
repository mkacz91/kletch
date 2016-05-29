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

    virtual bool on_event(SDL_Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;

private:
    static const int CLOTHOID_VERTEX_COUNT = 40;

    ClothoidAimer m_aimer;
    ClothoidAimer::Result m_aim_result;
    vec2f m_aim_eval = { 0.0f, 0.5f };
    float m_a = 0;
    float m_s = 0;

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

    static void TW_CALL set_refine_steps_cb(const void* value, void* client_data);
    static void TW_CALL get_refine_steps_cb(void* value, void* client_data);

    void aim();
};

} // namespace kletch

#endif // KLETCH_DEMO_AIMER_DEMO_H
