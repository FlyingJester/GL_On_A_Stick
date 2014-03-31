#include "glExtra.h"

#include <stdio.h>

void (APIENTRY * glGenBuffers)(GLsizei, GLuint*) = NULL;
void (APIENTRY * glDeleteBuffers)(GLsizei, GLuint*) = NULL;
void (APIENTRY * glBindBuffer)(GLenum, GLuint) = NULL;
void (APIENTRY * glBufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum) = NULL;
void (APIENTRY * glBufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid *) = NULL;
void (APIENTRY * glCopyImageSubData)(GLuint, GLenum, GLint, GLint, GLint, GLint, GLuint, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei) = NULL;
GLenum (APIENTRY * glCreateShader)(GLenum) = NULL;
void (APIENTRY * glDeleteShader)(GLenum) = NULL;
void (APIENTRY * glShaderSource)(GLenum, GLint, const GLchar **, const GLint *) = NULL;
void (APIENTRY * glGetShaderiv)(GLuint, GLenum, GLint*) = NULL;
void (APIENTRY * glCompileShader)(GLenum) = NULL;
GLenum (APIENTRY * glCreateProgram)(void) = NULL;
void (APIENTRY * glUseProgram)(GLenum) = NULL;
void (APIENTRY * glAttachShader)(GLenum, GLenum) = NULL;
void (APIENTRY * glLinkProgram)(GLenum) = NULL;
void (APIENTRY * glGetProgramiv)(GLuint, GLenum, GLint*) = NULL;
GLboolean (APIENTRY * glIsShader)(GLuint) = NULL;
void (APIENTRY * glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (APIENTRY * glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*) = NULL;
void (APIENTRY * glDeleteProgram)(GLuint) = NULL;
GLint(APIENTRY * glGetUniformLocation)(GLuint program, const GLchar *name) = NULL;
void (APIENTRY * glProgramUniform1f)(GLuint program, GLint location, GLfloat v0) = NULL;
void (APIENTRY * glBlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum) = NULL;
void (APIENTRY * glGenerateMipmap)(GLenum) = NULL;

void (APIENTRY * glGenVertexArrays)(GLsizei, GLuint*) = NULL;
void (APIENTRY * glDeleteVertexArrays)(GLsizei, GLuint*) = NULL;
void (APIENTRY * glBindVertexArray)(GLuint) = NULL;
GLint (APIENTRY * glGetAttribLocation)(GLuint, const GLchar *) = NULL;
GLint (APIENTRY * glVertexAttribPointer)(GLuint name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data) = NULL;

#define CHECK_FOR_PROCESS(NAME){\
if((void *)SDL_GL_GetProcAddress(NAME)==(void *)NULL){\
fprintf(stderr, "[glExtra] Init Error: " NAME " is not present in OpenGL library.\n");\
exit(1);\
}\
}
#define CHECK_FOR_PROCESS_NON_FATAL(NAME){\
if((void *)SDL_GL_GetProcAddress(NAME)==(void *)NULL){\
fprintf(stderr, "[glExtra] Init Error: " NAME " is not present in OpenGL library.\n");\
}\
}

#define GET_GL_FUNCTION(NAME, TYPING)\
CHECK_FOR_PROCESS( #NAME );\
NAME = TYPING SDL_GL_GetProcAddress( #NAME )
#define GET_GL_FUNCTION_NON_FATAL(NAME, TYPING)\
CHECK_FOR_PROCESS_NON_FATAL( #NAME );\
NAME = TYPING SDL_GL_GetProcAddress( #NAME )

void LoadGLFunctions(void){
    static int first = 0;

    if(first)
        return;
    first = 1;

    GET_GL_FUNCTION(glGenBuffers, (void (APIENTRY *)(GLsizei, GLuint*)));
    GET_GL_FUNCTION(glDeleteBuffers, (void (APIENTRY *)(GLsizei, GLuint*)));
    GET_GL_FUNCTION(glBindBuffer, (void (APIENTRY *)(GLenum, GLuint)));
    GET_GL_FUNCTION(glBufferData, (void (APIENTRY *)(GLenum, GLsizeiptr, const GLvoid *, GLenum)));
    GET_GL_FUNCTION(glBufferSubData, (void (APIENTRY *)(GLenum, GLintptr, GLsizeiptr, const GLvoid *)));
    GET_GL_FUNCTION(glCreateShader, (GLenum (APIENTRY *)(GLenum)));
    GET_GL_FUNCTION(glDeleteShader, (void (APIENTRY *)(GLenum)));
    GET_GL_FUNCTION(glShaderSource, (void (APIENTRY *)(GLuint, GLsizei, const GLchar **, const GLint *)));
    GET_GL_FUNCTION(glGetShaderiv, (void (APIENTRY *)(GLuint, GLenum, GLint *)));
    GET_GL_FUNCTION(glCompileShader, (void (APIENTRY *)(GLenum)));
    GET_GL_FUNCTION(glCreateProgram, (GLenum (APIENTRY *)(void)));
    GET_GL_FUNCTION(glUseProgram, (void (APIENTRY *)(GLenum)));
    GET_GL_FUNCTION(glAttachShader, (void (APIENTRY *)(GLenum, GLenum)));
    GET_GL_FUNCTION(glLinkProgram, (void (APIENTRY *)(GLenum)));
    GET_GL_FUNCTION(glGetProgramiv, (void (APIENTRY *)(GLuint, GLenum, GLint*)));
    GET_GL_FUNCTION(glIsShader, (GLboolean (APIENTRY *)(GLuint)));
    GET_GL_FUNCTION(glGetShaderInfoLog, (void (APIENTRY *)(GLuint, GLsizei, GLsizei *, GLchar *)));
    GET_GL_FUNCTION(glGetProgramInfoLog, (void (APIENTRY *)(GLuint, GLsizei, GLsizei*, GLchar*)));
    GET_GL_FUNCTION(glDeleteProgram, (void (APIENTRY *)(GLuint)));
    GET_GL_FUNCTION_NON_FATAL(glGetUniformLocation, (GLint (APIENTRY *)(GLuint, const GLchar *)));
    GET_GL_FUNCTION_NON_FATAL(glProgramUniform1f, (void (APIENTRY *)(GLuint program, GLint location, GLfloat v0)));
    GET_GL_FUNCTION_NON_FATAL(glBlendFuncSeparate, (void (APIENTRY *)(GLenum, GLenum, GLenum, GLenum)));
    GET_GL_FUNCTION_NON_FATAL(glGenerateMipmap, (void (APIENTRY *)(GLenum)));

    GET_GL_FUNCTION_NON_FATAL(glGenVertexArrays, (void (APIENTRY *)(GLsizei, GLuint*)));
    GET_GL_FUNCTION_NON_FATAL(glDeleteVertexArrays, (void (APIENTRY *)(GLsizei, GLuint*)));
    GET_GL_FUNCTION_NON_FATAL(glBindVertexArray, (void (APIENTRY *)(GLuint)));
    GET_GL_FUNCTION_NON_FATAL(glGetAttribLocation, (GLint (APIENTRY *)(const GLuint, GLchar*)));
    GET_GL_FUNCTION_NON_FATAL(glVertexAttribPointer, (GLint (APIENTRY *)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*)));

}
