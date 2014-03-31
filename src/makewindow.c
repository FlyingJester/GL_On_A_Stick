#include "makewindow.h"

SDL_GLContext *glcontext = NULL;

void **MakeWindow(int w, int h){

  SDL_Window *screen = NULL;
  glcontext = malloc(sizeof(SDL_GLContext));

  /////
  // This demo is designed to work even on Intel GMA cards.
  //
  // In fact, it was written on a machine with one.
  // The horror...the horror...
  //


  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  screen = SDL_CreateWindow(WINDOW_TITLE,
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    w, h, SDL2_VIDEO_FLAGS);

  *glcontext = SDL_GL_CreateContext(screen);
  {
    void ** ret = malloc(sizeof(void *)<<1);

    ret[0] = screen;
    ret[1] = glcontext;

    return ret;
  }
}
