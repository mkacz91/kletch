#ifndef KLETCH_DEMO_GL_UTILS
#define KLETCH_DEMO_GL_UTILS

#include "prefix.h"
#include <exception>

namespace kletch {
namespace gl {

public class exception : public std::exception
{
public:
    exception(const string& what) : m_what(what) { }
    virtual const char* what() override { return m_what.c_str(); }
private:
    string m_what;
}

GLuint load_vertex_shader(const string& filename);

GLuint load_fragment_shader(const string& filename);

string shader_filename(GLuint shader);

GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

GLuint link_program(const string& vertex_shader_filename, const string& fragment_shader_filename);

string error_string(GLenum);

} // namespace gl
} // namespace kletch

#endif // KLETCH_DEMO_GL_UTILS
