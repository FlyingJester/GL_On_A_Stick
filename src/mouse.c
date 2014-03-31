#include "mouse.h"
#include "drawqueue.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <assert.h>

static int MX = 0;
static int MY = 0;

static GLuint dest;
static GLsizei destoff;


void QueueUpdateMousePosition(int x, int y){
  int *dims = malloc(16);
  dims[0] = x; dims[1] = y;
  PushFunction(UpdateMousePosition, dims);
}

int UpdateMousePosition(int *dims){
  int width, height;
  float data = 0.0;
  SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);

  assert(dest);
  assert(sizeof(float)==4);

  if(MX!=dims[0]){
    MX = dims[0];

    //Convert raster coordinates to normalized coords.
    data = dims[0];
    data/= width;
    data*=2.0f;
    data-=1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, dest);
    glBufferSubData(GL_ARRAY_BUFFER, destoff, 4,
  	&data);
  }
  if(MY!=dims[1]){
    MY = dims[1];

    //Convert raster coordinates to normalized coords.
    data = dims[1];
    data/= height;
    data*=2.0f;
    data-=1.0f;
    data*=-1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, dest);
    glBufferSubData(GL_ARRAY_BUFFER, destoff+4, 4,
  	&data);
  }

  glFinish();

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  free(dims);
  return 0;
}

void SetCoordinateData(GLuint vbo, GLsizei offset){
  assert(vbo);
  dest = vbo;
  destoff = offset;
}

