#pragma once
#include <SDL2/SDL_opengl.h>
#include "glextra/glExtra.h"

/////
// Mouse positions are stored as 32-bit signed integers, packed (X,Y).
//

int UpdateMousePosition(int *dims);
void QueueUpdateMousePosition(int x, int y);
void SetCoordinateData(GLuint vbo, GLsizei offset);

