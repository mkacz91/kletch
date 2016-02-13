#ifndef KLETCH_DEMO_GL_UTILS
#define KLETCH_DEMO_GL_UTILS

#include "prefix.h"
#include <exception>
#include <vector>

#include "int_range.h"

namespace kletch {

template <typename T>
int byte_size(const std::vector<T>& xs) { return xs.size() * sizeof(T); }

template <typename T>
void glBufferData(GLenum target, const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW)
{
    ::glBufferData(target, byte_size(data), data.data(), usage);
}

inline void glDrawArrays(GLenum target, GLint first, GLint count)
{
    ::glDrawArrays(target, first, count);
}

inline void glDrawArrays(GLenum target, const int_range& vertex_range)
{
    ::glDrawArrays(target, vertex_range.first, vertex_range.count);
}

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
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = GL_NO_ERROR)

#define gl_error_guard(call) \
    gl_if_error (call) \
        throw ::kletch::gl::exception(::kletch::gl::error_string(error) + " during " + #call);

template <typename T>
GLuint create_buffer(GLenum target, const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, data, usage);
    return buffer;
}

GLuint load_vertex_shader(const string& resname);

GLuint load_fragment_shader(const string& resname);

string shader_name(GLuint shader);

GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

GLuint link_program(const string& vertex_shader_resname, const string& fragment_shader_resname);

GLint get_uniform_location(GLuint program, const char* name);

GLint get_attrib_location(GLuint program, const char* name);

string error_string(GLenum);

} // namespace gl
} // namespace kletch

#endif // KLETCH_DEMO_GL_UTILS
