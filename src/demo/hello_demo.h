#ifndef KLETCH_DEMO_HELLO_DEMO_H
#define KLETCH_DEMO_HELLO_DEMO_H

#include "prefix.h"

#include "demo.h"
#include "camera2.h"

namespace kletch {

class HelloDemo : public Demo
{
public:
    HelloDemo() : Demo("Hello") { }

    virtual bool on_event(Event const& e) override;

protected:
    virtual void on_render() override;
    virtual void on_open() override;
    virtual void on_close() override;

private:
    Camera2 m_camera;

    GLuint m_program = 0;
    GLint m_position_attrib = -1;
    GLuint m_vertices = 0;
};

} // namespace kletch

#endif // KLETCH_DEMO_HELLO_DEMO_H
