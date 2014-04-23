#ifdef _WIN32
#include "glextra/glExtra.h"
#else
#include <GL/gl.h>
#endif
#include "drawcommand.h"
#include "drawqueue.h"
#include <assert.h>
//#include <SDL2/SDL_opengl.h>

int QueueLine(void *arg){
  PushFunction(Line, arg);
  return 0;
}

int Line(void *arg){

  glBindBuffer(GL_ARRAY_BUFFER, ((LineArg *)arg)->Buffer);

  glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);
  glVertexPointer(2, GL_FLOAT, 0, (void *)8);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
  glDrawArrays(GL_LINES, 0, 2);
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return 0;
}

int QueueFlipScreen(void *window){
  assert(window);
  PushFunction(FlipScreen, window);
  return 0;
}
int FlipScreen(void *window){
//  SDL_GLContext context = SDL_GL_GetCurrentContext();
//  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SwapWindow(window);
  IncRenderFrame();
  glClear(GL_COLOR_BUFFER_BIT);
  return 0;
}
