#include "gl_utils.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace kletch {
namespace gl {

std::unordered_map<GLuint, string> shader_names;

GLuint load_shader(const string& resname, GLenum shader_type)
{
    const Resource& r = get_resource(resname);

    // Create shader object
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0)
        throw exception("Unable to create new shader");

    // Upload source and compile
    glShaderSource(shader, 1, &r.value, &r.length);
    glCompileShader(shader);

    // TODO: Do this only in DEBUG

    int compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        const int max_log_length = 255;
        char log[max_log_length + 1];
        glGetShaderInfoLog(shader, max_log_length, nullptr, (char*)&log);
        glDeleteShader(shader);
        throw exception("Shader " + squote(r.name) + ", compilation error: " + log);
    }

    shader_names[shader] = r.name;
    return shader;
}

GLuint load_vertex_shader(const string& resname)
{
    return load_shader(resname, GL_VERTEX_SHADER);
}

GLuint load_fragment_shader(const string& resname)
{
    return load_shader(resname, GL_FRAGMENT_SHADER);
}

string shader_name(GLuint shader)
{
    auto match = shader_names.find(shader);
    if (match != shader_names.end())
        return match->second;
    return to_string(shader);
}

GLuint link_program(const string& vertex_shader_resname, const string& fragment_shader_resname)
{
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    try
    {
        vertex_shader = load_vertex_shader(vertex_shader_resname);
        fragment_shader = load_fragment_shader(fragment_shader_resname);
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
    // Create program
    GLuint program = glCreateProgram();
    if (program == 0)
        throw exception("Unable to create new program");

    // Attach shaders
    gl_if_error (
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
    ) {
        glDeleteProgram(program);
        throw exception(
            "Unable to attach shaders " + squote(shader_name(vertex_shader)) + ", " +
            squote(shader_name(fragment_shader)) + ": "+ error_string(error)
        );
    }

    // Link program
    glLinkProgram(program);
    int link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        const int max_log_length = 255;
        char log[max_log_length + 1];
        glGetProgramInfoLog(program, max_log_length, nullptr, (char*)&log);
        glDeleteProgram(program);
        throw exception(
            "Unable to link shaders " + squote(shader_name(vertex_shader)) + ", " +
            squote(shader_name(fragment_shader)) + ": " + log
        );
    }

    return program;
}

GLint get_uniform_location(GLuint program, const char* name)
{
    gl_if_error (GLint location = glGetUniformLocation(program, name))
    {
        throw exception(
            error_string(error) + "during glGetUniformLocation(" +
            to_string(program) + ", " + quote(name) + ")"
        );
    }
    if (location < 0)
    {
        throw exception(
            "glGetUniformLocation(" + to_string(program) + ", " + quote(name) +
            ") returned " + to_string(location)
        );
    }
    return location;
}

GLint get_attrib_location(GLuint program, const char* name)
{
    gl_if_error (GLint location = glGetAttribLocation(program, name))
    {
        throw exception(
            error_string(error) + " during glGetAttribLocation(" +
            to_string(program) + ", " + quote(name) + ")"
        );
    }
    if (location < 0)
    {
        throw exception(
            "glGetAttribLocation(" + to_string(program) + ", " + quote(name) +
            ") returned " + to_string(location)
        );
    }
    return location;
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
