#ifndef KLETCH_DEMO_AIMER_DEMO_H
#define KLETCH_DEMO_AIMER_DEMO_H

#include "prefix.h"
#include "demo.h"

namespace kletch {

class AimerDemo : public Demo
{
public:
    AimerDemo() : Demo("Clothoid Aim") { }

    virtual void render() override;
    virtual void handle_event(const DemoEvent& e) override;

protected:
    virtual void open() override;
    virtual void close() noexcept override;

private:
    static const int CIRCLE_SEGMENT_COUNT;
    static const int POINT_VERTEX_COUNT;
    static const float POINT_RADIUS;
    static const float TAU;

    GLuint m_point_vertices;
    GLuint m_point_program;
    GLint m_point_transform_uniform;
    GLint m_point_position_attrib;

    vec2f point = vec2f::ZERO;
};

} // namespace kletch

#endif // KLETCH_DEMO_AIMER_DEMO_H
