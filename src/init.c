#include <stdlib.h>
#include "glextra/glExtra.h"
#include "mouse.h"
#include "makewindow.h"
#include "drawcommand.h"
#include "drawqueue.h"
#include <SDL2/SDL_opengl.h>
#include <assert.h>
#include <string.h>

static const int FrameFloat = 16; //Let the engine get up to 16 frames ahead of rendering.

const char * const WindowTitleBase = "GL On A Stick Demo - Frame Float: ";

char WindowTitle[64];

int Main(int argc, char *argv[]){

  LineArg *la = malloc(sizeof(LineArg));
  GLuint VBO = 0;
  GLsizei offsets = 0;
  SDL_Event event;
  SDL_Window *screen = NULL;
  int live = 1;

  SDL_Init(SDL_INIT_VIDEO);
  {
    void **view = MakeWindow(800, 450);
    LoadGLFunctions();

    screen = view[0];
  }
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glGenBuffers(1, &VBO);
  assert(VBO!=0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  {
    float zero = 0.0;
    GLubyte bufferData[] = {
    255, '\0', '\0', 255, //Color1
    255, '\0', '\0', 255, //Color2
    '\0', '\0', '\0', '\0', //X1
    '\0', '\0', '\0', '\0', //Y1
    '\0', '\0', '\0', '\0', //X2
    '\0', '\0', '\0', '\0', //Y2
    };

    memcpy(bufferData+8, &zero, 4);
    memcpy(bufferData+12, &zero, 4);
    memcpy(bufferData+16, &zero, 4);
    memcpy(bufferData+20, &zero, 4);

    glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_DYNAMIC_DRAW);
  }

  la->Buffer = VBO;

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  SetCoordinateData(VBO, 8);

  glFinish();

  InitDrawQueue();

  while(live){

    /////
    // Be a nice program and handle events.
    //
    while(SDL_PollEvent(&event)){
      switch(event.type){
      case (SDL_QUIT):
        live = 0;
        break;
      case (SDL_MOUSEMOTION):
        QueueUpdateMousePosition(event.motion.x, event.motion.y);
        break;
      }

    }
    QueueLine(la);
    QueueFlipScreen(screen);

    IncEngineFrame();

    {
      int diff = GetEngineFrame()-GetRenderFrame();

      assert(diff>=0);

      if(GetEngineFrame()%32){
        sprintf(WindowTitle, "%s%i - %i/%i", WindowTitleBase, diff, GetRenderFrame(), GetEngineFrame());
      }

      SDL_SetWindowTitle(screen, WindowTitle);

      while(diff>FrameFloat){
        diff = GetEngineFrame()-GetRenderFrame();
        SDL_Delay(0x10);
      }
    }
  }

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  CloseDrawQueue();

  return EXIT_SUCCESS;
}
