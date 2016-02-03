#include "hello_demo.h"

#include <vector>

namespace kletch {

using std::vector;

void HelloDemo::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //glUseProgram(m_program);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    //glEnableVertexAttribArray(m_position_attrib);
    glVertexAttribPointer(m_position_attrib, 2, GL_FLOAT, false, 0, 0);

    //glDrawArrays(GL_TRIANGLES, 0, 3);

    //glDisableVertexAttribArray(m_position_attrib);
}

void HelloDemo::handle_event(const DemoEvent& e)
{
    if (e.type() == SDL_VIDEORESIZE)
    {
        close();
        open();
    }
}

void HelloDemo::open()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    m_program = gl::link_program("shaders/hello_vx.glsl", "shaders/hello_ft.glsl");
    glUseProgram(m_program);
    m_position_attrib = gl::get_attrib_location(m_program, "position");

    glGenBuffers(1, &m_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    vector<float> vertices {
        -1, -1,
         0,  1,
         1, -1
    };
    glBufferData(GL_ARRAY_BUFFER, byte_size(vertices), vertices.data(), GL_STATIC_DRAW);

}

void HelloDemo::close() noexcept
{
    glDeleteBuffers(1, &m_vertices);
    m_vertices = 0;

    glDisableVertexAttribArray(m_position_attrib);
    m_position_attrib = -1;

    glDeleteProgram(m_program);
    m_program = 0;
}

} // anamespace keltch
