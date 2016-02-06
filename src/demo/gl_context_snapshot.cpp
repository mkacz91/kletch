#include "gl_context_snapshot.h"

namespace kletch {
namespace gl {

void ContextSnapshot::capture()
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &m_current_program);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_array_buffer_binding);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_element_array_buffer_binding);

    for (int i = 1; i <= MAX_VERTEX_ATTRIBS; ++i)
    {
        VertexAttrib& va = m_vertex_attribs[i];
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &va.binding);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &va.enabled);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &va.size);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &va.stride);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &va.type);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &va.normalized);
        glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &va.pointer);
    }
}

void ContextSnapshot::restore() const
{
    for (int i = MAX_VERTEX_ATTRIBS; i >= 1; --i)
    {
        const VertexAttrib& va = m_vertex_attribs[i];
        glBindBuffer(GL_ARRAY_BUFFER, va.binding);
        glVertexAttribPointer(
            i,
            va.size,
            va.type,
            va.normalized ? GL_TRUE : GL_FALSE,
            va.stride,
            va.pointer
        );
        if (va.enabled)
            glEnableVertexAttribArray(i);
        else
            glDisableVertexAttribArray(i);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_array_buffer_binding);
    glBindBuffer(GL_ARRAY_BUFFER, m_array_buffer_binding);
    glUseProgram(m_current_program);
}

} // namespace gl
} // namespace kletch
