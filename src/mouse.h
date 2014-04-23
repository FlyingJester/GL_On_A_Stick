#pragma once
#include <SDL2/SDL_opengl.h>
#ifdef _WIN32
#include "glextra/glExtra.h"
#else
#include <GL/gl.h>
#endif

/////
// Mouse positions are stored as 32-bit signed integers, packed (X,Y).
//

int UpdateMousePosition(int *dims);
void QueueUpdateMousePosition(int x, int y);
void SetCoordinateData(GLuint vbo, GLsizei offset);

