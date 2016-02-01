#ifndef KLETCH_DEMO_HELLO_DEMO_H
#define KLETCH_DEMO_HELLO_DEMO_H

#include "prefix.h"

#include "demo.h"

namespace kletch {

class HelloDemo : public Demo
{
public:
    HelloDemo() : Demo("Hello") { }

    virtual void render() override;

    virtual void handle_event(const DemoEvent& e) override;

protected:
    virtual void init() override;

    virtual void close() noexcept override;

private:
    GLuint m_program;
    GLint m_position_attrib;
    GLuint m_vertices;
};

} // namespace kletch

#endif // KLETCH_DEMO_HELLO_DEMO_H
