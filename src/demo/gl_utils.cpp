#include "gl_utils.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace kletch {
namespace gl {

std::unordered_map<GLuint, string> shader_names;

GLuint load_shader(const Resource& r, GLenum shader_type)
{
    // Create shader object
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0)
        throw exception("Unable to create new shader");

    // Upload source and compile
    glShaderSource(shader, 1, &r.value, &r.length);
    glGetError();
    glCompileShader(shader);
    if (glGetError() != GL_NO_ERROR)
    {
        const int max_log_length = 255;
        char log[max_log_length + 1];
        glGetShaderInfoLog(shader, max_log_length, nullptr, (char*)&log);
        glDeleteShader(shader);
        throw exception("Shader '" + r.name + "', compilation error: " + log);
    }

    shader_names[shader] = r.name;
    return shader;
}

GLuint load_vertex_shader(const Resource& r)
{
    return load_shader(r, GL_VERTEX_SHADER);
}

GLuint load_fragment_shader(const Resource& r)
{
    return load_shader(r, GL_FRAGMENT_SHADER);
}

string shader_name(GLuint shader)
{
    auto match = shader_names.find(shader);
    if (match != shader_names.end())
        return match->second;
    return to_string(shader);
}

GLuint link_program(const Resource& vertex_shader_r, const Resource& fragment_shader_r)
{
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    try
    {
        vertex_shader = load_vertex_shader(vertex_shader_r);
        fragment_shader = load_fragment_shader(fragment_shader_r);
        GLuint program = link_program(vertex_shader, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return program;
    }
    catch (...)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw;
    }
}

GLuint link_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint program = glCreateProgram();
    if (program == 0)
        throw exception("Unable to create new program");

    GLenum error = glGetError();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        glDeleteProgram(program);
        throw exception(
            "Unable to attach shaders '" + shader_name(vertex_shader) + "', '" +
            shader_name(fragment_shader) + "': "+ error_string(error)
        );
    }

    glLinkProgram(program);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        const int max_log_length = 255;
        char log[max_log_length + 1];
        glGetProgramInfoLog(program, max_log_length, nullptr, (char*)&log);
        glDeleteProgram(program);
        throw exception(
            "Unable to link shaders '" + shader_name(vertex_shader) + "', '" +
            shader_name(fragment_shader) + "': " + log
        );
    }

    return program;
}

string error_string(GLenum error)
{
#   define error_string_case(constant_name) case constant_name: return #constant_name
    switch (error)
    {
        error_string_case(GL_NO_ERROR);
        error_string_case(GL_INVALID_ENUM);
        error_string_case(GL_INVALID_VALUE);
        error_string_case(GL_INVALID_OPERATION);
        error_string_case(GL_STACK_OVERFLOW);
        error_string_case(GL_STACK_UNDERFLOW);
        error_string_case(GL_OUT_OF_MEMORY);
        error_string_case(GL_TABLE_TOO_LARGE);
        default: return "KLETCH_GL_UNKNOWN";
    }
}

} // namespace gl
} // namespace kletch
