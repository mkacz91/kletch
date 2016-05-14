#include "hello_demo.h"

#include <vector>

namespace kletch {

using std::vector;

void HelloDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_camera.render_grid();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void HelloDemo::handle_event(const DemoEvent& e)
{
    m_camera.handle_event(e);
}

void HelloDemo::gl_open()
{
    glClearColor(0.5f, 0.9f, 0.5f, 1.0f);

    gl::link_program(&m_program, "shaders/hello_vx.glsl", "shaders/hello_ft.glsl");
    glUseProgram(m_program);
    m_position_attrib = gl::get_attrib_location(m_program, "position");

    vector<float> vertices {
        -1, -1,
         0,  1,
         1, -1
    };
    gl::create_buffer(&m_vertices, vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glVertexAttribPointer(m_position_attrib, 2, GL_FLOAT, false, 0, 0);

    m_camera.open_grid();
    m_camera.set_size(width(), height());
}

void HelloDemo::gl_close()
{
    m_camera.close_grid();
    gl::delete_buffer(&m_vertices);
    gl::delete_program(&m_program);
}

} // anamespace keltch
