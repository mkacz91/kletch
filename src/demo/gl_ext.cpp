#include <SDL.h>
#include <SDL_opengl.h>

#ifdef __WIN32__

PFNGLBUFFERDATAPROC glBufferData;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

#endif // __WIN32__

namespace kletch {
namespace gl {

int init_ext() {
#ifdef __WIN32__

#define get_proc_address(proctype, procname) \
    procname = (proctype)SDL_GL_GetProcAddress(#procname); \
    if (procname == 0) \
        return 1;

    get_proc_address(PFNGLBUFFERDATAPROC, glBufferData);
    get_proc_address(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
    get_proc_address(PFNGLUNIFORM2FPROC, glUniform2f);
    get_proc_address(PFNGLUSEPROGRAMPROC, glUseProgram);
    get_proc_address(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    get_proc_address(PFNGLUNIFORM3FPROC, glUniform3f);
    get_proc_address(PFNGLBINDBUFFERPROC, glBindBuffer);
    get_proc_address(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    get_proc_address(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    get_proc_address(PFNGLUNIFORM4FPROC, glUniform4f);
    get_proc_address(PFNGLGENBUFFERSPROC, glGenBuffers);
    get_proc_address(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    get_proc_address(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    get_proc_address(PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv);
    get_proc_address(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv);
    get_proc_address(PFNGLCREATESHADERPROC, glCreateShader);
    get_proc_address(PFNGLSHADERSOURCEPROC, glShaderSource);
    get_proc_address(PFNGLCOMPILESHADERPROC, glCompileShader);
    get_proc_address(PFNGLGETSHADERIVPROC, glGetShaderiv);
    get_proc_address(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    get_proc_address(PFNGLDELETESHADERPROC, glDeleteShader);
    get_proc_address(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    get_proc_address(PFNGLATTACHSHADERPROC, glAttachShader);
    get_proc_address(PFNGLLINKPROGRAMPROC, glLinkProgram);
    get_proc_address(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    get_proc_address(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    get_proc_address(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    get_proc_address(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);

#endif // __WIN32__
return 0;
}

} // namespace gl
} // namespace kletch
