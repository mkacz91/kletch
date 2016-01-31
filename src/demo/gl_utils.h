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

GLuint load_vertex_shader(const Resource& r);

GLuint load_fragment_shader(const Resource& r);

string shader_name(GLuint shader);

GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

GLuint link_program(const Resource& vertex_shader_r, const Resource& fragment_shader_r);

string error_string(GLenum);

} // namespace gl
} // namespace kletch

#endif // KLETCH_DEMO_GL_UTILS
