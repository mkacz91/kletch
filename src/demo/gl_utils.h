#ifndef KLETCH_DEMO_GL_UTILS
#define KLETCH_DEMO_GL_UTILS

#include "prefix.h"
#include <exception>
#include <vector>

namespace kletch {

template <typename T>
int byte_size(const std::vector<T>& xs) { return xs.size() * sizeof(T); }

namespace gl {

class exception : public std::exception
{
public:
    exception(const string& what) : m_what(what) { }
    virtual const char* what() const noexcept override { return m_what.c_str(); }
private:
    string m_what;
};

#define gl_if_error(call) \
    glGetError(); \
    call; \
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = GL_NO_ERROR) \

#define gl_error_guard(call) \
    gl_if_error (call) \
        throw ::kletch::gl::exception(::kletch::gl::error_string(error) + " during " + #call);

GLuint load_vertex_shader(const string& resname);

GLuint load_fragment_shader(const string& resname);

string shader_name(GLuint shader);

GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

GLuint link_program(const string& vertex_shader_resname, const string& fragment_shader_resname);

GLint get_attrib_location(GLuint program, const char* name);

string error_string(GLenum);

} // namespace gl
} // namespace kletch

#endif // KLETCH_DEMO_GL_UTILS
