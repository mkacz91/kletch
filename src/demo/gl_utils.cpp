#include "gl_utils.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace kletch {
namespace gl {

std::unordered_map<GLuint, string> shader_filenames;

GLuint load_shader(const string& filename, GLenum shader_type)
{
    // Open file
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw exception("Unable to open file: " + filename);

    // Create shader object
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0)
        throw exception("Unable to create new shader");

    // Upload source
    std::ostringstream sout;
    sout << fin.rdbuf() << endl;
    string source = sout.str();
    const char* c_source = source.c_str();
    int source_length = source.length();
    glShaderSource(shader, 1, &c_source, &source_length);

    // Compile
    glGetError();
    glCompileShader(shader);
    if (glGetError() != GL_NO_ERROR)
    {
        const int max_log_length = 255;
        char log[max_log_length + 1];
        glGetShaderInfoLog(shader, max_log_length, nullptr, (char*)&log);
        glDeleteShader(shader);
        throw exception("Shader compilation error, file '" + filename + "': " + log);
    }

    shader_filenames[shader] = filename;
    return shader;
}

GLuint load_vertex_shader(const string& filename)
{
    return load_shader(filename, GL_VERTEX_SHADER);
}

GLuint load_fragment_shader(const string& filename)
{
    return load_shader(filename, GL_FRAGMENT_SHADER);
}

string shader_filename(GLuint shader)
{
    auto match = shader_filenames.find(shader);
    if (match != shader_filenames.end())
        return match->second;
    return to_string(shader);
}

GLuint link_program(const string& vertex_shader_filename, const string& fragment_shader_filename)
{
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    try
    {
        vertex_shader = load_vertex_shader(vertex_shader_filename);
        fragment_shader = load_fragment_shader(fragment_shader_filename);
        GLuint program = link_program(vertex_shader, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
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
            "Unable to attach shaders: " + error_string(error) +
            ", shaders: " + shader_filename(vertex_shader) + ", " +
            shader_filename(fragment_shader)
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
            "Unable to link program, shaders: " + shader_filename(vertex_shader) + ", " +
            shader_filename(fragment_shader) + ", log: " + log
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
