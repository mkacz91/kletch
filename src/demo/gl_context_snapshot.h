#ifndef KLETCH_DEMO_GL_CONTEXT_GUARD
#define KLETCH_DEMO_GL_CONTEXT_GUARD

#include "prefix.h"

namespace kletch {
namespace gl {

class ContextGuard
{
public:
    ContextGuard();
    ~ContextGuard();

private:
    static const int MAX_VERTEX_ATTRIBS = 8;

    struct VertexAttrib
    {
        GLint binding;
        GLint enabled;
        GLint size;
        GLint stride;
        GLint type;
        GLint normalized;
        GLvoid* pointer;
    };

    GLint m_current_program;
    GLint m_array_buffer_binding;
    GLint m_element_array_buffer_binding;
    VertexAttrib m_vertex_attribs[MAX_VERTEX_ATTRIBS + 1];
};

} // namespace gl
} // namespace kletch

#endif // KLETCH_DEMO_GL_CONTEXT_GUARD
